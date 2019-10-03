#include "Material.h"
#include "Core/Texture/Texture2D.h"

Material::Material(Texture2D* diff, Texture2D* spec, float shi)
	:m_diffuse(diff),
	m_specular(spec),
	m_shininess(shi)
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

void Material::copy_other(Material const& other)
{
	m_diffuse.reset(new Texture2D(*other.m_diffuse));
	m_specular.reset(new Texture2D(*other.m_specular));
	m_shininess = other.m_shininess;
}
