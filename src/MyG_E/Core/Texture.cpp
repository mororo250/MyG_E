#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "Foundation/Gldebug.h"
#include "Foundation/UI/FileBrowser.h"
#include "Texture.h"

#include "stb_image.h"
#include "imgui.h"


Texture::Texture(std::string const& file_path)
	:m_texture(0),
	m_color(nullptr),
	m_file_path(file_path), 
	m_bpp(0), 
	m_width(0), 
	m_height(0),
	m_is_unitary(false)
{
	if_is_a_texture();
}

Texture::Texture(Vector<float, 3> const& color)
	:m_texture(0),
	m_color(new float[3]{ color[0], color[1], color[2] }),
	m_file_path(""),
	m_bpp(4),
	m_width(1),
	m_height(1),
	m_is_unitary(true)
{
	if_is_a_color();
}

Texture::Texture(Texture const& other)
{
	copy_other(other);
	if (m_is_unitary)
		if_is_a_color();
	else
		if_is_a_texture();
}

Texture& Texture::operator=(Texture const& other)
{
	if (this != &other)
	{
		GLcall(glDeleteTextures(1, &m_texture)); //Delete current texture before create a new one.
		copy_other(other);
		if (m_is_unitary)
			if_is_a_color();
		else
			if_is_a_texture();
	}
	return *this;
}

Texture::~Texture()
{
	GLcall(glDeleteTextures(1, &m_texture));
}

void Texture::bind(unsigned int slot) const
{
	GLcall(glActiveTexture(GL_TEXTURE0 + slot));
	GLcall(glBindTexture(GL_TEXTURE_2D, m_texture));
}

void Texture::unbind() const
{
	GLcall(glBindTexture(GL_TEXTURE_2D, 0));
}

void Texture::change_texture(std::string const& file_path)
{
	m_is_unitary = false;
	m_file_path = file_path;
	GLcall(glDeleteTextures(1, &m_texture)); //Delete current texture before create a new one.
	
	if (m_color)
		m_color.reset(nullptr);
	if_is_a_texture();
}

void Texture::change_texture(Vector<float, 3> const& color)
{
	m_is_unitary = true;
	m_file_path = "";
	GLcall(glDeleteTextures(1, &m_texture)); //Delete current texture before create a new one.
	
	m_color.reset(new float[3]);
	m_color[0] = color[0];
	m_color[1] = color[1];
	m_color[2] = color[2];
	if_is_a_color();
}

void Texture::imgui_renderer(std::string const& texture_name)
{
	if (ImGui::Button("import texture"))
	{
		change_texture(open_file_browser("(*.png) Project File\0*.png\0"));
	}
	ImGui::Text("Current texture: %s", m_file_path.c_str());

	if (m_is_unitary)
	{
		Vector<float, 3> color{ m_color[0], m_color[1], m_color[2] };
		ImGui::ColorEdit3("Texture_name", &color[0]);
		change_texture(color);
	}
	else
		if (ImGui::Button("remove texture"))
			change_texture(Vector<float, 3>{});
}

void Texture::copy_other(Texture const& other)
{
	m_file_path = other.m_file_path;

	m_width = other.m_width;
	m_height = other.m_height;
	m_bpp = other.m_bpp;
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

void Texture::if_is_a_color()
{
	unsigned char* local_buffer = new unsigned char[4]{ static_cast<unsigned char>(m_color[0] * 255.0f),
	static_cast<unsigned char>(m_color[1] * 255.0f), static_cast<unsigned char>(m_color[2] * 255.0f), 255 };
	m_width = 1;
	m_height = 1;

	create_texture(local_buffer);

	delete local_buffer;
}

void Texture::if_is_a_texture()
{
	stbi_set_flip_vertically_on_load(1); // Flip the image - opengl start from the top left intead of the bottom lefet
	unsigned char* local_buffer = stbi_load(m_file_path.c_str(), &m_width, &m_height, &m_bpp, 4);

	create_texture(local_buffer);

	if (local_buffer)
		stbi_image_free(local_buffer);
}

void Texture::create_texture(unsigned char const* local_buffer)
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