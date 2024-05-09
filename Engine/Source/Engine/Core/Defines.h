#pragma once

#include <cstdint>

namespace sl
{

enum class CameraControllerMode : uint8_t
{
	None,
	FPS,
	Editor,
};

enum class GraphicsBackend : uint8_t
{
	None = 0,
	OpenGL,
	DirectX11,
	DirectX12,
	Vulkan,
	Metal,
};

#define SL_CLEAR_NONE    UINT8_C(0x00)
#define SL_CLEAR_COLOR   UINT8_C(0x01)
#define SL_CLEAR_DEPTH   UINT8_C(0x02)
#define SL_CLEAR_STENCIL UINT8_C(0x04)

} // namespace sl
