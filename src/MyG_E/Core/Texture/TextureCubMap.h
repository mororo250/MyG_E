#pragma once

#include "Core/Texture/Texture.h"

class TextureCubMap : public Texture
{
public:
	TextureCubMap(std::array<std::string, 6> const& textures_files);
	TextureCubMap(std::string const& folder);
	~TextureCubMap();

	void bind(unsigned int slot = 0) const override;
	void unbind() const override;

	void imgui_renderer(std::string const& texture_name) override;

	void change_texture(std::array<std::string, 6> const& texture_files);
	void change_texture(std::string const& folder_name);

	inline std::array<std::string, 6> get_texture_files() const { return m_texture_files; }

	inline std::string get_folder() const { return std::filesystem::path(m_texture_files[0]).parent_path().string(); }

private:
	void get_texture_files_from_folder(std::string const& folder);
	void create_texture();

	std::array<std::string, 6> m_texture_files;
	unsigned int m_texture;
};