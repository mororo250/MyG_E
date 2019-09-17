#include "ProjectFileReader.h"

bool ProjectFileReader::read_file(std::string& const file_path)
{
	if (!is_json(get_filename(file_path)))
		return false;

	std::ifstream json_file(file_path);
	std::string json_string((std::istreambuf_iterator<char>(json_file)),
		(std::istreambuf_iterator<char>()));

	rapidjson::Document document;
	if (document.Parse(json_string.c_str()).HasParseError())
		return false;
	
	return true;
}