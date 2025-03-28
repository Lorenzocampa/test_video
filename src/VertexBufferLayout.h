#pragma once

#include <vector>
#include <../inc/glad/glad.h>

struct VertexBufferElement
{
	unsigned int type;
	unsigned int count;
	unsigned char normalized;

	static unsigned int get_size_type(unsigned int type)
	{
		switch (type)
		{
		case GL_FLOAT:
			return 4;
		case GL_UNSIGNED_INT:
			return 4;
		case GL_UNSIGNED_BYTE:
			return 1;
		}
	}
};

class VertexBufferLayout
{
  public:
	template <typename T> void push(unsigned int count); // Declaration only

	const std::vector<VertexBufferElement>& GetElements() const { return m_elements; }
	unsigned int GetStride() const { return m_stride; }

  private:
	std::vector<VertexBufferElement> m_elements;
	unsigned int m_stride = 0;
};