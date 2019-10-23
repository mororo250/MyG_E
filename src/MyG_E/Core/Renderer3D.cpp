#include "Renderer3D.h"
#include "Renderer2D.h"

// There is a problem here what if a create a new renderer and after delete the last one.
Renderer3D::Renderer3D()
{
}

Renderer3D::~Renderer3D()
{
	set_disactive();
	clear();
}

void Renderer3D::set_active()
{
	m_is_active = true;
	GLcall(glEnable(GL_DEPTH_TEST));
	GLcall(glEnable(GL_MULTISAMPLE));
}

void Renderer3D::set_disactive()
{
	m_is_active = false;
	GLcall(glDisable(GL_DEPTH_TEST));
	GLcall(glDisable(GL_MULTISAMPLE))
}

void Renderer3D::draw_element(const IndexBuffer & ib) const
{
	GLcall(glDrawElements(GL_TRIANGLES, ib.GetNumIndex(), GL_UNSIGNED_INT, nullptr));
}

void Renderer3D::clear() const
{
	GLcall(glClearColor(0.1f, 0.1f, 0.1f, 1.0f));
	GLcall(glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT));
}
