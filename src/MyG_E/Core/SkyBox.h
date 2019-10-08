#pragma once

#include "Core/Mesh.h"
#include "Core/Texture/TextureCubMap.h"
#include "Core/Shader.h"

class SkyBox
{
public:
	SkyBox(TextureCubMap* texture);
	~SkyBox() = default;

	void imgui_rederer();

	void begin();
	void end();
	void draw();

	inline TextureCubMap const* get_texture() const { return m_skybox_texture.get(); }
private:
	std::unique_ptr<TextureCubMap> m_skybox_texture;
	std::unique_ptr<VertexArray> m_vao;
	std::unique_ptr<VertexBuffer> m_vb;
};