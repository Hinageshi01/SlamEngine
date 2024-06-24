#include "ShaderResource.h"

#include "Core/Log.h"
#include "Core/Path.hpp"
#include "nameof/nameof.hpp"
#include "RenderCore/RenderCore.h"
#include "RenderCore/Shader.h"
#include "Resource/FileIO.h"

#include <shaderc/shaderc.hpp>

// TEMPLATE: Replace these to SPRI-V
#include "Platform/OpenGL/OpenGLDefines.h"
#include <glad/glad.h>

namespace sl
{

namespace
{

constexpr const char *BackendToDef[nameof::enum_count<GraphicsBackend>()] =
{
	"SL_NONE",	    // GraphicsBackend::None
	"SL_OPENGL",    // GraphicsBackend::OpenGL,
	"SL_VULKAN",    // GraphicsBackend::Vulkan,
	"SL_DIRECTX11", // GraphicsBackend::DirectX11,
	"SL_DIRECTX12", // GraphicsBackend::DirectX12,
	"SL_METAL",	    // GraphicsBackend::Metal,
};

constexpr shaderc_shader_kind ShaderTypeToShaderKind[nameof::enum_count<ShaderType>()] =
{
	shaderc_vertex_shader,	 // ShaderType::VertexShader
	shaderc_fragment_shader, // ShaderType::FragmentShader
	shaderc_compute_shader,	 // ShaderType::ComputeShader
};

ShaderType ProgramTypeToShaderType(ShaderProgramType programType)
{
	switch (programType)
	{
		case sl::ShaderProgramType::Compute:
		{
			return ShaderType::ComputeShader;
			break;
		}
		default:
		{
			SL_ASSERT(false);
			return ShaderType::VertexShader;
			break;
		}
	}
}

uint32_t CompileShader(sl::ShaderInfo& info)
{
	{
		// 1. Preprocess
		shaderc::Compiler compiler;
		shaderc::CompileOptions options;

		// Include
		//IncluderInterface
		
		// Defines
		options.AddMacroDefinition(BackendToDef[(size_t)RenderCore::GetBackend()]);

		shaderc::PreprocessedSourceCompilationResult result = compiler.PreprocessGlsl(
			info.m_rowData.c_str(), ShaderTypeToShaderKind[(size_t)info.m_type],
			info.m_name.c_str(), options);
		
		if (result.GetCompilationStatus() != shaderc_compilation_status_success)
		{
			SL_LOG_ERROR("Shader preprocess failed: \"{}\"", info.m_name.c_str());
			return 0;
		}
		
		info.m_rowData = { result.cbegin(), result.cend() };
	}

	GLuint shaderHandle = glCreateShader(GLShaderType[(size_t)info.m_type]);

	const GLchar *pSource = (const GLchar *)info.m_rowData.data();
	const GLint GLsize = (GLint)info.m_rowData.size();
	glShaderSource(shaderHandle, 1, &pSource, &GLsize);
	glCompileShader(shaderHandle);

#ifndef SL_FINAL
	GLint isCompiled = 0;
	glGetShaderiv(shaderHandle, GL_COMPILE_STATUS, &isCompiled);
	if (GL_FALSE == isCompiled)
	{
		GLint maxLength = 0;
		glGetShaderiv(shaderHandle, GL_INFO_LOG_LENGTH, &maxLength);

		// The maxLength includes the NULL character
		std::vector<GLchar> infoLog(maxLength);
		glGetShaderInfoLog(shaderHandle, maxLength, &maxLength, infoLog.data());
		SL_LOG_ERROR("Shader compile failed: {}", infoLog.data());

		glDeleteShader(shaderHandle);

		return 0;
	}
#endif

	return shaderHandle;
}

uint32_t CompileProgram(uint32_t vsHandle, uint32_t fsHandle)
{
	uint32_t programHandle = glCreateProgram();
	glAttachShader(programHandle, vsHandle);
	glAttachShader(programHandle, fsHandle);
	glLinkProgram(programHandle);

#ifndef SL_FINAL
	GLint isLinked = 0;
	glGetProgramiv(programHandle, GL_LINK_STATUS, &isLinked);
	if (isLinked == GL_FALSE)
	{
		GLint maxLength = 0;
		glGetProgramiv(programHandle, GL_INFO_LOG_LENGTH, &maxLength);

		// The maxLength includes the NULL character
		std::vector<GLchar> infoLog(maxLength);
		glGetProgramInfoLog(programHandle, maxLength, &maxLength, infoLog.data());
		SL_LOG_ERROR("Shader program compile failed: {}", infoLog.data());

		glDeleteProgram(programHandle);
		glDeleteShader(vsHandle);
		glDeleteShader(fsHandle);

		return 0;
	}
#endif

	glDetachShader(programHandle, vsHandle);
	glDetachShader(programHandle, fsHandle);
	glDeleteShader(vsHandle);
	glDeleteShader(fsHandle);

	return programHandle;
}

uint32_t CompileProgram(uint32_t handle)
{
	uint32_t programHandle = glCreateProgram();
	glAttachShader(programHandle, handle);
	glLinkProgram(programHandle);

#ifndef SL_FINAL
	GLint isLinked = 0;
	glGetProgramiv(programHandle, GL_LINK_STATUS, &isLinked);
	if (isLinked == GL_FALSE)
	{
		GLint maxLength = 0;
		glGetProgramiv(programHandle, GL_INFO_LOG_LENGTH, &maxLength);

		// The maxLength includes the NULL character
		std::vector<GLchar> infoLog(maxLength);
		glGetProgramInfoLog(programHandle, maxLength, &maxLength, infoLog.data());
		SL_LOG_ERROR("Shader program compile failed: {}", infoLog.data());

		glDeleteProgram(programHandle);
		glDeleteShader(handle);

		return 0;
	}
#endif

	glDetachShader(programHandle, handle);
	glDeleteShader(handle);

	return programHandle;
}

} // namespace

ShaderResource::ShaderResource(std::string_view vsPath, std::string_view fsPath) :
	m_programType(ShaderProgramType::Standard)
{
	m_shaders[0].m_type = ShaderType::VertexShader;
	m_shaders[0].m_name = Path::NameWithoutExtension(vsPath);
	m_shaders[0].m_assetPath = vsPath;

	m_shaders[1].m_type = ShaderType::FragmentShader;
	m_shaders[1].m_name = Path::NameWithoutExtension(fsPath);
	m_shaders[1].m_assetPath = fsPath;

	SetStatus(ResourceStatus::Importing);
}

ShaderResource::ShaderResource(ShaderProgramType type, std::string_view path) :
	m_programType(type)
{
	m_shaders[0].m_type = ProgramTypeToShaderType(m_programType);
	m_shaders[0].m_name = Path::NameWithoutExtension(path);
	m_shaders[0].m_assetPath = path;

	SetStatus(ResourceStatus::Importing);
}

ShaderResource::~ShaderResource()
{
	OnDestroy();
}

void ShaderResource::OnImport()
{
	SL_LOG_TRACE("Loading shader: \"{}\"", m_shaders[0].m_assetPath.c_str());
	m_shaders[0].m_rowData = FileIO::LoadString(m_shaders[0].m_assetPath);

	if (ShaderProgramType::Standard == m_programType)
	{
		SL_LOG_TRACE("Loading shader: \"{}\"", m_shaders[1].m_assetPath.c_str());
		m_shaders[1].m_rowData = FileIO::LoadString(m_shaders[1].m_assetPath);
	}

	SetStatus(ResourceStatus::Building);
}

void ShaderResource::OnBuild()
{
	SetStatus(ResourceStatus::Uploading);
}

void ShaderResource::OnLoad()
{
	SetStatus(ResourceStatus::Uploading);
}

void ShaderResource::OnUpload()
{
	SL_LOG_TRACE("Compiling vertex shader: \"{}\"", Path::NameWithoutExtension(m_shaders[0].m_assetPath).c_str());
	uint32_t shaderHandle = CompileShader(m_shaders[0]);

	if (0 == shaderHandle)
	{
		SetStatus(ResourceStatus::Destroying);
		return;
	}

	uint32_t programHandle = 0;
	if (ShaderProgramType::Standard == m_programType)
	{
		SL_LOG_TRACE("Compiling fragment shader: \"{}\"", Path::NameWithoutExtension(m_shaders[1].m_assetPath).c_str());
		uint32_t fsHandle = CompileShader(m_shaders[1]);

		if (0 == fsHandle)
		{
			SetStatus(ResourceStatus::Destroying);
			return;
		}

		SL_LOG_TRACE("Compiling shader program");
		programHandle = CompileProgram(shaderHandle, fsHandle);
	}
	else
	{
		SL_LOG_TRACE("Compiling shader program");
		programHandle = CompileProgram(shaderHandle);
	}

	if (0 == programHandle)
	{
		SetStatus(ResourceStatus::Destroying);
		return;
	}

	m_pShaderProgram.reset(Shader::Creat(programHandle));

	SetStatus(ResourceStatus::Ready);
}

void ShaderResource::OnReady()
{
	static uint32_t frameCount = 0;
	if (frameCount < 60)
	{
		++frameCount;
	}
	else if (frameCount == 60)
	{
		DestroyRawData();
	}
}

void ShaderResource::OnDestroy()
{
	DestroyRawData();
	m_pShaderProgram.reset();

	SetStatus(ResourceStatus::Destroyed);
}

void ShaderResource::DestroyRawData()
{
	m_shaders[0].m_rowData.clear();
	std::string().swap(m_shaders[0].m_rowData);

	m_shaders[1].m_rowData.clear();
	std::string().swap(m_shaders[1].m_rowData);
}

} // namespace sl

static_assert(std::is_same_v<GLchar, char>);
