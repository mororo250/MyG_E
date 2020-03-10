#include "TextureCubMap.h"

#include <glad/glad.h>

#include "Foundation/Gldebug.h"
#include "Foundation/UI/FileBrowser.h"

#include "stb_image.h"
#include "imgui.h"


TextureCubMap::TextureCubMap(std::array<std::string, 6> const& texture_files)
	: m_texture(0)
	, m_texture_files(texture_files)
{
	create_texture();
}

TextureCubMap::TextureCubMap(std::string const& folder)
{
	get_texture_files_from_folder(folder);
	create_texture();
}

TextureCubMap::~TextureCubMap()
{
	GLcall(glDeleteTextures(1, &m_texture));
}

void TextureCubMap::bind(unsigned int slot) const
{
	GLcall(glActiveTexture(GL_TEXTURE0 + slot));
	GLcall(glBindTexture(GL_TEXTURE_CUBE_MAP, m_texture));
}

void TextureCubMap::unbind() const
{
	GLcall(glBindTexture(GL_TEXTURE_CUBE_MAP, 0));
}

void TextureCubMap::imgui_renderer(std::string const& texture_name)
{
	if(ImGui::Button(texture_name.c_str()))
		change_texture(open_folder_browser());
}

void TextureCubMap::change_texture(std::string const& folder_name)
{
	GLcall(glDeleteTextures(1, &m_texture));
	get_texture_files_from_folder(folder_name);
	create_texture();
}

void TextureCubMap::change_texture(std::array<std::string, 6> const& texture_files)
{
	m_texture_files = texture_files;
	GLcall(glDeleteTextures(1, &m_texture));
	create_texture();
}

void TextureCubMap::get_texture_files_from_folder(std::string const& folder_name)
{
	m_texture_files = {
	folder_name + "\\right.jpg",
	folder_name + "\\left.jpg",
	folder_name + "\\top.jpg",
	folder_name + "\\bottom.jpg",
	folder_name + "\\front.jpg",
	folder_name + "\\back.jpg" };
}

void TextureCubMap::create_texture()
{
	GLcall(glGenTextures(1, &m_texture));
	GLcall(glBindTexture(GL_TEXTURE_CUBE_MAP, m_texture));

	int width, height, nrChannels;
	for (unsigned int i = 0; i < m_texture_files.size(); i++)
	{
		unsigned char* data = stbi_load(m_texture_files[i].c_str(), &width, &height, &nrChannels, 0);
		if (data)
		{
			GLcall(glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data));
			stbi_image_free(data);
		}
		else
		{
			std::cout << "Cubemap texture failed to load at path: " << m_texture_files[i] << "\n\n";
			stbi_image_free(data);
		}
	}
	GLcall(glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR));
	GLcall(glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR));
	GLcall(glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE));
	GLcall(glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE));
	GLcall(glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE));
}
