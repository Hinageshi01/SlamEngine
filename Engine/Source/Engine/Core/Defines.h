#pragma once

#include <cstdint>

namespace sl
{

enum class GraphicsBackend : uint8_t
{
	None = 0,
	OpenGL,
	DirectX11,
	DirectX12,
	Vulkan,
	Metal,
};

enum class AttribType : uint8_t
{
	Int8 = 0,
	Uint8,
	Int16,
	Uint16,
	Int32,
	Uint32,

	Half,
	Float,
	Double,
};

enum class TextureFormat : uint8_t
{
	R8 = 0,  // Unsigned normalized integer
	R8S,     // Signed normalized integer
	R8U,     // Unsigned integer
	R8I,     // Signed integer
	R16,
	R16S,
	R16U,
	R16I,
	R16F,
	R32U,
	R32I,
	R32F,

	RG8,
	RG8S,
	RG8U,
	RG8I,
	RG16,
	RG16S,
	RG16U,
	RG16I,
	RG16F,
	RG32U,
	RG32I,
	RG32F,

	RGB8,
	RGB8S,
	RGB8U,
	RGB8I,
	RGB16,
	RGB16S,
	RGB16U,
	RGB16I,
	RGB16F,
	RGB32U,
	RGB32I,
	RGB32F,

	RGBA8,
	RGBA8S,
	RGBA8U,
	RGBA8I,
	RGBA16,
	RGBA16S,
	RGBA16U,
	RGBA16I,
	RGBA16F,
	RGBA32U,
	RGBA32I,
	RGBA32F,

	D16,
	D24,
	D32,
	D32F,
	D24S8,
	D32FS8,
	S8,

	// TODO: Compress format
};

enum class AttachmentType : uint8_t
{
	Color,
	Depth,
	DepthAndStencil,
	Stencil,
};

inline AttachmentType GetAttachmentType(TextureFormat format)
{
	switch (format)
	{
		case TextureFormat::D16:
		case TextureFormat::D24:
		case TextureFormat::D32:
		case TextureFormat::D32F:
		{
			return AttachmentType::Depth;
		}
		case TextureFormat::D24S8:
		case TextureFormat::D32FS8:
		{
			return AttachmentType::DepthAndStencil;
		}
		case TextureFormat::S8:
		{
			return AttachmentType::Stencil;
		}
		default:
		{
			return AttachmentType::Color;
		}
	}
}

enum class ShaderType : uint8_t
{
	VertexShader = 0,
	FragmentShader,
	ComputeShader,
	// Geometry
	// Raytracing
};

enum class ShaderProgramType : uint8_t
{
	Standard = 0,
	Compute,
	// VertexOnly
};

enum class CompareFunction : uint8_t
{
	Never = 0,
	Less,
	LessEqual,
	Greater,
	GreaterEqual,
	Equal,
	NotEqual,
	Always,
};

enum class BlendFunction : uint8_t
{
	Zero = 0,
	One,
	SrcColor,
	OneMinusSrcColor,
	DstColor,
	OneMinusDstColor,
	SrcAlpha,
	OneMinusSrcAlpha,
	DstAlpha,
	OneMinusDstAlpha,
	ConstantColor,
	OneMinusConstantColor,
	ConstantAlpha,
	OneMinusConstantAlpha,
	SrcAlphaSaturate,
	Src1Color,
	OneMinusSrc1Color,
	Src1Alpha,
	OneMinusSrc1Alpha,
};

enum class BlendEquation : uint8_t
{
	FunctionAdd = 0,
	FunctionSubtract,
	FunctionReverseSubTrect,
	Min,
	Max,
};

enum class CullingMode : uint8_t
{
	Front = 0,
	Back,
	FrontAndBack,
	FrontNitherBack,
};

enum class FrontFace : uint8_t
{
	CCW = 0,
	CW,
};

enum class StencilOperation : uint8_t
{
	Keep = 0,
	Zero,
	Replace,
	Increase,
	IncreaseWrap,
	Decrease,
	DecreaseWrap,
	Invert,
};

#define SL_SAMPLER_NONE                   UINT32_C(0x00000000)
							              
#define SL_SAMPLER_U_CLAMP                UINT32_C(0x00000001)
#define SL_SAMPLER_U_BORDER               UINT32_C(0x00000002)
#define SL_SAMPLER_U_MIRROR               UINT32_C(0x00000003)
#define SL_SAMPLER_U_REPEAT               UINT32_C(0x00000004)
#define SL_SAMPLER_U_MASK                 UINT32_C(0x0000000f)
#define SL_SAMPLER_U_SHIFT                0
							              
#define SL_SAMPLER_V_CLAMP                UINT32_C(0x00000010)
#define SL_SAMPLER_V_BORDER               UINT32_C(0x00000020)
#define SL_SAMPLER_V_MIRROR               UINT32_C(0x00000030)
#define SL_SAMPLER_V_REPEAT               UINT32_C(0x00000040)
#define SL_SAMPLER_V_MASK                 UINT32_C(0x000000f0)
#define SL_SAMPLER_V_SHIFT                4
							              
#define SL_SAMPLER_W_CLAMP                UINT32_C(0x00000100)
#define SL_SAMPLER_W_BORDER               UINT32_C(0x00000200)
#define SL_SAMPLER_W_MIRROR               UINT32_C(0x00000300)
#define SL_SAMPLER_W_REPEAT               UINT32_C(0x00000400)
#define SL_SAMPLER_W_MASK                 UINT32_C(0x00000f00)
#define SL_SAMPLER_W_SHIFT                8
							              
#define SL_SAMPLER_MIN_NEAREST            UINT32_C(0x00001000)
#define SL_SAMPLER_MIN_LINEAR             UINT32_C(0x00002000)
#define SL_SAMPLER_MIN_MASK               UINT32_C(0x00003000)
#define SL_SAMPLER_MIN_SHIFT              12
							              
#define SL_SAMPLER_MAG_NEAREST            UINT32_C(0x00004000)
#define SL_SAMPLER_MAG_LINEAR             UINT32_C(0x00008000)
#define SL_SAMPLER_MAG_MASK               UINT32_C(0x0000c000)
#define SL_SAMPLER_MAG_SHIFT              14
										  
#define SL_SAMPLER_MIPMAP_NEAREST_NEAREST UINT32_C(0x00010000) // Nearest between pixels, nearest between mipmaps.
#define SL_SAMPLER_MIPMAP_NEAREST_LINEAR  UINT32_C(0x00030000) // Nearest between pixels, linear between mipmaps.
#define SL_SAMPLER_MIPMAP_LINEAR_NEAREST  UINT32_C(0x00020000) // Linear between pixels, nearest between mipmaps.
#define SL_SAMPLER_MIPMAP_LINEAR_LINEAR   UINT32_C(0x00040000) // Linear between pixels, linear between mipmaps.
#define SL_SAMPLER_MIPMAP_MASK            UINT32_C(0x000f0000)
#define SL_SAMPLER_MIPMAP_SHIFT           16

#define SL_SAMPLER_CLAMP (0 \
	| SL_SAMPLER_U_CLAMP \
	| SL_SAMPLER_V_CLAMP \
	| SL_SAMPLER_W_CLAMP \
	)

#define SL_SAMPLER_BORDER (0 \
	| SL_SAMPLER_U_BORDER \
	| SL_SAMPLER_V_BORDER \
	| SL_SAMPLER_W_BORDER \
	)

#define SL_SAMPLER_MIRROR (0 \
	| SL_SAMPLER_U_MIRROR \
	| SL_SAMPLER_V_MIRROR \
	| SL_SAMPLER_W_MIRROR \
	)

#define SL_SAMPLER_REPEAT (0 \
	| SL_SAMPLER_U_REPEAT \
	| SL_SAMPLER_V_REPEAT \
	| SL_SAMPLER_W_REPEAT \
	)

#define SL_SAMPLER_NEAREST (0 \
	| SL_SAMPLER_MIN_NEAREST \
	| SL_SAMPLER_MAG_NEAREST \
	| SL_SAMPLER_MIPMAP_NEAREST_NEAREST \
	)

#define SL_SAMPLER_BILINEAR (0 \
	| SL_SAMPLER_MIN_LINEAR \
	| SL_SAMPLER_MAG_LINEAR \
	| SL_SAMPLER_MIPMAP_LINEAR_NEAREST \
	)

#define SL_SAMPLER_TRILINEAR (0 \
	| SL_SAMPLER_MIN_LINEAR \
	| SL_SAMPLER_MAG_LINEAR \
	| SL_SAMPLER_MIPMAP_LINEAR_LINEAR \
	)

} // namespace sl
