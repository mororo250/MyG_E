// Heap file.
#include "Model3D.h"

// Intern files.
#include "Foundation\Math\Quaternion.h"

// Third Parties.
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include "imgui.h"

unsigned int Model3D::m_number_of_objects = 0;

Model3D::Model3D(std::vector<Mesh*> const& meshes, std::string const& file_path)
	: m_position({ 0.0f, 0.0f, 0.0f })
	, m_trans_matrix(0.0f, 0.0f, 0.0f)
	, m_scale_matrix(1.0f, 1.0f, 1.0f)
	, m_scale({ 1.0f, 1.0f, 1.0f })
	, m_rotation_matrix(Matrix<float, 4, 4>::make_identity())
	, m_file_path(file_path)
	, m_is_visible(true)
{
	m_meshes.reserve(meshes.size());
	for (auto& aux : meshes)
		m_meshes.emplace_back(new Mesh(*aux));

	m_object_name = "Object " + std::to_string(m_number_of_objects);
	m_number_of_objects++;
}

Model3D::Model3D(std::vector<Mesh*>&& meshes, std::string const& file_path)
	: m_position({ 0.0f, 0.0f, 0.0f })
	, m_trans_matrix(0.0f, 0.0f, 0.0f)
	, m_scale_matrix(1.0f, 1.0f, 1.0f)
	, m_scale({ 1.0f, 1.0f, 1.0f })
	, m_rotation_matrix(Matrix<float, 4, 4>::make_identity())
	, m_meshes(std::move(meshes))
	, m_file_path(file_path)
	, m_is_visible(true)
{
	m_object_name = "Object " + std::to_string(m_number_of_objects);
	m_number_of_objects++;
}

Model3D::Model3D(std::vector<Mesh*> const& meshes, std::string const& name, std::string const& file_path)
	: m_position({ 0.0f, 0.0f, 0.0f })
	, m_trans_matrix(0.0f, 0.0f, 0.0f)
	, m_scale_matrix(1.0f, 1.0f, 1.0f)
	, m_scale({ 1.0f, 1.0f, 1.0f })
	, m_rotation_matrix(Matrix<float, 4, 4>::make_identity())
	, m_object_name(name)
	, m_file_path(file_path)
	, m_is_visible(true)
{
	m_meshes.reserve(meshes.size());
	for (auto& aux : meshes)
		m_meshes.emplace_back(new Mesh(*aux));
}

Model3D::Model3D(std::vector<Mesh*>&& meshes, std::string const& name, std::string const& file_path)
	: m_position({ 0.0f, 0.0f, 0.0f })
	, m_trans_matrix(0.0f, 0.0f, 0.0f)
	, m_scale_matrix(1.0f, 1.0f, 1.0f)
	, m_scale({ 1.0f, 1.0f, 1.0f })
	, m_rotation_matrix(Matrix<float, 4, 4>::make_identity())
	, m_meshes(std::move(meshes))
	, m_object_name(name)
	, m_file_path(file_path)
	, m_is_visible(true)
{
}

Model3D::Model3D(Model3D const& other)
	: m_trans_matrix(0.0f, 0.0f, 0.0f)
	, m_scale_matrix(1.0f, 1.0f, 1.0f)
	, m_rotation_matrix(Matrix<float, 4, 4>::make_identity())
{
	copy_other(other);
}

Model3D& Model3D::operator=(Model3D const& other)
{
	if (this != &other)
	{
		for (auto& aux : m_meshes)
			delete aux;
		copy_other(other);
	}
	return *this;
}

Model3D::Model3D(Model3D&& other)
	: m_trans_matrix(0.0f, 0.0f, 0.0f)
	, m_scale_matrix(1.0f, 1.0f, 1.0f)
	, m_rotation_matrix(Matrix<float, 4, 4>::make_identity())
{
	move_other(std::move(other));
}

Model3D& Model3D::operator=(Model3D&& other)
{
	if (this != &other)
	{
		for (auto& aux : m_meshes)
			delete aux;
		move_other(std::move(other));
	}
	return *this;
}

Model3D::~Model3D()
{
	m_number_of_objects--;
	for (auto& aux : m_meshes)
		delete aux;
}

void Model3D::set_translation(Vector3f const& trans)
{
	m_position = trans;
	m_trans_matrix.SetTranX(m_position[0]);
	m_trans_matrix.SetTranY(m_position[1]);
	m_trans_matrix.SetTranZ(m_position[2]);
}

void Model3D::set_scale(Vector3f const& scale)
{
	m_scale = scale;
	m_scale_matrix.SetScaleX(m_scale[0]);
	m_scale_matrix.SetScaleY(m_scale[1]);
	m_scale_matrix.SetScaleZ(m_scale[2]);
}

void Model3D::set_rotation(Vector3f const& rotation)
{
	m_rotate = rotation;
	Quaternion quatx = Quaternion::MakeRotate(m_rotate[0], { 1.0f, 0.0f, 0.0f });
	Quaternion quaty = Quaternion::MakeRotate(m_rotate[1], { 0.0f, 1.0f, 0.0f });
	Quaternion quatz = Quaternion::MakeRotate(m_rotate[2], { 0.0f, 0.0f, 1.0f });
	Quaternion quat = quatx * quaty * quatz;

	m_rotation_matrix = Quaternion::CreateRotationMatrix(quat);
}

void Model3D::ImGuiRenderer()
{
	ImGui::DragFloat3("Translate", &m_position[0], 0.1f);
	ImGui::DragFloat3("Scale", &m_scale[0], 0.1f);
	ImGui::DragFloat3("Rotate", &m_rotate[0], 0.05f, -6.28f, 6.28f);
	ImGui::Separator();
	
	m_material.imgui_renderer();
	ImGui::Separator();
	
	ImGui::Checkbox("Visibility", &m_is_visible);

	set_translation(m_position);
	set_scale(m_scale);
	set_rotation(m_rotate);
}

Model3D Model3D::load_model(std::string const& file_path)
{
	Assimp::Importer import;
	const aiScene* scene = import.ReadFile(file_path, aiProcess_Triangulate | aiProcess_FlipUVs);

	if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode)
	{
		std::cout << "ERROR::ASSIMP::" << import.GetErrorString() << std::endl;
		throw;
	}

	std::vector<Mesh*> meshes;
	process_assimp_node(scene->mRootNode, scene, meshes);
	return Model3D(std::move(meshes), file_path);
}

void Model3D::process_assimp_node(aiNode* node, aiScene const* scene, std::vector<Mesh*>& meshes)
{
	// process all the node's meshes (if any)
	meshes.reserve(node->mNumMeshes);
	for (unsigned int i = 0; i < node->mNumMeshes; i++)
	{
		aiMesh* mesh = scene->mMeshes[node->mMeshes[i]];
		meshes.emplace_back(process_mesh(mesh, scene));
	}
	// then do the same for each of its children
	for (unsigned int i = 0; i < node->mNumChildren; i++)
	{
		process_assimp_node(node->mChildren[i], scene, meshes);
	}
}

Mesh* Model3D::process_mesh(aiMesh* mesh, aiScene const* scene)
{
	// Process vertices
	std::vector<vertex> vertices(mesh->mNumVertices);;
	for (unsigned int i = 0; i < vertices.capacity(); i++)
	{
		vertices[i].position = mesh->mVertices[i];
		vertices[i].normal = mesh->mNormals[i];
		if (mesh->mTextureCoords[0])
		{
			vertices[i].textcoord[0] = mesh->mTextureCoords[0][i].x;
			vertices[i].textcoord[1] = mesh->mTextureCoords[0][i].y;
		}
	}

	// Process indices
	std::vector<unsigned int> indices;
	for (unsigned int i = 0; i < mesh->mNumFaces; i++)
	{
		aiFace* face = &mesh->mFaces[i];
		indices.reserve(face->mNumIndices);
		for (unsigned int j = 0; j < face->mNumIndices; j++)
		{
			indices.emplace_back(face->mIndices[j]);
		}
	}
	return new Mesh(std::move(vertices), std::move(indices));
}

void Model3D::copy_other(Model3D const& other)
{
	m_meshes.reserve(other.m_meshes.size());
	for (auto& aux : other.m_meshes)
		m_meshes.emplace_back(new Mesh(*aux));
	m_material = other.m_material;

	//matrices
	set_translation(other.m_position);
	set_scale(other.m_scale);
	set_rotation(other.m_rotate);

	m_object_name = other.m_object_name;
	m_is_visible = other.m_is_visible;
	
	m_number_of_objects++;
}

void Model3D::move_other(Model3D&& other)
{
	m_meshes = std::move(other.m_meshes);
	m_material = other.m_material;

	// matrices
	set_translation(other.m_position);
	set_scale(other.m_scale);
	set_rotation(other.m_rotate);

	m_object_name = other.m_object_name;
	m_is_visible = other.m_is_visible;

	m_number_of_objects++;
}
