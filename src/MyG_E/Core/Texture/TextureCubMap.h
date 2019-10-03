#pragma once

#include "Core/Texture/Texture.h"

class TextureCubMap : public Texture
{
public:
	TextureCubMap(std::array<std::string, 6> const& textures_files);
	~TextureCubMap();

	void bind(unsigned int slot = 0) const override;
	void unbind() const override;

	void imgui_renderer(std::string const& texture_name) override;
	
	void change_texture(std::string const& folder_name);
	void change_texture(std::array<std::string, 6> const& texture_files);

private:
	void create_texture(std::array<std::string, 6> const& files_names);

	std::array<std::string, 6> m_texture_files;
	unsigned int m_texture;
};