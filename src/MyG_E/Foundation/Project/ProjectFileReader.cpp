#include "ProjectFileReader.h"

#include "Core/ProjectController.h"

bool ProjectFileReader::read_file(std::string const& file_path)
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
	if (!load_camera(document, controller))
		return false; // It will return false until the user be able to create a camera inside the project;

	// Load objects
	load_objects(document, controller);

	// Load lights
	load_lights(document, controller);

	Game::Get().set_project(controller);
	return true;
}

bool ProjectFileReader::load_camera(rapidjson::Document const& document, ProjectController* controller)
{
	// Load camera
	if (!document.HasMember("camera"))
	{
		std::cout << "There isn't a camera in the file" << std::endl;
		return false;
	}

	if (!document["camera"].HasMember("model"))
	{
		std::cout << "There isn't a type for the camera in the file" << std::endl;
		return false;
	}

	if (!document["camera"].HasMember("position"))
	{
		std::cout << "There isn't the position of the camera in the file" << std::endl;
		return false;
	}
	if (!document["camera"].HasMember("direction"))
	{
		std::cout << "There isn't the direction of the camera in the file" << std::endl;
		return false;
	}
	// Create new camera
	std::string camera_model(document["camera"]["model"].GetString());

	// Load camera position
	Vector<float, 3> position{ document["camera"]["position"]["x"].GetFloat(),
		document["camera"]["position"]["y"].GetFloat(),
		document["camera"]["position"]["z"].GetFloat() };
	
	// Load camera direction
	Vector<float, 3> direction{ document["camera"]["direction"]["x"].GetFloat(),
		document["camera"]["direction"]["y"].GetFloat(),
		document["camera"]["direction"]["z"].GetFloat() };
	
	// Create new camera
	if (camera_model == "edit_camera")
	{
		controller->set_camera(new EditCamera(position, direction));
	}
	else if (document["camera"]["model"].GetString() == "fps_camera")
	{
		controller->set_camera(new FPSCamera(position, direction));
	}
	return true;

}

bool ProjectFileReader::load_lights(rapidjson::Document const& document, ProjectController* controller)
{
	if (!document.HasMember("light_buffer"))
	{
		std::cout << "Trere isn't any light in the json file" << std::endl;
		return false;
	}
	
	auto& light_buffer = document["light_buffer"];
	for (unsigned int i = 0; i < light_buffer.Size(); i++)
	{
		// Load light position
		Vector<float, 3> position;
		if (light_buffer[i].HasMember("light_buffer"))
			position = { light_buffer[i]["position"]["x"].GetFloat(),
				light_buffer[i]["position"]["y"].GetFloat(),
				light_buffer[i]["position"]["z"].GetFloat() };

		// Load Light Color
		Vector<float, 3> color{ 1.0, 1.0, 1.0 };
		if (light_buffer[i].HasMember("light_color"))
			color = { light_buffer[i]["color"]["x"].GetFloat(),
				light_buffer[i]["color"]["y"].GetFloat(),
				light_buffer[i]["color"]["z"].GetFloat() };

		// Create light
		std::string light_model = "point_light";
		if (light_buffer[i].HasMember("model"))
			light_model = light_buffer[i]["model"].GetString();

		if (light_model == "directional_light")
		{
			// Load Direction			
			Vector<float, 3> direction{0.0f, -1.0f, 0.0f};
			if (light_buffer[i].HasMember("direction"))
				direction = { light_buffer[i]["direction"]["x"].GetFloat(),
					light_buffer[i]["direction"]["y"].GetFloat(),
					light_buffer[i]["direction"]["z"].GetFloat() };

			controller->push_light(new DirectionalLight(position, color, direction));
		}
		else if (light_model == "point_light")
			controller->push_light(new PointLight(position, color));
		else if (light_model == "spot_light")
		{
			// In and out angles
			float in_angle = 0.436332f;
			if (light_buffer[i].HasMember("in_angle"))
				in_angle = light_buffer[i]["in_angle"].GetFloat();
			float out_angle = 0.523599f;
			if (light_buffer[i].HasMember("out_angle"))
				out_angle = light_buffer[i]["out_angle"].GetFloat();

			// Load Direction
			Vector<float, 3> direction{0.0f, -1.0f, 0.0f};
			if (light_buffer[i].HasMember("direction"))
				direction = { light_buffer[i]["direction"]["x"].GetFloat(),
					light_buffer[i]["direction"]["y"].GetFloat(),
					light_buffer[i]["direction"]["z"].GetFloat() };
			controller->push_light(new SpotLight(position, color, in_angle, out_angle, direction));
		}
		else
		{
			std::cout << light_model << " is not one type of light" << std::endl;
		}
	}
	return true;
}

bool ProjectFileReader::load_objects(rapidjson::Document const& document, ProjectController* controller)
{
	if (!document.HasMember("object_buffer"))
	{
		std::cout << "There isn't object in the file." << std::endl;
		return false;
	}
	auto& object_buffer = document["object_buffer"];
	for (unsigned int i = 0; i < object_buffer.Size(); i++)
	{
		// Load object position
		Vector<float, 3> position{ 0.0f, 0.0f, 0.0f };
		if (object_buffer[i].HasMember("position"))
			position = { object_buffer[i]["position"]["x"].GetFloat(),
				object_buffer[i]["position"]["y"].GetFloat(),
				object_buffer[i]["position"]["z"].GetFloat() };

		// Load scale position
		Vector<float, 3> scale{ 1.0f, 1.0f, 1.0f };
		if (object_buffer[i].HasMember("scale"))
			scale = { object_buffer[i]["scale"]["x"].GetFloat(),
				object_buffer[i]["scale"]["y"].GetFloat(),
				object_buffer[i]["scale"]["z"].GetFloat() };

		// Is visible?
		bool is_visible = true;
		if (object_buffer[i].HasMember("visibility"))
			is_visible = object_buffer[i]["visibility"].GetBool();

		// Load object material
		Material material;
		if (object_buffer[i].HasMember("material")) 
		{
			auto& material_json = object_buffer[i]["material"];
			
			// Check if it's possible load material
			if (!material_json.HasMember("ambient"))
			{
				std::cout << "It wasn't possible to load material of object. Ambient light information is missing" << i << std::endl;
				break;
			}
			if (!material_json.HasMember("diffuse"))
			{
				std::cout << "It wasn't possible to load material of object. Diffuse light information is missing" << i << std::endl;
				break;
			}
			if (!material_json.HasMember("specular"))
			{
				std::cout << "It wasn't possible to load material of object. specular light information is missing" << i << std::endl;
				break;
			}

			if (!material_json.HasMember("shininess"))
			{
				std::cout << "It wasn't possible to load material of object. shininess light information is missing" << i << std::endl;
				break;
			}

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
			material = Material(ambient, diffuse, specular, shininess);
		}
		
		// Create Object
		std::string object_model = "cube";
		if (object_buffer[i].HasMember("model"))
			object_model = object_buffer[i]["model"].GetString();
		
		Model3D* object = nullptr;
		if (object_model == "plane")
			object = new Model3D(new Mesh(Shape::PLANE));
		else if (object_model == "cube")
			object = new Model3D(new Mesh(Shape::CUBE));
		else if (object_model == "pyramid")
			object = new Model3D(new Mesh(Shape::PYRAMID));
		else if (object_model == "sphere")
			object = new Model3D(new Mesh(Shape::SPHERE));
		
		object->set_position(position);
		object->set_scale(scale);
		object->set_material(material);
		
		// pass object ownership to controller.
		controller->push_object(object);
	}
	return true;
}
