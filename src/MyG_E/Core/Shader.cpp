#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "Foundation/Gldebug.h"
#include "Shader.h"


Shader::Shader(std::string const& vert_file, std::string const& frag_file, std::string const& geom_file)
	: m_shader_program(0)
	, m_vert_file(vert_file)
	, m_frag_file(frag_file)
	, m_geom_file(geom_file)
{
	ASSERT(!create_shader());
}

Shader::~Shader() {
	GLcall(glDeleteProgram(m_shader_program))
}

void Shader::bind() const
{
	GLcall(glUseProgram(m_shader_program));
}

void Shader::unbind() const
{
	GLcall(glUseProgram(0));
}

void Shader::set_uniform1i(int location, int value) const
{
	GLcall(glUniform1i(location, value));
}

void Shader::set_uniform1f(int location, float value) const
{
	GLcall(glUniform1f(location, value));
}

void Shader::set_uniform2f(int location, Vector2f const& vector) const
{
	GLcall(glUniform2f(location, vector[0], vector[1]));
}

void Shader::set_uniform3f(int location, const Vector3f& vector) const
{
	GLcall(glUniform3f(location, vector[0], vector[1], vector[2]));
}

void Shader::set_uniform4f(int location, const Vector<float, 4>& vector) const
{
	GLcall(glUniform4f(location, vector[0], vector[1], vector[2], vector[3]));
}

void Shader::set_uniformMatrix3f(int location, const Matrix<float, 3, 3>& matrix) const
{
	GLcall(glUniformMatrix3fv(location, 1, GL_TRUE, reinterpret_cast<const float *>(matrix.GetAsPointer()) ));
}

void Shader::set_uniformMatrix4f(int location, const Matrix<float, 4, 4>& matrix) const
{
	GLcall(glUniformMatrix4fv(location, 1, GL_TRUE, reinterpret_cast<const float *>(matrix.GetAsPointer()) ));
}

int Shader::get_uniform_location(const std::string& name) const
{
	GLcall(int uniform_location = glGetUniformLocation(m_shader_program, name.c_str()));
	if (uniform_location == -1)
		std::cout << "There isn't any uniform named:" << name << std::endl;
	return uniform_location;
}

bool Shader::create_shader()
{
	// read vertex file.
	std::string vert_string("");
	if (!read_shader_file(vert_string, m_vert_file))
		return false;

	// read fragment file.
	std::string frag_string("");
	if (!read_shader_file(frag_string, m_frag_file))
		return false;


	// Create a shader program that links together all the shaders.
	GLcall(m_shader_program = glCreateProgram());

	unsigned int vert_shader = compile_shader(vert_string, GL_VERTEX_SHADER);
	unsigned int frag_shader = compile_shader(frag_string, GL_FRAGMENT_SHADER);
	unsigned int geom_shader = 0; // It wont be used if there is no geometry shader.

	GLcall(glAttachShader(m_shader_program, vert_shader));
	GLcall(glAttachShader(m_shader_program, frag_shader));

	// if there is a geometry shader.
	if (!m_geom_file.empty())
	{
		// read geometry file.
		std::string geom_string("");
		if (!read_shader_file(geom_string, m_geom_file))
			return false;

		geom_shader = compile_shader(geom_string, GL_GEOMETRY_SHADER);
		GLcall(glAttachShader(m_shader_program, geom_shader));
	}

	GLcall(glLinkProgram(m_shader_program));
	
	// Checks whether the executables contained in the program can execute given the current OpenGL state
	GLcall(glValidateProgram(m_shader_program));

	// Delete intermidiates Shaders 
	GLcall(glDeleteShader(vert_shader));
	GLcall(glDeleteShader(frag_shader));
	if (geom_shader)
		GLcall(glDeleteShader(geom_shader));

	// Testing if everything went right
	int result;
	GLcall(glGetProgramiv(m_shader_program, GL_VALIDATE_STATUS, &result));
	if (!result) 
	{
		int length;
		GLcall(glGetProgramiv(m_shader_program, GL_INFO_LOG_LENGTH, &length));
		char *message = (char *)alloca(length * sizeof(char));  //Alloca is a c fuction use for alloc dinamically on stack
		GLcall(glGetProgramInfoLog(m_shader_program, length, &length, message));
		
		std::cout << "Failed to create Program" << std::endl;
		return false;
	}
	return true;
}


bool Shader::read_shader_file(std::string& shader_string, std::string& file_name)
{
	std::string file_path = "..\\..\\..\\src\\MyG_E\\Shaders\\" + file_name;
	std::ifstream file(file_path);

	if (file.is_open())
	{
		shader_string.assign(std::istreambuf_iterator<char>(file),
			std::istreambuf_iterator<char>());
		return true;
	}
	else
	{
		std::cout << "Failure to open" << file_path << "shader file" << std::endl;
		return false;
	}
}


unsigned int Shader::compile_shader(const std::string& source, const unsigned int shader_type) 
{
	// Create a shader of the specified type
	GLcall(unsigned int id = glCreateShader(shader_type));
	const char* c_str = source.c_str();

	// Set the source characters and try to compile
	GLcall(glShaderSource(id, 1, &c_str, nullptr));
	GLcall(glCompileShader(id));

	// Testing if everything went right.
	int result;
	GLcall(glGetShaderiv(id, GL_COMPILE_STATUS, &result));
	if (!result)
	{
		int length;
		GLcall(glGetShaderiv(id, GL_INFO_LOG_LENGTH, &length));
		char *message = (char *)alloca(length * sizeof(char));  //Alloca is a c fuction use for alloc dinamically on stack
		GLcall(glGetShaderInfoLog(id, length, &length, message));

		std::cout << "Failed to compailed " <<(shader_type == GL_VERTEX_SHADER ? "vertex" : "fragment" ) << "shader!" << std::endl;
		std::cout << message << std::endl;
	}

	return id;
}
