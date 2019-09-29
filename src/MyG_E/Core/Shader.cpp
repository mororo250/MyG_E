#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "Foundation/Gldebug.h"
#include "Shader.h"


Shader::Shader(const std::string& filepath)
	: m_shader_program(0)
{
	ASSERT(!create_shader(filepath));
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

void Shader::set_uniform3f(int location, float v1, float v2, float v3) const
{
	GLcall(glUniform3f(location, v1, v2, v3));
}

void Shader::set_uniform4f(int location, float v1, float v2, float v3, float v4) const
{
	GLcall(glUniform4f(location, v1, v2, v3, v4));
}

void Shader::set_uniform3f(int location, const Vector<float, 3>& vector) const
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

//this function read the shader file and put the shader code in a string
void Shader::read_shader_file(const std::string& shader_name, std::string& vertex_string, std::string& frag_string)
{
	// Shader directory.
	std::string file_path = std::filesystem::current_path().parent_path().string();
	file_path += "\\src\\MyG_E\\Shaders\\" + shader_name;
	std::ifstream shader_file(file_path);

	//test if the file was open correctly
	if (shader_file.is_open())
	{
		std::string fLine;
		shader_type type = NONE;

		while (getline(shader_file, fLine))
		{
			if (fLine.find("#shader") != std::string::npos)
			{
				if (fLine.find("vertex") != std::string::npos)
				{
					type = VERTEX;
				}
				else if (fLine.find("fragment") != std::string::npos)
				{
					type = FRAGMENT;
				}
			}
			else {
				switch (type)
				{
				case VERTEX:
					vertex_string += std::move(fLine) + "\n";
					break;

				case FRAGMENT:
					frag_string += std::move(fLine) + "\n";
					break;

				default:
					break;
				}

			}
		}
	}
	else //if the file couldn't be opened print a error message
	{
		std::cout << "Failure to open shader file" << std::endl;
	}
}

bool Shader::create_shader(const std::string& filepath)
{
	std::string vertex_string("");
	std::string frag_string("");
	read_shader_file(filepath, vertex_string, frag_string);
	//create a shader program that links together the vertex/frag shaders
	GLcall(m_shader_program = glCreateProgram());


	unsigned int vertex_shader = compile_shader(vertex_string, GL_VERTEX_SHADER);
	unsigned int frag_shader = compile_shader(frag_string, GL_FRAGMENT_SHADER);

	// links together the vertex/frag shaders
	GLcall(glAttachShader(m_shader_program, vertex_shader));
	GLcall(glAttachShader(m_shader_program, frag_shader));
	GLcall(glLinkProgram(m_shader_program));
	// Checks whether the executables contained in the program can execute given the current OpenGL state
	GLcall(glValidateProgram(m_shader_program));

	// Delete intermidiates Shaders 
	GLcall(glDeleteShader(vertex_shader));
	GLcall(glDeleteShader(frag_shader));
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
