#pragma once
#include <../inc/glad/glad.h>
#include "VertexArray.h"
#include "IndexBuffer.h"
#include "Shader.h"

class Renderer
{
  public:
	void Clear() const;
	void Draw(VertexArray& va, const IndexBuffer& ib, const Shader& shader) const;
};