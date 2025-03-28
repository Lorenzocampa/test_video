#include "VertexArray.h"
#include "VertexBufferLayout.h"

VertexArray::VertexArray()
{
	glGenVertexArrays(1, &m_rendererID);
}

VertexArray::~VertexArray()
{
	glDeleteVertexArrays(1, &m_rendererID);
}

void VertexArray::AddBuffer(const VertexBuffer& vb, const VertexBufferLayout& layout)
{
	Bind();
	vb.Bind();
	const auto& elements = layout.GetElements();

	unsigned int offset = 0;
	for (unsigned int idx_for_i = 0; idx_for_i < elements.size(); ++idx_for_i)
	{
		const auto& element = elements[idx_for_i];
		glEnableVertexAttribArray(idx_for_i);
		glVertexAttribPointer(idx_for_i, element.count, element.type, element.normalized, layout.GetStride(), (const void*)offset);

		offset += element.count * VertexBufferElement::get_size_type(element.type);
	}
}

void VertexArray::Bind() const
{
	glBindVertexArray(m_rendererID);
}

void VertexArray::Unbind() const
{
	glBindVertexArray(0);
}