#include "Renderer2D.h"

void Renderer2D::draw_element(const IndexBuffer& ib) const
{
	GLcall(glDrawElements(GL_TRIANGLES, ib.GetNumIndex(), GL_UNSIGNED_INT, nullptr));
	//if I didn't use indices shuld be: glDrawArrays(GL_TRIANGLES, 0, 6);
}

void Renderer2D::clear() const
{
	GLcall(glClear(GL_COLOR_BUFFER_BIT));
}