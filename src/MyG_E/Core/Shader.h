#pragma once

#include "Foundation\Math\Vector.h"
#include "Foundation\Math\Matrix.h"

class Shader
{
public: 

	Shader(std::string const& vert_file, std::string const& frag_file, std::string const& geom_file = "");
	~Shader();

	void bind() const;
	void unbind() const;
	void set_uniform1i(int location, int value) const;
	void set_uniform1ui(int location, unsigned int value) const;
	void set_uniform1f(int location, float value) const;
	void set_uniform2f(int location, Vector2f const& vector) const;
	void set_uniform3f(int location, Vector3f const& vector3) const;
	void set_uniform4f(int location, Vector4f const& vector4) const;
	void set_uniformMatrix3f(int location, Matrix3x3f const& matrix) const;
	void set_uniformMatrix4f(int location, Matrix4x4f const& matrix) const;

	int get_uniform_location(std::string const& name) const;

	inline unsigned int get_program() const { return m_shader_program; }

private:
	enum shader_type { NONE, VERTEX, FRAGMENT };

	bool create_shader();
	bool read_shader_file(std::string& shader_string, std::string& file_path);
	unsigned int compile_shader(std::string const& source, unsigned int const shader_type);
	
	unsigned int m_shader_program;
	std::string m_vert_file;
	std::string m_frag_file;
	std::string m_geom_file;
};
