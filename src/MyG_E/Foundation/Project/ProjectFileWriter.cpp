#include "ProjectFileWriter.h"

bool ProjectFileWriter::write_file(ProjectController const* project_controller, std::string const& filename )
{
	rapidjson::StringBuffer string_buffer;
	rapidjson::PrettyWriter<rapidjson::StringBuffer> writer(string_buffer);
	
	// must pass an allocator when the object may need to allocate memory
	writer.StartObject(); // root

	write_camera(writer, project_controller);
	write_objects(writer, project_controller);
	write_lights(writer, project_controller);
	
	// skybox
	if (project_controller->get_skybox())
	{
		writer.Key("skybox");
		writer.String(std::filesystem::relative(
			project_controller->get_skybox()->get_texture()->get_folder()).string().c_str());
	}
	writer.EndObject(); // root

	// Write file
	std::ofstream file;
	file.open(filename);
	file << string_buffer.GetString();
	file.close();

	return true;
}

void ProjectFileWriter::write_camera(rapidjson::PrettyWriter<rapidjson::StringBuffer>& writer, ProjectController const* project_controller)
{
	// Write camera
	Camera const* camera = project_controller->get_camera();

	writer.Key("camera");

	writer.StartObject(); // Camera
	writer.Key("model");
	if (dynamic_cast<EditCamera const*>(camera))
	{
		writer.String("edit_camera");
		writer.Key("direction");
		write_vector3f(writer, &dynamic_cast<EditCamera const*>(camera)->get_focal_point()[0]);
	}
	if (dynamic_cast<FPSCamera const*>(camera))
	{
		writer.String("fps_camera");
		writer.Key("focal_point");
		write_vector3f(writer, &dynamic_cast<FPSCamera const*>(camera)->get_direction()[0]);
	}

	writer.Key("position");
	write_vector3f(writer, &camera->get_position()[0]);

	writer.Key("yaw");
	writer.Double(camera->get_yaw());

	writer.Key("pitch");
	writer.Double(camera->get_pitch());

	writer.EndObject(); // End Camera
}

void ProjectFileWriter::write_objects(rapidjson::PrettyWriter<rapidjson::StringBuffer>& writer, ProjectController const* project_controller)
{
	// Write Object Buffer
	std::vector<Model3D*> const object_buffer = project_controller->get_object_buffer();
	writer.Key("object_buffer");
	writer.StartArray();
	for (unsigned int i = 0; i < object_buffer.size(); i++)
	{
		writer.StartObject(); // Object

		writer.Key("path");
		writer.String(object_buffer[i]->get_path().c_str());
		writer.Key("visibility");
		writer.Bool(object_buffer[i]->is_visible());

		writer.Key("position");
		write_vector3f(writer, &object_buffer[i]->get_position()[0]);

		writer.Key("scale");
		write_vector3f(writer, &object_buffer[i]->get_scale()[0]);

		writer.Key("rotation");
		write_vector3f(writer, &object_buffer[i]->get_rotation()[0]);

		writer.Key("material");
		writer.StartObject(); // Material

		// Texture
		if (object_buffer[i]->get_material()->get_diffuse()->is_unitary())
		{
			writer.Key("has_texture");
			writer.Bool(false);
			writer.Key("diffuse");
			write_vector3f(writer, object_buffer[i]->get_material()->get_diffuse()->get_color());
		}
		else
		{
			writer.Key("has_texture");
			writer.Bool(true);
			writer.Key("texture");
			writer.String(
				std::filesystem::relative(object_buffer[i]->get_material()->get_diffuse()->get_filepath()).string().c_str());
		}
		writer.Key("texture_scale_uv");
		write_vector2f(writer, &object_buffer[i]->get_material()->get_diffuse()->get_scale_uv()[0]);

		// Specular Map
		if (object_buffer[i]->get_material()->get_specular()->is_unitary())
		{
			writer.Key("has_specular_map");
			writer.Bool(false);
			writer.Key("specular");
			write_vector3f(writer, object_buffer[i]->get_material()->get_specular()->get_color());
		}
		else
		{
			writer.Key("has_specular_map");
			writer.Bool(true);
			writer.Key("specular_map");
			writer.String(
				std::filesystem::relative(object_buffer[i]->get_material()->get_specular()->get_filepath()).string().c_str());
		}
		writer.Key("specular_map_scale_uv");
		write_vector2f(writer, &object_buffer[i]->get_material()->get_specular()->get_scale_uv()[0]);

		// Normal map
		if (!object_buffer[i]->get_material()->get_normal_map()->is_unitary())
		{
			writer.Key("normal_map");
			writer.String(
				std::filesystem::relative(object_buffer[i]->get_material()->get_normal_map()->get_filepath()).string().c_str());
		}
		writer.Key("normal_map_scale_uv");
		write_vector2f(writer, &object_buffer[i]->get_material()->get_normal_map()->get_scale_uv()[0]);

		writer.Key("shininess");
		writer.Double(object_buffer[i]->get_material()->get_shininess());
		writer.EndObject(); // Material
		writer.EndObject(); // Object
	}
	writer.EndArray();
}

void ProjectFileWriter::write_lights(rapidjson::PrettyWriter<rapidjson::StringBuffer>& writer, ProjectController const* project_controller)
{
	// Write Light
	std::vector<Light*> const light_buffer = project_controller->get_light_buffer();
	writer.Key("light_buffer");
	writer.StartArray();
	for (unsigned int i = 0; i < light_buffer.size(); i++)
	{
		writer.StartObject(); // Light

		writer.Key("position");
		write_vector3f(writer, &light_buffer[i]->get_light_position()[0]);

		writer.Key("color");
		write_vector3f(writer, &light_buffer[i]->get_light_color()[0]);

		writer.Key("ambient_strength");
		writer.Double(light_buffer[i]->get_ambient_strength());
		writer.Key("diffuse_strength");
		writer.Double(light_buffer[i]->get_diffuse_strength());
		writer.Key("specular_strength");
		writer.Double(light_buffer[i]->get_specular_strength());

		if (dynamic_cast<DirectionalLight*>(light_buffer[i]))
		{
			writer.Key("model");
			writer.String("directional_light");
			DirectionalLight* light = static_cast<DirectionalLight*>(light_buffer[i]);

			writer.Key("direction");
			write_vector3f(writer, &light->GetDirection()[0]);
		}
		else if (dynamic_cast<SpotLight*>(light_buffer[i]))
		{
			writer.Key("model");
			writer.String("spot_light");

			SpotLight* light = static_cast<SpotLight*>(light_buffer[i]);

			writer.Key("direction");
			write_vector3f(writer, &light->GetDirection()[0]);

			writer.Key("in_angle");
			writer.Double(light->GetInAngle());
			writer.Key("out_angle");
			writer.Double(light->GetOutAngle());
		}
		else
		{
			writer.Key("model");
			writer.String("point_light");
		}

		writer.EndObject(); // Light
	}
	writer.EndArray();
}

void ProjectFileWriter::write_vector3f(rapidjson::PrettyWriter<rapidjson::StringBuffer>& writer, float const* vec)
{
	writer.StartObject();
	writer.Key("x");
	writer.Double(vec[0]);
	writer.Key("y");
	writer.Double(vec[1]);
	writer.Key("z");
	writer.Double(vec[2]);
	writer.EndObject();
}

void ProjectFileWriter::write_vector2f(rapidjson::PrettyWriter<rapidjson::StringBuffer>& writer, float const* vec)
{
	writer.StartObject();
	writer.Key("x");
	writer.Double(vec[0]);
	writer.Key("y");
	writer.Double(vec[1]);
	writer.EndObject();
}
