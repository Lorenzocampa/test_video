#include "VertexBufferLayout.h"

template <> void VertexBufferLayout::push<float>(unsigned int count)
{
	m_elements.push_back({GL_FLOAT, count, GL_FALSE});
	m_stride += VertexBufferElement::get_size_type(GL_FLOAT) * count;
}

template <> void VertexBufferLayout::push<unsigned int>(unsigned int count)
{
	m_elements.push_back({GL_UNSIGNED_INT, count, GL_FALSE});
	m_stride += VertexBufferElement::get_size_type(GL_UNSIGNED_INT) * count;
}

template <> void VertexBufferLayout::push<unsigned char>(unsigned int count)
{
	m_elements.push_back({GL_UNSIGNED_BYTE, count, GL_TRUE});
	m_stride += VertexBufferElement::get_size_type(GL_UNSIGNED_BYTE) * count;
}