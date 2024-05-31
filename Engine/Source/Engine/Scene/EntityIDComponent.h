#pragma once

namespace sl
{

class Shader;

struct EntityIDComponent final
{
	void Reset()
	{

	}

	Shader *m_pShader = nullptr;
};

} // namespace sl
