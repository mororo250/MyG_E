#include <iostream>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <fstream>
#include <string>

#include "Gldebug.h"
#include "Shader.h"


Shader::Shader(const std::string& filepath)
	: mShaderProgram(0)
{
	ASSERT(!CreateShader(filepath));
}

Shader::~Shader() {
	GLcall(glDeleteProgram(mShaderProgram))
}

void Shader::bind() const
{
	GLcall(glUseProgram(mShaderProgram));
}

void Shader::unbind() const
{
	GLcall(glUseProgram(0));
}

void Shader::SetUniform1i(int Location, int value)
{
	GLcall(glUniform1i(Location, value));
}

void Shader::SetUniform1f(int Location, float value)
{
	GLcall(glUniform1f(Location, value));
}

void Shader::SetUniform4f(int Location, float v1, float v2, float v3, float v4)
{
	GLcall(glUniform4f(Location, v1, v2, v3, v4));
}

void Shader::SetUniformMatrix3f(int Location, Matrix<float, 3, 3> matrix)
{
	GLcall(glUniformMatrix3fv(Location, 1, GL_TRUE, reinterpret_cast<const float *>(matrix.GetAsPointer()) ));
}

void Shader::SetUniformMatrix4f(int Location, Matrix<float, 4, 4> matrix)
{
	GLcall(glUniformMatrix4fv(Location, 1, GL_TRUE, reinterpret_cast<const float *>(matrix.GetAsPointer()) ));
}

int Shader::GetUniformLocation(const std::string& name) const
{
	GLcall(int UniformLocation = glGetUniformLocation(mShaderProgram, name.c_str()));
	if (UniformLocation == -1)
		std::cout << "There isn't any uniform named:" << name;
	return UniformLocation;
}

//this function read the shader file and put the shader code in a string
void Shader::ReadShader_File(const std::string& filepath, std::string& VertexString, std::string& FragString)
{
	std::ifstream ShaderFile(filepath);
	//test if the file was open correctly
	if (ShaderFile.is_open())
	{
		std::string fLine;
		ShaderType type = NONE;

		while (getline(ShaderFile, fLine))
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
					VertexString += std::move(fLine) + "\n";
					break;

				case FRAGMENT:
					FragString += std::move(fLine) + "\n";
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

bool Shader::CreateShader(const std::string& filepath)
{
	std::string VertexString("");
	std::string FragString("");
	ReadShader_File(filepath, VertexString, FragString);
	//create a shader program that links together the vertex/frag shaders
	GLcall(mShaderProgram = glCreateProgram());


	unsigned int VertexShader = CompileShader(VertexString, GL_VERTEX_SHADER);
	unsigned int FragShader = CompileShader(FragString, GL_FRAGMENT_SHADER);

	// links together the vertex/frag shaders
	GLcall(glAttachShader(mShaderProgram, VertexShader));
	GLcall(glAttachShader(mShaderProgram, FragShader));
	GLcall(glLinkProgram(mShaderProgram));
	//checks to see whether the executables contained in program can execute given the current OpenGL state
	GLcall(glValidateProgram(mShaderProgram));

	//Delete intermidiates Shaders 
	GLcall(glDeleteShader(VertexShader));
	GLcall(glDeleteShader(FragShader));
	//Testing if everything went right
	int result;
	GLcall(glGetProgramiv(mShaderProgram, GL_VALIDATE_STATUS, &result));
	if (!result) 
	{
		int length;
		GLcall(glGetProgramiv(mShaderProgram, GL_INFO_LOG_LENGTH, &length));
		char *message = (char *)alloca(length * sizeof(char));  //Alloca is a c fuction use for alloc dinamically on stack
		GLcall(glGetProgramInfoLog(mShaderProgram, length, &length, message));
		
		std::cout << "Failed to create Program" << std::endl;
		return false;
	}

	return true;
}

unsigned int Shader::CompileShader(const std::string& source, const unsigned int ShaderType) 
{
	// Create a shader of the specified type
	GLcall(unsigned int id = glCreateShader(ShaderType));
	const char* C_str = source.c_str();

	// Set the source characters and try to compile
	GLcall(glShaderSource(id, 1, &C_str, nullptr));
	GLcall(glCompileShader(id));

	//Testing if everything went right
	int result;
	GLcall(glGetShaderiv(id, GL_COMPILE_STATUS, &result));
	if (!result)
	{
		int length;
		GLcall(glGetShaderiv(id, GL_INFO_LOG_LENGTH, &length));
		char *message = (char *)alloca(length * sizeof(char));  //Alloca is a c fuction use for alloc dinamically on stack
		GLcall(glGetShaderInfoLog(id, length, &length, message));

		std::cout << "Failed to compailed " <<(ShaderType == GL_VERTEX_SHADER ? "vertex" : "fragment" ) << "shader!" << std::endl;
		std::cout << message << std::endl;
	}

	return id;
}
