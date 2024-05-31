#pragma once

#include "Core/Defines.h"

namespace sl
{

class Texture2D;

struct Attachment
{
	Texture2D *m_pTexture;
	uint32_t m_point;
};

} // namespace sl
