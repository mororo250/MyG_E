#include "Renderer3D.h"
#include "Renderer2D.h"

Renderer3D::Renderer3D()
{
	GLcall(glEnable(GL_DEPTH_TEST));
	GLcall(glEnable(GL_MULTISAMPLE));
}

Renderer3D::~Renderer3D()
{
	GLcall(glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT));
	GLcall(glDisable(GL_DEPTH_TEST));
	GLcall(glDisable(GL_MULTISAMPLE))
}

void Renderer3D::Draw(const IndexBuffer & ib) const
{
	GLcall(glDrawElements(GL_TRIANGLES, ib.GetNumIndex(), GL_UNSIGNED_INT, nullptr));
}

void Renderer3D::Clear() const
{
	GLcall(glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT));
}
