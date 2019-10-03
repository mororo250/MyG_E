#pragma once

#include "Core\Mesh.h"
#include "Foundation\Math\Matrix.h"
#include "Core\Material.h"

// write copy contructor
class Model3D
{
public:
	Model3D(Mesh* mesh);
	Model3D(Mesh* mesh, std::string& const name);
	Model3D(Model3D const& other);
	Model3D& operator=(Model3D const& other);
	~Model3D();

	inline Vector<float, 3> get_position() const { return m_position; };
	inline void set_position(Vector<float, 3> const& pos) { set_translation(pos); }

	inline TranslationMatrix4 get_translation() const { return m_trans_matrix; }
	void set_translation(Vector<float, 3> const& trans);

	inline ScaleMatrix4 get_scale_matrix() const { return m_scale_matrix; }
	inline Vector<float, 3> get_scale() const { return m_scale; }
	void set_scale(Vector<float, 3> const& Scale);

	inline Matrix<float, 4, 4> get_rotation_matrix() { return m_rotation_matrix; }
	inline Vector<float, 3> get_rotation() { return m_rotate; };
	void set_rotation(Vector<float, 3> const& Rotation);

	inline Mesh const* get_mesh() { return m_mesh; }
	inline Shape get_shape() { return m_mesh->get_shape(); }

	inline Material const& get_material() const { return m_material; }
	inline void set_material(Material const& material) { m_material = material; };

	inline void rename_object(std::string const& name) { m_object_name = name; }
	inline std::string& get_object() { return m_object_name; }

	inline void change_visibility() { m_is_visible = !m_is_visible; }
	inline void set_visibility(bool is_visible) { m_is_visible = is_visible; }
	inline bool is_visible() { return m_is_visible; }

	void ImGuiRenderer();

	static unsigned int get_number_of_object() { return m_number_of_objects; }

private:
	void copy_other(Model3D const& other);

	Mesh* m_mesh;
	Material m_material;

	Vector<float, 3> m_position;
	Vector<float, 3> m_scale;
	Vector<float, 3> m_rotate;

	//matrices
	TranslationMatrix4 m_trans_matrix;
	ScaleMatrix4 m_scale_matrix;
	Matrix<float, 4, 4> m_rotation_matrix;
	
	std::string m_object_name;
	bool m_is_visible;

	static unsigned int m_number_of_objects;
};