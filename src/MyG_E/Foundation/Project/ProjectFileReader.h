#pragma once

#include <string>
#include <fstream>   // rapidjson's DOM-style API

class ProjectFileReader
{
public:
	ProjectFileReader() = default;
	~ProjectFileReader() = default;

	bool read_file(std::string const&  file_path);

private:
	inline std::string get_filename(std::string path) const{ return path.substr(path.find_last_of("/\\") + 1); }
	inline bool is_json(std::string file_name) const{ return file_name.substr(file_name.find_last_of(".")) == ".json"; }
};
