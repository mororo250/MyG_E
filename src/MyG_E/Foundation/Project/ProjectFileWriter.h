#pragma once

#include "Core/ProjectController.h"
#include "rapidjson/stringbuffer.h"
#include "rapidjson/prettywriter.h"

class ProjectFileWriter
{
public:
	ProjectFileWriter() = default;
	~ProjectFileWriter() = default;

	bool write_file(ProjectController const* project_controller, std::string const& name);

private:
	void write_camera(rapidjson::PrettyWriter<rapidjson::StringBuffer>& writer, 
		ProjectController const* project_controller);
	void write_objects(rapidjson::PrettyWriter<rapidjson::StringBuffer>& writer, 
		ProjectController const* project_controller);
	void write_lights(rapidjson::PrettyWriter<rapidjson::StringBuffer>& writer, 
		ProjectController const* project_controller);
};