#include "ProjectFileWriter.h"

#include <fstream>

#include "rapidjson/stringbuffer.h"
#include "rapidjson/writer.h"

bool ProjectFileWriter::write_file(ProjectController const* project_controller, std::string const& filename )
{
	rapidjson::StringBuffer string_buffer;
	rapidjson::Writer<rapidjson::StringBuffer> writer(string_buffer);
	
	// must pass an allocator when the object may need to allocate memory

	// Add camera
	{
		Camera camera = project_controller->get_camera();
		
		// Camera
		writer.StartObject();
		writer.Key("camera");

		writer.StartObject();
		writer.Key("model");
		writer.String("edit_camera");
		writer.EndObject();

		// Camera Position
		writer.StartObject();
		writer.Key("position");

		writer.StartObject();
		writer.Key("x");
		writer.Double(camera.GetPosition()[0]);
		writer.EndObject();
		writer.StartObject();
		writer.Key("y");
		writer.Double(camera.GetPosition()[1]);
		writer.EndObject();
		writer.StartObject();
		writer.Key("z");
		writer.Double(camera.GetPosition()[2]);
		writer.EndObject();
		writer.EndObject(); // End Camera Position

		// Camera Direction
		writer.StartObject();
		writer.Key("direction");

		writer.StartObject();
		writer.Key("x");
		writer.Double(camera.GetDirection()[0]);
		writer.EndObject();
		writer.StartObject();
		writer.Key("y");
		writer.Double(camera.GetDirection()[1]);
		writer.EndObject();
		writer.StartObject();
		writer.Key("z");
		writer.Double(camera.GetDirection()[2]);
		writer.EndObject();
		writer.EndObject(); // End Camera Direction
		writer.EndObject(); // End Camera
	}

	// Write file
	std::ofstream file;
	file.open(filename);
	file << string_buffer.GetString();
	file.close();

	return true;
}
