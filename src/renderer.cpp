#include "renderer.h"
#include <iostream>

void Renderer::Draw(VertexArray& va, const IndexBuffer& ib, const Shader& shader) const
{
	shader.Bind();
	va.Bind();
	ib.Bind();
	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, nullptr);
}

void Renderer::Clear() const
{
	glClearColor(0.125F, 0.125F, 0.125F, 1.0F); // Dark gray color
	glClear(GL_COLOR_BUFFER_BIT);
}