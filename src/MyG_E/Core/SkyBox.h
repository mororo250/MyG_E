#pragma once

#include "Core/Model3D.h"

class SkyBox
{
public:
	SkyBox();
	~SkyBox();


private:
	std::vector<std::string> m_texture_files;
	std::unique_ptr<Model3D> m_model;
};