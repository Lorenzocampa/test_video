#pragma once

#include "VertexBuffer.h"

class VertexBufferLayout;

class VertexArray
{
  private:
	unsigned int m_rendererID;

  public:
	VertexArray();
	~VertexArray();

	void AddBuffer(const VertexBuffer& vb, const VertexBufferLayout& lyout);

	void Bind() const;
	void Unbind() const;

};
