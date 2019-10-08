#include "SkyBox.h"

#include "glad/glad.h"
#include "imgui.h"

std::array<float, 108> m_skybox_vertices = {
	// positions          
	-1.0f,  1.0f, -1.0f,
	-1.0f, -1.0f, -1.0f,
	 1.0f, -1.0f, -1.0f,
	 1.0f, -1.0f, -1.0f,
	 1.0f,  1.0f, -1.0f,
	-1.0f,  1.0f, -1.0f,

	-1.0f, -1.0f,  1.0f,
	-1.0f, -1.0f, -1.0f,
	-1.0f,  1.0f, -1.0f,
	-1.0f,  1.0f, -1.0f,
	-1.0f,  1.0f,  1.0f,
	-1.0f, -1.0f,  1.0f,

	 1.0f, -1.0f, -1.0f,
	 1.0f, -1.0f,  1.0f,
	 1.0f,  1.0f,  1.0f,
	 1.0f,  1.0f,  1.0f,
	 1.0f,  1.0f, -1.0f,
	 1.0f, -1.0f, -1.0f,

	-1.0f, -1.0f,  1.0f,
	-1.0f,  1.0f,  1.0f,
	 1.0f,  1.0f,  1.0f,
	 1.0f,  1.0f,  1.0f,
	 1.0f, -1.0f,  1.0f,
	-1.0f, -1.0f,  1.0f,

	-1.0f,  1.0f, -1.0f,
	 1.0f,  1.0f, -1.0f,
	 1.0f,  1.0f,  1.0f,
	 1.0f,  1.0f,  1.0f,
	-1.0f,  1.0f,  1.0f,
	-1.0f,  1.0f, -1.0f,

	-1.0f, -1.0f, -1.0f,
	-1.0f, -1.0f,  1.0f,
	 1.0f, -1.0f, -1.0f,
	 1.0f, -1.0f, -1.0f,
	-1.0f, -1.0f,  1.0f,
	 1.0f, -1.0f,  1.0f
};

SkyBox::SkyBox(TextureCubMap* texture)
	:m_skybox_texture(texture)
	,m_vao(new VertexArray())
	,m_vb(new VertexBuffer(reinterpret_cast<float*>(m_skybox_vertices.data()), static_cast<unsigned int>(m_skybox_vertices.size()) / 3, 3))
{
	m_vao->PushLayout(3, GL_FLOAT, GL_FALSE, 0);
	m_vao->AddBuffer(*m_vb);
	
	m_vao->unbind();
	m_vb->unbind();
}

void SkyBox::imgui_rederer()
{
	m_skybox_texture->imgui_renderer("Change skybox");
}

void SkyBox::begin()
{
	glDepthFunc(GL_LEQUAL);
}

void SkyBox::end()
{
	glDepthFunc(GL_LESS);
}

void SkyBox::draw()
{
	m_vao->bind();
	m_vb->bind();
	m_skybox_texture->bind(0);
	glDrawArrays(GL_TRIANGLES, 0, 36);
	m_skybox_texture->unbind();
	m_vao->unbind();
	m_vb->unbind();
}
