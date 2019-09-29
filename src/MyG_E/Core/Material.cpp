#include "Material.h"
#include "Core/Texture.h"

Material::Material(Texture* diff, Texture* spec, float shi)
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
	m_diffuse.reset(new Texture(*other.m_diffuse));
	m_specular.reset(new Texture(*other.m_specular));
	m_shininess = other.m_shininess;
}
