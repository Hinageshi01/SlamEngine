#pragma once

#include "Core/Defines.h"
#include "Resource/Resource.h"

#include <string>
#include <vector>

namespace sl
{

class Texture2D;

class TextureResource : public Resource
{
public:
	TextureResource(std::string_view path, uint32_t flags = 0);
	virtual ~TextureResource() override;

	virtual void OnImport() override;
	virtual void OnBuild() override;
	virtual void OnLoad() override;
	virtual void OnUpload() override;
	virtual void OnReady() override;
	virtual void OnDestroy() override;

	Texture2D *GetTexture() const { return m_pTexture; }

private:
	std::string m_assetPath;
	std::vector<std::byte> m_rowData;
	
	uint32_t m_flags = 0;
	uint32_t m_width = 0;
	uint32_t m_height = 0;
	uint8_t m_channels = 0;
	bool m_isHDR = false;

	Texture2D *m_pTexture = nullptr;
};

} // namespace sl
