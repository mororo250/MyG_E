#pragma once

#include "Core\Mesh.h"
#include "Foundation\Math\Matrix.h"
#include "Core\Material.h"

// write copy contructor
class Model3D
{
public:
	Model3D(Material* material, std::string const& file_path);
	Model3D(Material* material, std::string const& name, std::string const& file_path);
	Model3D(std::vector<Mesh*> const& mesh, Material* material, std::string const& file_path);
	Model3D(std::vector<Mesh*>&& mesh, Material* material, std::string const& file_path);
	Model3D(std::vector<Mesh*> const& mesh, Material* material, std::string const& name, std::string const& file_path);
	Model3D(std::vector<Mesh*>&& mesh, Material* material, std::string const& name, std::string const& file_path);

	// copy
	Model3D(Model3D const& other);
	Model3D& operator=(Model3D const& other);

	// move
	Model3D(Model3D&& other);
	Model3D& operator=(Model3D&& other);
	
	~Model3D();

	inline Vector3f get_position() const { return m_position; };
	inline void set_position(Vector3f const& pos) { set_translation(pos); }

	inline TranslationMatrix4 get_translation() const { return m_trans_matrix; }
	void set_translation(Vector3f const& trans);

	inline ScaleMatrix4 get_scale_matrix() const { return m_scale_matrix; }
	inline Vector3f get_scale() const { return m_scale; }
	void set_scale(Vector3f const& Scale);

	inline Matrix<float, 4, 4> get_rotation_matrix() { return m_rotation_matrix; }
	inline Vector3f get_rotation() { return m_rotate; };
	void set_rotation(Vector3f const& Rotation);

	inline std::vector<Mesh*> const* const get_meshes() { return &m_meshes; }

	inline Material const* get_material() const { return m_material.get(); }
	inline void set_material(Material* material);

	inline std::string get_path() const { return m_file_path; }

	inline void rename_object(std::string const& name) { m_object_name = name; }
	inline std::string get_name() const { return m_object_name; }

	inline void change_visibility() { m_is_visible = !m_is_visible; }
	inline void set_visibility(bool is_visible) { m_is_visible = is_visible; }
	inline bool is_visible() { return m_is_visible; }

	void ImGuiRenderer();

	static unsigned int get_number_of_object() { return m_number_of_objects; } 

private:
	void load_model();
	void process_assimp_node(struct aiNode* node, struct aiScene const* scene);
	Mesh* process_mesh(struct aiMesh* mesh, struct aiScene const* scene);

	void copy_other(Model3D const& other);
	void move_other(Model3D&& other);

	std::vector<Mesh*> m_meshes;
	std::unique_ptr<Material> m_material;

	Vector3f m_position;
	Vector3f m_scale;
	Vector3f m_rotate;

	//matrices
	TranslationMatrix4 m_trans_matrix;
	ScaleMatrix4 m_scale_matrix;
	Matrix<float, 4, 4> m_rotation_matrix;
	
	std::string m_file_path;
	std::string m_object_name;
	bool m_is_visible;

	static unsigned int m_number_of_objects;
};
