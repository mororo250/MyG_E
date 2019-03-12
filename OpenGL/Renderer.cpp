#include "Renderer.h"



void Renderer::Draw(const VertexArray& va, const IndexBuffer& ib, const Shader& shader) const
{
	va.bind();
	ib.bind();
	shader.bind();
	GLcall(glDrawElements(GL_TRIANGLES, ib.GetNumIndex(), GL_UNSIGNED_INT, nullptr));
	//if I didn't use indices shuld be: glDrawArrays(GL_TRIANGLES, 0, 6);
}

void Renderer::Clear() const
{
	GLcall(glClear(GL_COLOR_BUFFER_BIT));
}
