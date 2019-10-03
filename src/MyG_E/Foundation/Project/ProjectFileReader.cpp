#include "ProjectFileReader.h"

#include "Core/ProjectController.h"

// It needs a redsign.

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
	// Load camera // ISNT LOADING FOV< SPEED< SENSIBILITY
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
		if (light_buffer[i].HasMember("position"))
			position = { light_buffer[i]["position"]["x"].GetFloat(),
				light_buffer[i]["position"]["y"].GetFloat(),
				light_buffer[i]["position"]["z"].GetFloat() };

		// Load Light Color
		Vector<float, 3> color{ 1.0, 1.0, 1.0 };
		if (light_buffer[i].HasMember("light_color"))
			color = { light_buffer[i]["color"]["x"].GetFloat(),
				light_buffer[i]["color"]["y"].GetFloat(),
				light_buffer[i]["color"]["z"].GetFloat() };

		float ambient_strength = 0.1f;
		float diffuse_strength = 1.0f;
		float specular_strength = 1.0f;
		if (light_buffer[i].HasMember("ambient_strength"))
			ambient_strength = light_buffer[i]["ambient_strength"].GetDouble();
		if (light_buffer[i].HasMember("diffuse_strength"))
			ambient_strength = light_buffer[i]["diffuse_strength"].GetDouble();
		if (light_buffer[i].HasMember("specular_strength"))
			ambient_strength = light_buffer[i]["specular_strength"].GetDouble();

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

			DirectionalLight* light = new DirectionalLight(position, color, direction);
			load_light_strength(light, light_buffer[i]);
			controller->push_light(light);
		}
		else if (light_model == "point_light")
		{
			PointLight* light = new PointLight(position, color);
			load_light_strength(light, light_buffer[i]);
			controller->push_light(light);
		}
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
			SpotLight* light = new SpotLight(position, color, in_angle, out_angle, direction);
			load_light_strength(light, light_buffer[i]);
			controller->push_light(light);
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

		// Load object material
		if (object_buffer[i].HasMember("material"))
		{
			auto& material_json = object_buffer[i]["material"];

			// Check if it's possible load material
			bool check = true;
			if (!material_json.HasMember("has_texture"))
			{
				std::cout << "It wasn't possible to load material of object. has texture information is missing" << i << std::endl;
				check = false;
			}
			if (!material_json.HasMember("has_specular_map"))
			{
				std::cout << "It wasn't possible to load material of object. has specular_map information is missing" << i << std::endl;
				check = false;
			}
			if (check)
			{
				bool has_texture = material_json["has_texture"].GetBool(); // If true use texture if false load diffuse 
				bool has_specular_map = material_json["has_specular_map"].GetBool();

				if (!material_json.HasMember("diffuse") && !has_texture)
				{
					std::cout << "It wasn't possible to load material of object. Diffuse information is missing" << i << std::endl;
					check = false;
				}
				if (!material_json.HasMember("texture") && has_texture)
				{
					std::cout << "It wasn't possible to load material of object. Teture information is missing" << i << std::endl;
					check = false;
				}

				if (!material_json.HasMember("specular") && !has_specular_map)
				{
					std::cout << "It wasn't possible to load material of object. specular information is missing" << i << std::endl;
					check = false;
				}
				
				if (!material_json.HasMember("specular_map") && has_specular_map)
				{
					std::cout << "It wasn't possible to load material of object. specular_map information is missing" << i << std::endl;
					check = false;
				}

				if (!material_json.HasMember("shininess"))
				{
					std::cout << "It wasn't possible to load material of object. shininess information is missing" << i << std::endl;
					check = false;
				}

				if (check)
				{
					float shininess = material_json["shininess"].GetFloat();

					Texture2D* specular;
					if (!has_specular_map)
						specular = new Texture2D(Vector<float, 3>{ material_json["specular"]["x"].GetFloat(),
							material_json["specular"]["y"].GetFloat(),
							material_json["specular"]["z"].GetFloat() });
					else
						specular = new Texture2D(std::filesystem::absolute(material_json["specular_map"].GetString()).string());
					if (!has_texture)
					{
						Vector<float, 3> diffuse{ material_json["diffuse"]["x"].GetFloat(),
							material_json["diffuse"]["y"].GetFloat(),
							material_json["diffuse"]["z"].GetFloat() };
						object->set_material(Material(new Texture2D(diffuse), specular, shininess));
					}
					else
						object->set_material(Material(new Texture2D(std::filesystem::absolute(
							material_json["texture"].GetString()).string()), specular, shininess));
				}
			}
		}
		
		// pass object ownership to controller.
		controller->push_object(object);
	}
	return true;
}

void ProjectFileReader::load_light_strength(Light* light, rapidjson::Value const& json)
{
	light->set_ambient_strength(json["ambient_strength"].GetDouble());
	light->set_diffuse_strength(json["diffuse_strength"].GetDouble());
	light->set_specular_strength(json["specular_strength"].GetDouble());
}
