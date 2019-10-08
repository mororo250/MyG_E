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
	void set_uniform1i(int location, int value) const;
	void set_uniform1f(int location, float value) const;
	void set_uniform3f(int location, float v1, float v2, float v3) const;
	void set_uniform4f(int location, float v1, float v2, float v3, float v4) const;

	void set_uniform3f(int location, Vector3f const& vector3) const;
	void set_uniform4f(int location, Vector<float, 4> const& vector4) const;

	void set_uniformMatrix3f(int location, Matrix< float, 3, 3> const& matrix) const;
	void set_uniformMatrix4f(int location, Matrix< float, 4, 4> const& matrix) const;
	int get_uniform_location(std::string const& name) const;

	inline unsigned int get_program() const { return m_shader_program; }

private:
	enum shader_type { NONE, VERTEX, FRAGMENT };

	void read_shader_file(std::string const& filepath, std::string& vertex_string, std::string& frag_string);
	bool create_shader(std::string const& filepath);
	unsigned int compile_shader(std::string const& source, unsigned int const shader_type);
	
	unsigned int m_shader_program;

};