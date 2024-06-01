#pragma once

#include "Core/Defines.h"
#include "Core/EnumOf.hpp"

#include <glad/glad.h>

namespace sl
{

inline constexpr GLenum GLAttribType[nameof::enum_count<AttribType>()] =
{
	GL_BYTE,           // AttribType::Int8
	GL_UNSIGNED_BYTE,  // AttribType::Uint8
	GL_SHORT,          // AttribType::Int16
	GL_UNSIGNED_SHORT, // AttribType::Uint16
	GL_INT,            // AttribType::Int32
	GL_UNSIGNED_INT,   // AttribType::Uint32
	GL_HALF_FLOAT,     // AttribType::Half
	GL_FLOAT,          // AttribType::Float
	GL_DOUBLE,         // AttribType::Double
};

inline constexpr GLenum GLInternalTextureFormat[nameof::enum_count<TextureFormat>()] =
{
	GL_R8,                 // TextureFormat::R8
	GL_R8_SNORM,           // TextureFormat::R8S
	GL_R8UI,               // TextureFormat::R8U
	GL_R8I,                // TextureFormat::R8I
	GL_R16,                // TextureFormat::R16
	GL_R16_SNORM,          // TextureFormat::R16S
	GL_R16UI,              // TextureFormat::R16U
	GL_R16I,               // TextureFormat::R16I
	GL_R16F,               // TextureFormat::R16F
	GL_R32UI,              // TextureFormat::R32U
	GL_R32I,               // TextureFormat::R32I
	GL_R32F,               // TextureFormat::R32F

	GL_RG8,                // TextureFormat::RG8
	GL_RG8_SNORM,          // TextureFormat::RG8S
	GL_RG8UI,              // TextureFormat::RG8U
	GL_RG8I,               // TextureFormat::RG8I
	GL_RG16,               // TextureFormat::RG16
	GL_RG16_SNORM,         // TextureFormat::RG16S
	GL_RG16UI,             // TextureFormat::RG16U
	GL_RG16I,              // TextureFormat::RG16I
	GL_RG16F,              // TextureFormat::RG16F
	GL_RG32UI,             // TextureFormat::RG32U
	GL_RG32I,              // TextureFormat::RG32I
	GL_RG32F,              // TextureFormat::RG32F

	GL_RGB8,               // TextureFormat::RGB8
	GL_RGB8_SNORM,         // TextureFormat::RGB8S
	GL_RGB8UI,             // TextureFormat::RGB8U
	GL_RGB8I,              // TextureFormat::RGB8I
	GL_RGB16,              // TextureFormat::RGB16
	GL_RGB16_SNORM,        // TextureFormat::RGB16S
	GL_RGB16UI,            // TextureFormat::RGB16U
	GL_RGB16I,             // TextureFormat::RGB16I
	GL_RGB16F,             // TextureFormat::RGB16F
	GL_RGB32UI,            // TextureFormat::RGB32U
	GL_RGB32I,             // TextureFormat::RGB32I
	GL_RGB32F,             // TextureFormat::RGB32F

	GL_RGBA8,              // TextureFormat::RGBA8
	GL_RGBA8_SNORM,        // TextureFormat::RGBA8S
	GL_RGBA8UI,            // TextureFormat::RGBA8U
	GL_RGBA8I,             // TextureFormat::RGBA8I
	GL_RGBA16,             // TextureFormat::RGBA16
	GL_RGBA16_SNORM,       // TextureFormat::RGBA16S
	GL_RGBA16UI,           // TextureFormat::RGBA16U
	GL_RGBA16I,            // TextureFormat::RGBA16I
	GL_RGBA16F,            // TextureFormat::RGBA16F
	GL_RGBA32UI,           // TextureFormat::RGBA32U
	GL_RGBA32I,            // TextureFormat::RGBA32I
	GL_RGBA32F,            // TextureFormat::RGBA32F

	GL_DEPTH_COMPONENT16,  // TextureFormat::D16
	GL_DEPTH_COMPONENT24,  // TextureFormat::D24
	GL_DEPTH_COMPONENT32F, // TextureFormat::D32F
	GL_DEPTH_COMPONENT32,  // TextureFormat::D32
	GL_DEPTH24_STENCIL8,   // TextureFormat::D24S8
	GL_DEPTH32F_STENCIL8,  // TextureFormat::D32FS8
	GL_STENCIL_INDEX8,	   // TextureFormat::S8
};

inline constexpr GLenum GLTextureFormat[nameof::enum_count<TextureFormat>()] =
{
	GL_RED,             // TextureFormat::R8
	GL_RED,             // TextureFormat::R8S
	GL_RED_INTEGER,     // TextureFormat::R8U
	GL_RED_INTEGER,     // TextureFormat::R8I
	GL_RED,             // TextureFormat::R16
	GL_RED,             // TextureFormat::R16S
	GL_RED_INTEGER,     // TextureFormat::R16U
	GL_RED_INTEGER,     // TextureFormat::R16I
	GL_RED,             // TextureFormat::R16F
	GL_RED_INTEGER,     // TextureFormat::R32U
	GL_RED_INTEGER,     // TextureFormat::R32I
	GL_RED,             // TextureFormat::R32F
	
	GL_RG,              // TextureFormat::RG8
	GL_RG,              // TextureFormat::RG8S
	GL_RG_INTEGER,      // TextureFormat::RG8U
	GL_RG_INTEGER,      // TextureFormat::RG8I
	GL_RG,              // TextureFormat::RG16
	GL_RG,              // TextureFormat::RG16S
	GL_RG_INTEGER,      // TextureFormat::RG16U
	GL_RG_INTEGER,      // TextureFormat::RG16I
	GL_RG,              // TextureFormat::RG16F
	GL_RG_INTEGER,      // TextureFormat::RG32U
	GL_RG_INTEGER,      // TextureFormat::RG32I
	GL_RG,              // TextureFormat::RG32F
	
	GL_RGB,             // TextureFormat::RGB8
	GL_RGB,             // TextureFormat::RGB8S
	GL_RGB_INTEGER,     // TextureFormat::RGB8U
	GL_RGB_INTEGER,     // TextureFormat::RGB8I
	GL_RGB,             // TextureFormat::RGB16
	GL_RGB,             // TextureFormat::RGB16S
	GL_RGB_INTEGER,     // TextureFormat::RGB16U
	GL_RGB_INTEGER,     // TextureFormat::RGB16I
	GL_RGB,             // TextureFormat::RGB16F
	GL_RGB_INTEGER,     // TextureFormat::RGB32U
	GL_RGB_INTEGER,     // TextureFormat::RGB32I
	GL_RGB,             // TextureFormat::RGB32F
	
	GL_RGBA,            // TextureFormat::RGBA8
	GL_RGBA,            // TextureFormat::RGBA8S
	GL_RGBA_INTEGER,    // TextureFormat::RGBA8U
	GL_RGBA_INTEGER,    // TextureFormat::RGBA8I
	GL_RGBA,            // TextureFormat::RGBA16
	GL_RGBA,            // TextureFormat::RGBA16S
	GL_RGBA_INTEGER,    // TextureFormat::RGBA16U
	GL_RGBA_INTEGER,    // TextureFormat::RGBA16I
	GL_RGBA,            // TextureFormat::RGBA16F
	GL_RGBA_INTEGER,    // TextureFormat::RGBA32U
	GL_RGBA_INTEGER,    // TextureFormat::RGBA32I
	GL_RGBA,            // TextureFormat::RGBA32F

	GL_DEPTH_COMPONENT, // TextureFormat::D16
	GL_DEPTH_COMPONENT, // TextureFormat::D24
	GL_DEPTH_COMPONENT, // TextureFormat::D32F
	GL_DEPTH_COMPONENT, // TextureFormat::D32
	GL_DEPTH_STENCIL,   // TextureFormat::D24S8
	GL_DEPTH_STENCIL,   // TextureFormat::D32FS8
	GL_STENCIL_INDEX,   // TextureFormat::S8
};

inline constexpr GLenum GLDataType[nameof::enum_count<TextureFormat>()] =
{
	GL_UNSIGNED_BYTE,  // TextureFormat::R8
	GL_BYTE,           // TextureFormat::R8S
	GL_UNSIGNED_BYTE,  // TextureFormat::R8U
	GL_BYTE,           // TextureFormat::R8I
	GL_UNSIGNED_SHORT, // TextureFormat::R16
	GL_SHORT,          // TextureFormat::R16S
	GL_UNSIGNED_SHORT, // TextureFormat::R16U
	GL_SHORT,          // TextureFormat::R16I
	GL_HALF_FLOAT,     // TextureFormat::R16F
	GL_UNSIGNED_INT,   // TextureFormat::R32U
	GL_INT,            // TextureFormat::R32I
	GL_FLOAT,          // TextureFormat::R32F

	GL_UNSIGNED_BYTE,  // TextureFormat::RG8
	GL_BYTE,           // TextureFormat::RG8S
	GL_UNSIGNED_BYTE,  // TextureFormat::RG8U
	GL_BYTE,           // TextureFormat::RG8I
	GL_UNSIGNED_SHORT, // TextureFormat::RG16
	GL_SHORT,          // TextureFormat::RG16S
	GL_UNSIGNED_SHORT, // TextureFormat::RG16U
	GL_SHORT,          // TextureFormat::RG16I
	GL_HALF_FLOAT,     // TextureFormat::RG16F
	GL_UNSIGNED_INT,   // TextureFormat::RG32U
	GL_INT,            // TextureFormat::RG32I
	GL_FLOAT,          // TextureFormat::RG32F

	GL_UNSIGNED_BYTE,  // TextureFormat::RGB8
	GL_BYTE,           // TextureFormat::RGB8S
	GL_UNSIGNED_BYTE,  // TextureFormat::RGB8U
	GL_BYTE,           // TextureFormat::RGB8I
	GL_UNSIGNED_SHORT, // TextureFormat::RGB16
	GL_SHORT,          // TextureFormat::RGB16S
	GL_UNSIGNED_SHORT, // TextureFormat::RGB16U
	GL_SHORT,          // TextureFormat::RGB16I
	GL_HALF_FLOAT,     // TextureFormat::RGB16F
	GL_UNSIGNED_INT,   // TextureFormat::RGB32U
	GL_INT,            // TextureFormat::RGB32I
	GL_FLOAT,          // TextureFormat::RGB32F

	GL_UNSIGNED_BYTE,  // TextureFormat::RGBA8
	GL_BYTE,           // TextureFormat::RGBA8S
	GL_UNSIGNED_BYTE,  // TextureFormat::RGBA8U
	GL_BYTE,           // TextureFormat::RGBA8I
	GL_UNSIGNED_SHORT, // TextureFormat::RGBA16
	GL_SHORT,          // TextureFormat::RGBA16S
	GL_UNSIGNED_SHORT, // TextureFormat::RGBA16U
	GL_SHORT,          // TextureFormat::RGBA16I
	GL_HALF_FLOAT,     // TextureFormat::RGBA16F
	GL_UNSIGNED_INT,   // TextureFormat::RGBA32U
	GL_INT,            // TextureFormat::RGBA32I
	GL_FLOAT,          // TextureFormat::RGBA32F

	// TODO: Depth / Stencil data type
	GL_INVALID_ENUM,   // TextureFormat::D16
	GL_INVALID_ENUM,   // TextureFormat::D24
	GL_INVALID_ENUM,   // TextureFormat::D32F
	GL_INVALID_ENUM,   // TextureFormat::D32
	GL_INVALID_ENUM,   // TextureFormat::D24S8
	GL_INVALID_ENUM,   // TextureFormat::D32FS8
	GL_INVALID_ENUM,   // TextureFormat::S8
};

inline constexpr GLenum GLAttachmentPoint[nameof::enum_count<AttachmentType>()] =
{
	GL_COLOR_ATTACHMENT0,		 // AttachmentType::Color
	GL_DEPTH_ATTACHMENT,		 // AttachmentType::Depth
	GL_STENCIL_ATTACHMENT,		 // AttachmentType::DepthAndStencil
	GL_DEPTH_STENCIL_ATTACHMENT, // AttachmentType::Stencil
};

inline constexpr GLenum GLShaderType[nameof::enum_count<ShaderType>()] =
{
	GL_VERTEX_SHADER,   // ShaderType::VertexShader
	GL_FRAGMENT_SHADER, // ShaderType::FragmentShader
	GL_COMPUTE_SHADER,  // ShaderType::ComputeShader
};

inline constexpr GLenum GLCompareFunc[nameof::enum_count<CompareFunction>()] =
{
	GL_NEVER,	 // DepthFunc::Never 
	GL_LESS,	 // DepthFunc::Less
	GL_LEQUAL,	 // DepthFunc::LessEqual
	GL_GREATER,	 // DepthFunc::Greater
	GL_GEQUAL,	 // DepthFunc::GreaterEqual
	GL_EQUAL,    // DepthFunc::Equal
	GL_NOTEQUAL, // DepthFunc::NotEqual
	GL_ALWAYS,	 // DepthFunc::Always
};

inline constexpr GLenum GLBlendFunc[nameof::enum_count<BlendFunction>()] =
{
	GL_ZERO,					 // BlendFunction::Zero
	GL_ONE,						 // BlendFunction::One
	GL_SRC_COLOR,				 // BlendFunction::SrcColor
	GL_ONE_MINUS_SRC_COLOR,		 // BlendFunction::OneMinusSrcColor
	GL_DST_COLOR,				 // BlendFunction::DstColor
	GL_ONE_MINUS_DST_COLOR,		 // BlendFunction::OneMinusDstColor
	GL_SRC_ALPHA,				 // BlendFunction::SrcAlpha
	GL_ONE_MINUS_SRC_ALPHA,		 // BlendFunction::OneMinusSrcAlpha
	GL_DST_ALPHA,				 // BlendFunction::DstAlpha
	GL_ONE_MINUS_DST_ALPHA,		 // BlendFunction::OneMinusDstAlpha
	GL_CONSTANT_COLOR,			 // BlendFunction::ConstantColor
	GL_ONE_MINUS_CONSTANT_COLOR, // BlendFunction::OneMinusConstantColor
	GL_CONSTANT_ALPHA,			 // BlendFunction::ConstantAlpha
	GL_ONE_MINUS_CONSTANT_ALPHA, // BlendFunction::OneMinusConstantAlpha
	GL_SRC_ALPHA_SATURATE,		 // BlendFunction::SrcAlphaSaturate
	GL_SRC1_COLOR,				 // BlendFunction::Src1Color
	GL_ONE_MINUS_SRC1_COLOR,	 // BlendFunction::OneMinusSrc1Color
	GL_SRC1_ALPHA,				 // BlendFunction::Src1Alpha
	GL_ONE_MINUS_SRC1_ALPHA,	 // BlendFunction::OneMinusSrc1Alpha
};

inline constexpr GLenum GLBlendEquation[nameof::enum_count<BlendEquation>()] =
{
	GL_FUNC_ADD,			  // BlendEquation::FunctionAdd
	GL_FUNC_SUBTRACT,		  // BlendEquation::FunctionSubtract
	GL_FUNC_REVERSE_SUBTRACT, // BlendEquation::FunctionReverseSubTrect
	GL_MIN,					  // BlendEquation::Min
	GL_MAX,					  // BlendEquation::Max
};

inline constexpr GLenum GLStencilOp[nameof::enum_count<StencilOperation>()] =
{
	GL_KEEP,	  // StencilOp::Keep
	GL_ZERO,	  // StencilOp::Zero
	GL_REPLACE,	  // StencilOp::Replace
	GL_INCR,	  // StencilOp::Increase
	GL_INCR_WRAP, // StencilOp::IncreaseWrap
	GL_DECR,	  // StencilOp::Decrease
	GL_DECR_WRAP, // StencilOp::DecreaseWrap
	GL_INVERT,	  // StencilOp::Invert
};

inline constexpr GLenum GLCullingMode[nameof::enum_count<CullingMode>() - 1] =
{
	GL_FRONT,		   // CullingMode::Front
	GL_BACK,		   // CullingMode::Back
	GL_FRONT_AND_BACK, // CullingMode::FrontAndBack
};

inline constexpr GLenum GLFrontFace[nameof::enum_count<FrontFace>()] =
{
	GL_CCW, // FrontFace::CCW
	GL_CW,  // FrontFace::CW
};

inline constexpr GLint GLTextureFilter[] =
{
	0,
	GL_NEAREST,
	GL_LINEAR,
};

inline constexpr GLint GLTextureMipmapFilter[] =
{
	0,
	GL_NEAREST_MIPMAP_NEAREST,
	GL_NEAREST_MIPMAP_LINEAR,
	GL_LINEAR_MIPMAP_NEAREST,
	GL_LINEAR_MIPMAP_LINEAR,
};

inline constexpr GLint GLTextureWrap[] =
{
	0,
	GL_CLAMP_TO_EDGE,
	GL_CLAMP_TO_BORDER,
	GL_MIRRORED_REPEAT,
	GL_REPEAT,
	// GL_MIRROR_CLAMP_TO_EDGE,
};

} // namespace sl
