#include "Texture2D.h"

#include <glad/glad.h>

#include "Foundation/Gldebug.h"
#include "Foundation/UI/FileBrowser.h"

#include "stb_image.h"
#include "imgui.h"


Texture2D::Texture2D(std::string const& file_path)
	:m_texture(0),
	m_color(nullptr),
	m_file_path(file_path), 
	m_nrChannels(0),
	m_width(0), 
	m_height(0),
	m_is_unitary(false)
{
	if_is_a_texture();
}

Texture2D::Texture2D(Vector<float, 3> const& color)
	:m_texture(0),
	m_color(new float[3]{ color[0], color[1], color[2] }),
	m_file_path(""),
	m_nrChannels(4),
	m_width(1),
	m_height(1),
	m_is_unitary(true)
{
	if_is_a_color();
}

Texture2D::Texture2D(Texture2D const& other)
{
	copy_other(other);
	if (m_is_unitary)
		if_is_a_color();
	else
		if_is_a_texture();
}

Texture2D& Texture2D::operator=(Texture2D const& other)
{
	if (this != &other)
	{
		GLcall(glDeleteTextures(1, &m_texture)); //Delete current Texture2D before create a new one.
		copy_other(other);
		if (m_is_unitary)
			if_is_a_color();
		else
			if_is_a_texture();
	}
	return *this;
}

Texture2D::~Texture2D()
{
	GLcall(glDeleteTextures(1, &m_texture));
}

void Texture2D::bind(unsigned int slot) const
{
	GLcall(glActiveTexture(GL_TEXTURE0 + slot));
	GLcall(glBindTexture(GL_TEXTURE_2D, m_texture));
}

void Texture2D::unbind() const
{
	GLcall(glBindTexture(GL_TEXTURE_2D, 0));
}

void Texture2D::change_texture(std::string const& file_path)
{
	m_is_unitary = false;
	m_file_path = file_path;
	GLcall(glDeleteTextures(1, &m_texture)); //Delete current Texture2D before create a new one.
	
	if (m_color)
		m_color.reset(nullptr);
	if_is_a_texture();
}

void Texture2D::change_texture(Vector<float, 3> const& color)
{
	m_is_unitary = true;
	m_file_path = "";
	GLcall(glDeleteTextures(1, &m_texture)); //Delete current Texture2D before create a new one.
	
	m_color.reset(new float[3]);
	m_color[0] = color[0];
	m_color[1] = color[1];
	m_color[2] = color[2];
	if_is_a_color();
}

void Texture2D::imgui_renderer(std::string const& name)
{ 
	ImGui::Image(reinterpret_cast<ImTextureID>(m_texture), ImVec2(32, 32), ImVec2(0, 0), ImVec2(1, 1));
	ImGui::SameLine();
	std::string button_name = "import " + name;
	if (ImGui::Button(button_name.c_str()))
		change_texture(open_file_browser("Image Files (.png, .jpg) \0*.png; *.jpg\0"));
	ImGui::Text("Current Texture2D: %s", m_file_path.c_str());

	if (m_is_unitary)
	{
		Vector<float, 3> color{ m_color[0], m_color[1], m_color[2] };
		ImGui::ColorEdit3(name.c_str(), &color[0]);
		change_texture(color);
	}
	else
	{
		button_name = "remove " + name;
		if (ImGui::Button(button_name.c_str()))
			change_texture(Vector<float, 3>{});
	}
}

void Texture2D::copy_other(Texture2D const& other)
{
	m_file_path = other.m_file_path;

	m_width = other.m_width;
	m_height = other.m_height;
	m_nrChannels = other.m_nrChannels;
	m_is_unitary = other.m_is_unitary;

	m_color.reset(nullptr);
	if (m_is_unitary)
	{
		m_color.reset(new float[3]);
		m_color[0] = other.m_color[0];
		m_color[1] = other.m_color[1];
		m_color[2] = other.m_color[2];
	}
}

void Texture2D::if_is_a_color()
{
	unsigned char* local_buffer = new unsigned char[4]{ static_cast<unsigned char>(m_color[0] * 255.0f),
	static_cast<unsigned char>(m_color[1] * 255.0f), static_cast<unsigned char>(m_color[2] * 255.0f), 255 };
	m_width = 1;
	m_height = 1;

	create_texture(local_buffer);

	delete local_buffer;
}

void Texture2D::if_is_a_texture()
{
	stbi_set_flip_vertically_on_load(1); // Flip the image - opengl start from the top left intead of the bottom lefet
	unsigned char* local_buffer = stbi_load(m_file_path.c_str(), &m_width, &m_height, &m_nrChannels, 4);

	create_texture(local_buffer);

	if (local_buffer)
		stbi_image_free(local_buffer);
}

void Texture2D::create_texture(unsigned char const* local_buffer)
{
	GLcall(glGenTextures(1, &m_texture));
	GLcall(glBindTexture(GL_TEXTURE_2D, m_texture)); // bind
	GLcall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR));
	GLcall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR));
	GLcall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE));
	GLcall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE));

	GLcall(glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, m_width, m_height, 0, GL_RGBA, GL_UNSIGNED_BYTE, local_buffer));
	GLcall(glBindTexture(GL_TEXTURE_2D, 0)); // unbind
}