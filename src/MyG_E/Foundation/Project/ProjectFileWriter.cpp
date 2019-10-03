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
		writer.String("edit_camera");
	if (dynamic_cast<FPSCamera const*>(camera))
		writer.String("fps_camera");
	writer.Key("position");

	writer.StartObject(); // Camera Position
	writer.Key("x");
	writer.Double(camera->get_position()[0]);
	writer.Key("y");
	writer.Double(camera->get_position()[1]);
	writer.Key("z");
	writer.Double(camera->get_position()[2]);
	writer.EndObject(); // End Camera Position

	writer.Key("direction");
	writer.StartObject(); // Camera Direction
	writer.Key("x");
	writer.Double(camera->get_direction()[0]);
	writer.Key("y");
	writer.Double(camera->get_direction()[1]);
	writer.Key("z");
	writer.Double(camera->get_direction()[2]);
	writer.EndObject(); // End Camera Direction
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

		writer.Key("model");
		switch (object_buffer[i]->get_shape())
		{
		case Shape::PLANE:
			writer.String("plane");
			break;
		case Shape::CUBE:
			writer.String("cube");
			break;
		case Shape::SPHERE:
			writer.String("sphere");
			break;
		case Shape::PYRAMID:
			writer.String("pyramid");
			break;
		default:
			break;
		}

		writer.Key("visibility");
		writer.Bool(object_buffer[i]->is_visible());

		writer.Key("position");
		writer.StartObject(); // Position
		writer.Key("x");
		writer.Double(object_buffer[i]->get_position()[0]);
		writer.Key("y");
		writer.Double(object_buffer[i]->get_position()[1]);
		writer.Key("z");
		writer.Double(object_buffer[i]->get_position()[2]);
		writer.EndObject(); // Position

		writer.Key("scale");
		writer.StartObject(); // Scale
		writer.Key("x");
		writer.Double(object_buffer[i]->get_scale()[0]);
		writer.Key("y");
		writer.Double(object_buffer[i]->get_scale()[1]);
		writer.Key("z");
		writer.Double(object_buffer[i]->get_scale()[2]);
		writer.EndObject(); // Scale

		writer.Key("rotation");
		writer.StartObject(); // Rotation
		writer.Key("x");
		writer.Double(object_buffer[i]->get_rotation()[0]);
		writer.Key("y");
		writer.Double(object_buffer[i]->get_rotation()[1]);
		writer.Key("z");
		writer.Double(object_buffer[i]->get_rotation()[2]);
		writer.EndObject(); // Rotation

		writer.Key("material");
		writer.StartObject(); // Material

		if (object_buffer[i]->get_material().get_diffuse()->is_unitary())
		{
			writer.Key("has_texture");
			writer.Bool(false);
			writer.Key("diffuse");
			writer.StartObject(); // Diffuse
			writer.Key("x");
			writer.Double(object_buffer[i]->get_material().get_diffuse()->get_color()[0]);
			writer.Key("y");
			writer.Double(object_buffer[i]->get_material().get_diffuse()->get_color()[1]);
			writer.Key("z");
			writer.Double(object_buffer[i]->get_material().get_diffuse()->get_color()[2]);
			writer.EndObject(); // Diffuse
		}
		else
		{
			writer.Key("has_texture");
			writer.Bool(true);
			writer.Key("texture");
			writer.String(
				std::filesystem::relative(object_buffer[i]->get_material().get_diffuse()->get_filepath()).string().c_str());
		}

		if (object_buffer[i]->get_material().get_specular()->is_unitary())
		{
			writer.Key("has_specular_map");
			writer.Bool(false);
			writer.Key("specular");
			writer.StartObject(); // specular
			writer.Key("x");
			writer.Double(object_buffer[i]->get_material().get_specular()->get_color()[0]);
			writer.Key("y");
			writer.Double(object_buffer[i]->get_material().get_specular()->get_color()[1]);
			writer.Key("z");
			writer.Double(object_buffer[i]->get_material().get_specular()->get_color()[2]);
			writer.EndObject(); // specular
		}
		else
		{
			writer.Key("has_specular_map");
			writer.Bool(true);
			writer.Key("specular_map");
			writer.String(
				std::filesystem::relative(object_buffer[i]->get_material().get_specular()->get_filepath()).string().c_str());
		}

		writer.Key("shininess");
		writer.Double(object_buffer[i]->get_material().get_shininess());
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
		writer.StartObject(); // Position
		writer.Key("x");
		writer.Double(light_buffer[i]->get_light_position()[0]);
		writer.Key("y");
		writer.Double(light_buffer[i]->get_light_position()[1]);
		writer.Key("z");
		writer.Double(light_buffer[i]->get_light_position()[2]);
		writer.EndObject(); // Position

		writer.Key("color");
		writer.StartObject(); // color
		writer.Key("x");
		writer.Double(light_buffer[i]->get_light_color()[0]);
		writer.Key("y");
		writer.Double(light_buffer[i]->get_light_color()[1]);
		writer.Key("z");
		writer.Double(light_buffer[i]->get_light_color()[0]);
		writer.EndObject(); // color

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
			writer.StartObject(); // Direction
			writer.Key("x");
			writer.Double(light->GetDirection()[0]);
			writer.Key("y");
			writer.Double(light->GetDirection()[1]);
			writer.Key("z");
			writer.Double(light->GetDirection()[2]);
			writer.EndObject(); // Direction
		}
		else if (dynamic_cast<SpotLight*>(light_buffer[i]))
		{
			writer.Key("model");
			writer.String("spot_light");

			SpotLight* light = static_cast<SpotLight*>(light_buffer[i]);

			writer.Key("direction");
			writer.StartObject(); // Direction
			writer.Key("x");
			writer.Double(light->GetDirection()[0]);
			writer.Key("y");
			writer.Double(light->GetDirection()[1]);
			writer.Key("z");
			writer.Double(light->GetDirection()[2]);
			writer.EndObject(); // Direction

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
