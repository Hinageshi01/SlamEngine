#pragma once

#include "Resource/Resource.h"

#include <memory>
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

	void SetRowData(std::vector<std::byte> data) { m_rowData = std::move(data); }

	Texture2D *GetTexture() const { return m_pTexture.get(); }

private:
	void DestroyCPUData();

	std::string m_assetPath;
	std::vector<std::byte> m_rowData;
	
	uint32_t m_flags = 0;
	uint32_t m_width = 0;
	uint32_t m_height = 0;
	uint8_t m_channels = 0;
	bool m_isHDR = false;

	std::unique_ptr<Texture2D> m_pTexture;
};

} // namespace sl
