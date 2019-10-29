#include "Material.h"
#include "Core/Texture/Texture2D.h"

#include "imgui.h"

Material::Material(Texture2D* diff, Texture2D* spec, Texture2D* norm_map, float shi)
	: m_diffuse(diff)
	, m_specular(spec)
	, m_normal_map(norm_map)
	, m_shininess(shi)
	, m_use_normal_map(false)
{}

Material::Material(Material const& other)
{
	copy_other(other);
}

Material& Material::operator=(Material const& other)
{
	copy_other(other);
	return *this;
}

void Material::imgui_renderer()
{
	if (ImGui::CollapsingHeader("Material", nullptr, ImGuiTreeNodeFlags_DefaultOpen))
	{
		ImGui::Indent(10.0f);
		if (ImGui::CollapsingHeader("Diffuse", nullptr, ImGuiTreeNodeFlags_DefaultOpen))
			m_diffuse->imgui_renderer("diffuse");
		if (ImGui::CollapsingHeader("Specular Map", nullptr, ImGuiTreeNodeFlags_DefaultOpen))
			m_specular->imgui_renderer("specular map");
		if (ImGui::CollapsingHeader("Normal Map", nullptr, ImGuiTreeNodeFlags_DefaultOpen))
		{
			m_normal_map->imgui_renderer("normal map");
			ImGui::Checkbox("Use", &m_use_normal_map);
		}
		ImGui::DragFloat("shininess", &m_shininess, 0.05f, 0.0f, 100.0f);
		ImGui::Unindent(10.0f);
	}
}

void Material::copy_other(Material const& other)
{
	m_diffuse.reset(new Texture2D(*other.m_diffuse));
	m_specular.reset(new Texture2D(*other.m_specular));
	m_normal_map.reset(new Texture2D(*other.m_normal_map));
	m_shininess = other.m_shininess;
	m_use_normal_map = other.m_use_normal_map;
}
