#include "ProjectFileWriter.h"

#include "rapidjson/stringbuffer.h"
#include "rapidjson/prettywriter.h"

bool ProjectFileWriter::write_file(ProjectController const* project_controller, std::string const& filename )
{
	rapidjson::StringBuffer string_buffer;
	rapidjson::PrettyWriter<rapidjson::StringBuffer> writer(string_buffer);
	
	// must pass an allocator when the object may need to allocate memory

	// Write camera
	writer.StartObject(); // root
	{
		Camera const* camera = project_controller->get_camera();
		
		writer.Key("camera");

		writer.StartObject(); // Camera
		writer.Key("model");
		if(dynamic_cast<EditCamera const*>(camera))
			writer.String("edit_camera");
		if (dynamic_cast<FPSCamera const*>(camera))
			writer.String("fps_camera");
		writer.Key("position");

		writer.StartObject(); // Camera Position
		writer.Key("x");
		writer.Double(camera->GetPosition()[0]);
		writer.Key("y");
		writer.Double(camera->GetPosition()[1]);
		writer.Key("z");
		writer.Double(camera->GetPosition()[2]);
		writer.EndObject(); // End Camera Position

		writer.Key("direction");
		writer.StartObject(); // Camera Direction
		writer.Key("x");
		writer.Double(camera->GetDirection()[0]);
		writer.Key("y");
		writer.Double(camera->GetDirection()[1]);
		writer.Key("z");
		writer.Double(camera->GetDirection()[2]);
		writer.EndObject(); // End Camera Direction
		writer.EndObject(); // End Camera
	}

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

		writer.Key("material");
		writer.StartObject(); // Material
		
		writer.Key("ambient");
		writer.StartObject(); // Ambient
		writer.Key("x");
		writer.Double(object_buffer[i]->get_material().ambient[0]);
		writer.Key("y");
		writer.Double(object_buffer[i]->get_material().ambient[1]);
		writer.Key("z");
		writer.Double(object_buffer[i]->get_material().ambient[2]);
		writer.EndObject(); // ambient

		writer.Key("diffuse");
		writer.StartObject(); // Diffuse
		writer.Key("x");
		writer.Double(object_buffer[i]->get_material().diffuse[0]);
		writer.Key("y");
		writer.Double(object_buffer[i]->get_material().diffuse[1]);
		writer.Key("z");
		writer.Double(object_buffer[i]->get_material().diffuse[2]);
		writer.EndObject(); // Diffuse

		writer.Key("specular");
		writer.StartObject(); // specular
		writer.Key("x");
		writer.Double(object_buffer[i]->get_material().specular[0]);
		writer.Key("y");
		writer.Double(object_buffer[i]->get_material().specular[1]);
		writer.Key("z");
		writer.Double(object_buffer[i]->get_material().specular[2]);
		writer.EndObject(); // Specular
		
		writer.Key("shininess");
		writer.Double(object_buffer[i]->get_material().shininess);
		writer.EndObject(); // Material
		writer.EndObject(); // Object
	}
	writer.EndArray();
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
		writer.Double(light_buffer[i]->GetLightPosition()[0]);
		writer.Key("y");
		writer.Double(light_buffer[i]->GetLightPosition()[1]);
		writer.Key("z");
		writer.Double(light_buffer[i]->GetLightPosition()[2]);
		writer.EndObject(); // Position

		writer.Key("color");
		writer.StartObject(); // color
		writer.Key("x");
		writer.Double(light_buffer[i]->GetLightColor()[0]);
		writer.Key("y");
		writer.Double(light_buffer[i]->GetLightColor()[1]);
		writer.Key("z");
		writer.Double(light_buffer[i]->GetLightColor()[0]);
		writer.EndObject(); // color

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
	writer.EndObject(); // root

	// Write file
	std::ofstream file;
	file.open(filename);
	file << string_buffer.GetString();
	file.close();

	return true;
}
