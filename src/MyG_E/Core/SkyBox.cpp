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

SkyBox::SkyBox()
{
	constexpr int NumAttrib = 3;
	std::array<std::string, 6> skymap = { 
		"C://Users//joao_//OneDrive//Desktop//git//MyG_E//src//MyG_E//Resources//skyboxs//lake//right.jpg",
		"C://Users//joao_//OneDrive//Desktop//git//MyG_E//src//MyG_E//Resources//skyboxs//lake//left.jpg",
		"C://Users//joao_//OneDrive//Desktop//git//MyG_E//src//MyG_E//Resources//skyboxs//lake//top.jpg",
		"C://Users//joao_//OneDrive//Desktop//git//MyG_E//src//MyG_E//Resources//skyboxs//lake//bottom.jpg",
		"C://Users//joao_//OneDrive//Desktop//git//MyG_E//src//MyG_E//Resources//skyboxs//lake//front.jpg",
		"C://Users//joao_//OneDrive//Desktop//git//MyG_E//src//MyG_E//Resources//skyboxs//lake//back.jpg"
	};
	m_skybox_texture.reset(new TextureCubMap(skymap));
	m_vao = std::make_unique<VertexArray>();
	m_vb = std::make_unique<VertexBuffer>(reinterpret_cast<float*>(m_skybox_vertices.data()), m_skybox_vertices.size() / NumAttrib, NumAttrib);
	
	m_vao->PushLayout(3, GL_FLOAT, GL_FALSE, 0);
	m_vao->AddBuffer(*m_vb);
	
	m_vao->unbind();
	m_vb->unbind();
}

void SkyBox::imgui_rederer()
{
	m_skybox_texture->imgui_renderer("skybox");
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
