#pragma once

#include <cstdint>
#include <string>
#include <vector>

namespace sl
{

enum class AttribType : uint8_t
{
	Int8 = 0,
	Uint8,
	Int16,
	Uint16,
	Int32,
	Uint32,

	Half, // Need to be supported in CPU side
	Float,
	Double,

	Count,
};

struct VertexLayoutElement
{
	VertexLayoutElement(std::string name, AttribType type, uint32_t count, bool normalize = false);

	std::string m_name;
	AttribType m_type;
	uint32_t m_count;
	bool m_normalize;

	uint32_t m_size = 0;
	uint32_t m_offset = 0;
};

class VertexLayout
{
public:
	VertexLayout() = default;
	VertexLayout(std::initializer_list<VertexLayoutElement> elements);

	uint32_t GetStride() const { return m_stride; }
	size_t GetElementCount() const { return m_elements.size(); }

	auto begin() { return m_elements.begin(); }
	auto end() { return m_elements.end(); }
	auto begin() const { return m_elements.cbegin(); }
	auto end() const { return m_elements.end(); }

private:
	std::vector<VertexLayoutElement> m_elements;
	uint32_t m_stride = 0;
};

} // namespace sl
