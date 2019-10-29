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

	if (document.HasMember("skybox"))
		controller->set_skybox(new TextureCubMap(document["skybox"].GetString()));

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

	float yaw = 0.0f;
	if (document["camera"].HasMember("yaw"))
	{
		yaw = document["camera"]["yaw"].GetFloat();
	}

	float pitch = 0.0f;
	if (document["camera"].HasMember("pitch"))
	{
		yaw = document["camera"]["pitch"].GetFloat();
	}

	// Create new camera
	std::string camera_model(document["camera"]["model"].GetString());

	// Load camera position
	Vector3f position{ document["camera"]["position"]["x"].GetFloat(),
		document["camera"]["position"]["y"].GetFloat(),
		document["camera"]["position"]["z"].GetFloat() };
	
	// Create new camera
	if (camera_model == "edit_camera")
	{
		if (!document["camera"].HasMember("focal_point"))
		{
			std::cout << "There isn't the direction of the camera in the file" << std::endl;
			return false;
		}
		// Load camera focal point
		Vector3f focal_point{ document["camera"]["focal_point"]["x"].GetFloat(),
			document["camera"]["focal_point"]["y"].GetFloat(),
			document["camera"]["focal_point"]["z"].GetFloat() };
		controller->set_camera(new EditCamera(position, focal_point, yaw, pitch));
	}
	else if (document["camera"]["model"].GetString() == "fps_camera")
	{
		if (!document["camera"].HasMember("direction"))
		{
			std::cout << "There isn't the direction of the camera in the file" << std::endl;
			return false;
		}
		// Load camera direction
		Vector3f direction{ document["camera"]["direction"]["x"].GetFloat(),
			document["camera"]["direction"]["y"].GetFloat(),
			document["camera"]["direction"]["z"].GetFloat() };
		controller->set_camera(new FPSCamera(position, yaw, pitch));
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
		Vector3f position;
		if (light_buffer[i].HasMember("position"))
			position = { light_buffer[i]["position"]["x"].GetFloat(),
				light_buffer[i]["position"]["y"].GetFloat(),
				light_buffer[i]["position"]["z"].GetFloat() };

		// Load Light Color
		Vector3f color{ 1.0, 1.0, 1.0 };
		if (light_buffer[i].HasMember("light_color"))
			color = { light_buffer[i]["color"]["x"].GetFloat(),
				light_buffer[i]["color"]["y"].GetFloat(),
				light_buffer[i]["color"]["z"].GetFloat() };

		float ambient_strength = 0.1f;
		float diffuse_strength = 1.0f;
		float specular_strength = 1.0f;
		if (light_buffer[i].HasMember("ambient_strength"))
			ambient_strength = light_buffer[i]["ambient_strength"].GetFloat();
		if (light_buffer[i].HasMember("diffuse_strength"))
			ambient_strength = light_buffer[i]["diffuse_strength"].GetFloat();
		if (light_buffer[i].HasMember("specular_strength"))
			ambient_strength = light_buffer[i]["specular_strength"].GetFloat();

		// Create light
		std::string light_model = "point_light";
		if (light_buffer[i].HasMember("model"))
			light_model = light_buffer[i]["model"].GetString();

		if (light_model == "directional_light")
		{
			// Load Direction			
			Vector3f direction{0.0f, -1.0f, 0.0f};
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
			Vector3f direction{0.0f, -1.0f, 0.0f};
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
		Vector3f position{ 0.0f, 0.0f, 0.0f };
		if (object_buffer[i].HasMember("position"))
			position = { object_buffer[i]["position"]["x"].GetFloat(),
				object_buffer[i]["position"]["y"].GetFloat(),
				object_buffer[i]["position"]["z"].GetFloat() };

		// Load scale position
		Vector3f scale{ 1.0f, 1.0f, 1.0f };
		if (object_buffer[i].HasMember("scale"))
			scale = { object_buffer[i]["scale"]["x"].GetFloat(),
				object_buffer[i]["scale"]["y"].GetFloat(),
				object_buffer[i]["scale"]["z"].GetFloat() };

		// Load rotation
		Vector3f rotation{ 0.0f, 0.0f, 0.0f };
		if (object_buffer[i].HasMember("rotation"))
			rotation = { object_buffer[i]["rotation"]["x"].GetFloat(),
				object_buffer[i]["rotation"]["y"].GetFloat(),
				object_buffer[i]["rotation"]["z"].GetFloat() };

		// Is visible?
		bool is_visible = true;
		if (object_buffer[i].HasMember("visibility"))
			is_visible = object_buffer[i]["visibility"].GetBool();
		
		// Create Object
		std::string file_path;
		if (object_buffer[i].HasMember("path"))
			file_path = object_buffer[i]["path"].GetString();
		else
		{
			std::cout << "It wasn't possible to load object " << i << std::endl;
			continue;
		}

		Model3D* object = nullptr;
		Material* material = nullptr;

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

					Texture2D* specular = nullptr;
					if (!has_specular_map)
						specular = new Texture2D(Vector3f{ material_json["specular"]["x"].GetFloat(),
							material_json["specular"]["y"].GetFloat(),
							material_json["specular"]["z"].GetFloat() });
					else
						specular = new Texture2D(std::filesystem::absolute(material_json["specular_map"].GetString()).string());
					if (material_json.HasMember("specular_map_scale_uv"))
						specular->set_scale_uv(Vector2f{ material_json["specular_map_scale_uv"]["x"].GetFloat(),
							material_json["texture_scale_uv"]["y"].GetFloat() });

					Texture2D* diffuse = nullptr;
					if (!has_texture)
					{
						diffuse = new Texture2D(Vector3f({ material_json["diffuse"]["x"].GetFloat(),
							material_json["diffuse"]["y"].GetFloat(),
							material_json["diffuse"]["z"].GetFloat() }));
					}
					else
						diffuse = new Texture2D(std::filesystem::absolute(material_json["texture"].GetString()).string());
					if (material_json.HasMember("texture_scale_uv"))
						diffuse->set_scale_uv(Vector2f{ material_json["texture_scale_uv"]["x"].GetFloat(),
							material_json["texture_scale_uv"]["y"].GetFloat() });

					Texture2D* normal_map = nullptr;
					if (material_json.HasMember("normal_map"))
						normal_map = new Texture2D(std::filesystem::absolute(material_json["normal_map"].GetString()).string());
					else
						normal_map = new Texture2D(Vector3f({0.0f, 0.0f, 1.0f}));
					if (material_json.HasMember("normal_map_scale_uv"))
						normal_map->set_scale_uv(Vector2f{ material_json["normal_map_scale_uv"]["x"].GetFloat(),
							material_json["normal_map_scale_uv"]["y"].GetFloat() });

					material = new Material(diffuse, specular, normal_map, shininess);
					object = new Model3D(material, file_path);
				}
			}
		}

		object->set_position(position);
		object->set_scale(scale);
		object->set_rotation(rotation);
		// pass object ownership to controller.
		controller->push_object(object);
	}
	return true;
}

void ProjectFileReader::load_light_strength(Light* light, rapidjson::Value const& json)
{
	light->set_ambient_strength(json["ambient_strength"].GetFloat());
	light->set_diffuse_strength(json["diffuse_strength"].GetFloat());
	light->set_specular_strength(json["specular_strength"].GetFloat());
}
