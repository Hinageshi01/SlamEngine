#pragma once

#include <cstdint>
#include <string>

namespace sl
{

class Texture2D
{
public:
	static Texture2D *Create(std::string path, bool mipmap, uint32_t flags = 0);

public:
	virtual ~Texture2D() = default;

	virtual uint32_t GetHeight() const = 0;
	virtual uint32_t GetWidth() const = 0;

	virtual void Bind(uint32_t slot) const = 0;
};

class Texture3D
{

};

class TextureCube
{

};

} // namespace sl
