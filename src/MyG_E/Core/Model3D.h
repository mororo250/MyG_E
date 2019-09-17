#pragma once

#include "Core\Mesh.h"
#include "Foundation\Math\Matrix.h"

// For while.
struct Material 
{
	Material(Vector<float, 3> amb = { 1.0f, 1.0f, 1.0f }, Vector<float, 3> dif = { 1.0f, 1.0f, 1.0f }, Vector<float, 3> spec = { 1.0f, 1.0f, 1.0f }, float shi = 32.0f)
		: ambient(amb)
		, diffuse(dif)
		, specular(spec)
		, shininess(shi)
	{}
	Vector<float, 3> ambient;
	Vector<float, 3> diffuse;
	Vector<float, 3> specular;
	float shininess;
};

// write copy contructor
class Model3D
{
public:
	Model3D(Mesh* mesh);
	Model3D(Mesh* mesh, std::string& const name);
	~Model3D();

	inline Vector<float, 3> GetPosition() const { return m_position; };
	inline void SetPosition(const Vector<float, 3>& pos) { SetTranslation(pos); }

	inline TranslationMatrix4 GetTranslation() const { return m_trans_matrix; }
	void SetTranslation(const Vector<float, 3>& trans);

	inline ScaleMatrix4 GetScale() const { return m_scale_matrix; }
	void SetScale(const Vector<float, 3>& Scale);

	inline Matrix<float, 4, 4> GetRotation() { return m_rotation_matrix; }
	void SetRotation(const Vector<float, 3>& Rotation);

	inline Mesh* GetMesh() { return m_mesh; }

	inline Material GetMaterial() const { return mMaterial; }
	inline void SetMaterial(const Material& material) { mMaterial = material; };
	void SetMaterial(Vector<float, 3>& ambient, Vector<float, 3>& diffuse, Vector<float, 3>& specular, float shininess);

	inline void RenameObject(const std::string& name) { m_object_name = name; }
	inline std::string& GetObjectName() { return m_object_name; }

	inline void ChangeVisibility() { m_is_visible = !m_is_visible; }
	inline bool isVisible() { return m_is_visible; }

	void ImGuiRenderer();

	static unsigned int GetNumberofObjects() { return m_number_of_objects; }

private:
	Mesh* m_mesh;
	Material mMaterial;

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