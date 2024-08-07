#include "ShaderResource.h"

#include "Core/Log.h"
#include "Core/Path.hpp"
#include "Core/Time.h"
#include "RenderCore/Shader.h"
#include "Resource/FileIO.hpp"
#include "Resource/ShaderCompiler.h"

namespace sl
{

namespace
{

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

std::string GetShaderBinaryCachePath(std::string_view name)
{
	std::string path = Path::FromeRoot("Engine/Binary/");
	path += name;
	path.replace(path.find_last_of("."), 1, "_");
#if defined(SL_DEBUG)
	path += "d";
#endif
	path += ".bin";
	
	return path;
}

} // namespace

ShaderResource::ShaderResource(std::string_view vsPath, std::string_view fsPath) :
	m_programType(ShaderProgramType::Standard)
{
	m_shaders[0].m_type = ShaderType::VertexShader;
	m_shaders[0].m_name = Path::NameWithExtension(vsPath);
	m_shaders[0].m_assetPath = vsPath;
	m_shaders[0].m_binaryPath = GetShaderBinaryCachePath(m_shaders[0].m_name);

	m_shaders[1].m_type = ShaderType::FragmentShader;
	m_shaders[1].m_name = Path::NameWithExtension(fsPath);
	m_shaders[1].m_assetPath = fsPath;
	m_shaders[1].m_binaryPath = GetShaderBinaryCachePath(m_shaders[1].m_name);

	if (Path::Exists(m_shaders[0].m_binaryPath) && Path::Exists(m_shaders[1].m_binaryPath))
	{
		SetStatus(ResourceStatus::Loading);
	}
	else
	{
		SetStatus(ResourceStatus::Importing);
	}
}

ShaderResource::ShaderResource(std::string_view path, ShaderProgramType type) :
	m_programType(type)
{
	std::filesystem::path binaryCatchPath = Path::FromeRoot("Engine/Binary");

	m_shaders[0].m_type = ProgramTypeToShaderType(m_programType);
	m_shaders[0].m_name = Path::NameWithExtension(path);
	m_shaders[0].m_assetPath = path;
	m_shaders[0].m_binaryPath = GetShaderBinaryCachePath(m_shaders[0].m_name);

	if (Path::Exists(m_shaders[0].m_binaryPath))
	{
		SetStatus(ResourceStatus::Loading);
	}
	else
	{
		SetStatus(ResourceStatus::Importing);
	}
}

ShaderResource::~ShaderResource()
{
	OnDestroy();
}

void ShaderResource::OnImport()
{
	SL_LOG_TRACE("Loading shader: \"{}\"", m_shaders[0].m_assetPath.c_str());
	m_shaders[0].m_source = FileIO::ReadString(m_shaders[0].m_assetPath);

	if (m_programType == ShaderProgramType::Standard)
	{
		SL_LOG_TRACE("Loading shader: \"{}\"", m_shaders[1].m_assetPath.c_str());
		m_shaders[1].m_source = FileIO::ReadString(m_shaders[1].m_assetPath);
	}

	if (m_shaders[0].m_source.empty() ||
		(m_programType == ShaderProgramType::Standard && m_shaders[1].m_source.empty()))
	{
		SL_LOG_ERROR("Failed to import shader!");
		SetStatus(ResourceStatus::Destroying);
		return;
	}

	SetStatus(ResourceStatus::Building);
}

void ShaderResource::OnBuild()
{
	SL_LOG_TRACE("Compiling SPIR-V: \"{}\"", m_shaders[0].m_name.c_str());
	
	Timer timer;
	auto spirvData = ShaderCompiler::SourceToSpirv(m_shaders[0]);
	FileIO::WriteBinary(m_shaders[0].m_binaryPath,
		reinterpret_cast<char *>(spirvData.data()), spirvData.size() * sizeof(uint32_t));
	m_shaders[0].m_source = ShaderCompiler::SpirvToSource(std::move(spirvData));
	SL_LOG_TRACE("  Done in {} ms", timer.GetDuration());

	if (m_programType == ShaderProgramType::Standard)
	{
		SL_LOG_TRACE("Compiling SPIR-V: \"{}\"", m_shaders[1].m_name.c_str());

		timer.Reset();
		auto fragSpirvData = ShaderCompiler::SourceToSpirv(m_shaders[1]);
		FileIO::WriteBinary(m_shaders[1].m_binaryPath,
			reinterpret_cast<char *>(fragSpirvData.data()), fragSpirvData.size() * sizeof(uint32_t));
		m_shaders[1].m_source = ShaderCompiler::SpirvToSource(std::move(fragSpirvData));
		SL_LOG_TRACE("  Done in {} ms", timer.GetDuration());
	}

	if (m_shaders[0].m_source.empty() ||
		(m_programType == ShaderProgramType::Standard && m_shaders[1].m_source.empty()))
	{
		SL_LOG_ERROR("Failed to build shader!");
		SetStatus(ResourceStatus::Destroying);
		return;
	}

	SetStatus(ResourceStatus::Uploading);
}

void ShaderResource::OnLoad()
{
	SL_LOG_TRACE("Loading SPIR-V cache: \"{}\"", m_shaders[0].m_binaryPath.c_str());
	auto spirvBinary = FileIO::ReadBinary<uint32_t>(m_shaders[0].m_binaryPath);
	m_shaders[0].m_source = ShaderCompiler::SpirvToSource(std::move(spirvBinary));

	if (m_programType == ShaderProgramType::Standard)
	{
		SL_LOG_TRACE("Loading SPIR-V cache: \"{}\"", m_shaders[1].m_binaryPath.c_str());
		auto fragSpirvBinary = FileIO::ReadBinary<uint32_t>(m_shaders[1].m_binaryPath);
		m_shaders[1].m_source = ShaderCompiler::SpirvToSource(std::move(fragSpirvBinary));
	}

	if (m_shaders[0].m_source.empty() ||
		(m_programType == ShaderProgramType::Standard && m_shaders[1].m_source.empty()))
	{
		SL_LOG_ERROR("Failed to load shader binary cache!");
		SetStatus(ResourceStatus::Destroying);
		return;
	}

	SetStatus(ResourceStatus::Uploading);
}

void ShaderResource::OnUpload()
{
	if (m_programType == ShaderProgramType::Standard)
	{
		SL_LOG_TRACE("Uploading shader program: \"{}\"", Path::NameWithoutExtension(m_shaders[0].m_name));
		m_pShaderProgram.reset(Shader::Create(m_shaders[0].m_source, m_shaders[1].m_source));
	}
	else
	{
		SL_LOG_TRACE("Uploading shader program: \"{}\"", Path::NameWithoutExtension(m_shaders[0].m_name));
		m_pShaderProgram.reset(Shader::Create(m_shaders[0].m_source, m_shaders[0].m_type));
	}

	if (!m_pShaderProgram)
	{
		SL_LOG_ERROR("Failed to create shader GPU handle!");
		SetStatus(ResourceStatus::Destroying);
		return;
	}

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
		DestroyCPUData();
	}
}

void ShaderResource::OnDestroy()
{
	DestroyCPUData();
	m_pShaderProgram.reset();

	SetStatus(ResourceStatus::Destroyed);
}

void ShaderResource::DestroyCPUData()
{
	m_shaders[0].m_source.clear();
	std::string().swap(m_shaders[0].m_source);

	if (m_programType == ShaderProgramType::Standard)
	{
		m_shaders[1].m_source.clear();
		std::string().swap(m_shaders[1].m_source);
	}
}

} // namespace sl
