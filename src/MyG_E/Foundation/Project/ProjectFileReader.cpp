#include "ProjectFileReader.h"
#include "Core/ProjectController.h"

bool ProjectFileReader::read_file(std::string& const file_path)
{
	ProjectController* controller = new ProjectController();
	if (!is_json(get_filename(file_path)))
		return false;

	std::ifstream json_file(file_path);
	std::string json_string((std::istreambuf_iterator<char>(json_file)),
		(std::istreambuf_iterator<char>()));

	rapidjson::Document document;
	if (document.Parse(json_string.c_str()).HasParseError())
		return false;
	
	// Load camera
	if (document.HasMember("camera"))
		if (document["camera"].HasMember("model"))
		{
			// Load camera position
			Vector<float, 3> position{ document["camera"]["position"]["x"].GetFloat(), 
				document["camera"]["position"]["y"].GetFloat(),
				document["camera"]["position"]["z"].GetFloat() };
			// Load camera focus point
			Vector<float, 3> focus_point{ document["camera"]["focus_point"]["x"].GetFloat(),
				document["camera"]["focus_point"]["y"].GetFloat(),
				document["camera"]["focus_point"]["z"].GetFloat() };
			// Create new camera
			std::string camera_model(document["camera"]["model"].GetString());
			if (camera_model == "edit_camera")
			{
				controller->set_camera(new EditCamera(position, focus_point));
			}
			else if (document["camera"]["model"].GetString() == "fps_camera")
			{
				controller->set_camera(new FPSCamera(position, focus_point));
			}
		}

	// Load objects
	if (document.HasMember("object_buffer"))
	{
		auto& object_buffer = document["object_buffer"];
		for (unsigned int i = 0; i < object_buffer.Size(); i++)
		{
			// Load object position
			Vector<float, 3> position{ object_buffer[i]["position"]["x"].GetFloat(),
				object_buffer[i]["position"]["y"].GetFloat(),
				object_buffer[i]["position"]["z"].GetFloat() };

			// Load scale position
			Vector<float, 3> scale{ object_buffer[i]["scale"]["x"].GetFloat(),
				object_buffer[i]["scale"]["y"].GetFloat(),
				object_buffer[i]["scale"]["z"].GetFloat() };

			// Load object material
			auto& material_json = object_buffer[i]["material"];
			Vector<float, 3> ambient{ material_json["ambient"]["x"].GetFloat(),
				material_json["ambient"]["y"].GetFloat(),
				material_json["ambient"]["z"].GetFloat() };
			Vector<float, 3> diffuse{ material_json["diffuse"]["x"].GetFloat(),
				material_json["diffuse"]["y"].GetFloat(),
				material_json["diffuse"]["z"].GetFloat() };
			Vector<float, 3> specular{ material_json["specular"]["x"].GetFloat(),
				material_json["specular"]["y"].GetFloat(),
				material_json["specular"]["z"].GetFloat() };
			float shininess = material_json["shininess"].GetFloat();
			Material material(ambient, diffuse, specular, shininess);

			// Create Object
			std::string object_model = object_buffer[i]["model"].GetString();
			Model3D* object = nullptr;
			if (object_model == "plane")
				object = new Model3D(new Mesh(Shape::PLANE));
			else if (object_model == "cube")
				object = new Model3D(new Mesh(Shape::CUBE));
			else if (object_model == "pyramid")
				object = new Model3D(new Mesh(Shape::PYRAMID));
			else if (object_model == "sphere")
				object = new Model3D(new Mesh(Shape::SPHERE));
			object->SetPosition(position);
			object->SetScale(scale);
			object->SetMaterial(material);
			controller->push_object(object);
		}
	}

	// Load lights
	if (document.HasMember("light_buffer"))
	{
		auto& light_buffer = document["light_buffer"];
		for (unsigned int i = 0; i < light_buffer.Size(); i++)
		{
			// Load light position
			Vector<float, 3> position{ light_buffer[i]["position"]["x"].GetFloat(),
				light_buffer[i]["position"]["y"].GetFloat(),
				light_buffer[i]["position"]["z"].GetFloat() };

			// Create light
			std::string light_model = light_buffer[i]["model"].GetString();
			if (light_model == "directional_light")
				controller->push_light( new DirectionalLight(position));
			else if (light_model == "point_light")
				controller->push_light(new PointLight(position));
			else if (light_model == "spot_light")
				controller->push_light(new SpotLight(position));
		}
	}
	Game::Get().set_project(controller);
	return true;
}