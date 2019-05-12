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


class Model3D
{
public:
	Model3D(std::shared_ptr<Mesh> mesh);
	Model3D(std::shared_ptr<Mesh> mesh, const std::string& name);
	~Model3D();

	inline Vector<float, 3> GetPosition() const { return mPosition; };
	inline void SetPosition(const Vector<float, 3>& pos) { SetTranslation(pos); }

	inline TranslationMatrix4 GetTranslation() const { return mTransMatrix; }
	void SetTranslation(const Vector<float, 3>& trans);

	inline ScaleMatrix4 GetScale() const { return mScaleMatrix; }
	void SetScale(const Vector<float, 3>& Scale);

	inline Matrix<float, 4, 4> GetRotation() { return mRotationMatrix; }
	void SetRotation(const Vector<float, 3>& Rotation);

	inline Mesh* GetMesh() { return mMesh.get(); }

	inline Material GetMaterial() const { return mMaterial; }
	inline void SetMaterial(const Material& material) { mMaterial = material; };
	void SetMaterial(Vector<float, 3>& ambient, Vector<float, 3>& diffuse, Vector<float, 3>& specular, float shininess);

	inline void RenameObject(const std::string& name) { mObjectName = name; }
	inline std::string& GetObjectName() { return mObjectName; }

	inline void ChangeVisibility() { m_is_visible = !m_is_visible; }
	inline bool isVisible() { return m_is_visible; }

	void ImGuiRenderer();

	static unsigned int GetNumberofObjects() { return mNumberofObjects; }
private:
	std::shared_ptr<Mesh> mMesh;
	Material mMaterial;

	Vector<float, 3> mPosition;
	Vector<float, 3> mScale;
	Vector<float, 3> mRotate;

	//matrices
	TranslationMatrix4 mTransMatrix;
	ScaleMatrix4 mScaleMatrix;
	Matrix<float, 4, 4> mRotationMatrix;
	
	std::string mObjectName;
	bool m_is_visible;

	static unsigned int mNumberofObjects;
};