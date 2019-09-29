#pragma once

#include "rapidjson/document.h"  

class ProjectController;

class ProjectFileReader
{
public:
	ProjectFileReader() = default;
	~ProjectFileReader() = default;

	bool read_file(std::string const&  file_path);

private:
	inline std::string get_filename(std::string const& path) const{ return path.substr(path.find_last_of("/\\") + 1); }
	inline bool is_json(std::string& file_name) const{ return file_name.substr(file_name.find_last_of(".")) == ".json"; }

	bool load_camera(rapidjson::Document const& document, ProjectController* controller);
	bool load_lights(rapidjson::Document const& document, ProjectController* controller);
	bool load_objects(rapidjson::Document const& document, ProjectController* controller);
	void load_light_strength(class Light* light, rapidjson::Value const& json);
};
