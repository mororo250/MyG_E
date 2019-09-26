#pragma once

#include "Foundation\Math\Vector.h"
#include "Foundation\Math\Matrix.h"

class Shader
{
public: 

	Shader(const std::string& filepath);
	~Shader();

	void bind() const;
	void unbind() const;
	void set_uniform1i(int location, int value);
	void set_uniform1f(int location, float value);
	void set_uniform3f(int location, float v1, float v2, float v3);
	void set_uniform4f(int location, float v1, float v2, float v3, float v4);

	void set_uniform3f(int location, const Vector<float, 3>& vector3);
	void set_uniform4f(int location, const Vector<float, 4>& vector4);

	void set_uniformMatrix3f(int location, const Matrix< float, 3, 3>& matrix);
	void set_uniformMatrix4f(int location, const Matrix< float, 4, 4>& matrix);
	int GetUniformLocation(const std::string& name) const;

	inline unsigned int GetProgram()const { return m_shader_program; }

private:
	enum shader_type { NONE, VERTEX, FRAGMENT };

	void ReadShader_File(const std::string& filepath, std::string& vertex_string, std::string& frag_string);
	bool CreateShader(const std::string& filepath);
	unsigned int CompileShader(const std::string& source, const unsigned int shader_type);
	
	unsigned int m_shader_program;

};