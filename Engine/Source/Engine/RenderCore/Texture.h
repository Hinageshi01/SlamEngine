#pragma once

#include "Core/Defines.h"

#include <cstdint>
#include <string>

namespace sl
{

class Texture2D
{
public:
	static Texture2D *Create(uint32_t width, uint32_t height, bool mipmap, TextureFormat format, uint32_t flags = 0, const void *pData = nullptr);

public:
	virtual ~Texture2D() = default;

	virtual uint32_t GetHandle() const = 0;
	
	virtual uint32_t GetHeight() const = 0;
	virtual uint32_t GetWidth() const = 0;
	virtual TextureFormat GetFormat() const = 0;

	virtual void Clear(const void *pClearData) const = 0;
	virtual void Resize(uint32_t width, uint32_t height, const void *pData = nullptr) = 0;

	virtual void Bind(uint32_t slot) const = 0;
};

class Texture3D
{

};

class TextureCube
{

};

} // namespace sl
