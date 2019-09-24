#pragma once

#include "Core/ProjectController.h"

#include <string>

class ProjectFileWriter
{
public:
	ProjectFileWriter() = default;
	~ProjectFileWriter() = default;

	bool write_file(ProjectController const* project_controller, std::string const& name);
};