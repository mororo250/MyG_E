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

// Still need to check assimp documentation for more flags...
static constexpr unsigned int mesh_import_flags =
aiProcess_CalcTangentSpace |        // Gen binormals/tangents if required.
aiProcess_Triangulate |             // Use triangles.
aiProcess_SortByPType |             // Split meshes by primitive type.
aiProcess_GenNormals |              // Gen normals if required.
aiProcess_GenUVCoords |             // Gen UVs if required. 
aiProcess_ValidateDataStructure;    // Validation.

Model3D::Model3D(Material* material, std::string const& file_path)
	: m_position( 0.0f, 0.0f, 0.0f )
	, m_scale(1.0f, 1.0f, 1.0f)
	, m_trans_matrix(Matrix4x4f::make_translation_matrix(m_position))
	, m_scale_matrix(Matrix4x4f::make_scale_matrix(m_scale))
	, m_rotation_matrix(Matrix4x4f::make_identity())
	, m_file_path(file_path)
	, m_is_visible(true)
{
	load_model();
	set_material(material);
	m_object_name = "Object " + std::to_string(m_number_of_objects);
	m_number_of_objects++;
}

Model3D::Model3D(Material* material, std::string const& name, std::string const& file_path)
	: m_position( 0.0f, 0.0f, 0.0f )
	, m_scale(1.0f, 1.0f, 1.0f)
	, m_trans_matrix(Matrix4x4f::make_translation_matrix(m_position))
	, m_scale_matrix(Matrix4x4f::make_scale_matrix(m_scale))
	, m_rotation_matrix(Matrix4x4f::make_identity())
	, m_object_name(name)
	, m_file_path(file_path)
	, m_is_visible(true)
{
	load_model();
	set_material(material);
}

Model3D::Model3D(std::vector<Mesh*> const& meshes, Material* material, std::string const& file_path)
	: m_position( 0.0f, 0.0f, 0.0f )
	, m_scale(1.0f, 1.0f, 1.0f)
	, m_trans_matrix(Matrix4x4f::make_translation_matrix(m_position))
	, m_scale_matrix(Matrix4x4f::make_scale_matrix(m_scale))
	, m_rotation_matrix(Matrix4x4f::make_identity())
	, m_file_path(file_path)
	, m_is_visible(true)
{
	m_meshes.reserve(meshes.size());
	for (auto& aux : meshes)
		m_meshes.emplace_back(new Mesh(*aux));
	
	set_material(material);
	m_object_name = "Object " + std::to_string(m_number_of_objects);
	m_number_of_objects++;
}

Model3D::Model3D(std::vector<Mesh*>&& meshes, Material* material, std::string const& file_path)
	: m_position( 0.0f, 0.0f, 0.0f )
	, m_scale(1.0f, 1.0f, 1.0f)
	, m_trans_matrix(Matrix4x4f::make_translation_matrix(m_position))
	, m_scale_matrix(Matrix4x4f::make_scale_matrix(m_scale))
	, m_rotation_matrix(Matrix4x4f::make_identity())
	, m_meshes(std::move(meshes))
	, m_file_path(file_path)
	, m_is_visible(true)
{
	set_material(material);
	m_object_name = "Object " + std::to_string(m_number_of_objects);
	m_number_of_objects++;
}

Model3D::Model3D(std::vector<Mesh*> const& meshes, Material* material, std::string const& name, std::string const& file_path)
	: m_position( 0.0f, 0.0f, 0.0f )
	, m_scale(1.0f, 1.0f, 1.0f)
	, m_trans_matrix(Matrix4x4f::make_translation_matrix(m_position))
	, m_scale_matrix(Matrix4x4f::make_scale_matrix(m_scale))
	, m_rotation_matrix(Matrix4x4f::make_identity())
	, m_object_name(name)
	, m_file_path(file_path)
	, m_is_visible(true)
{
	set_material(material);
	m_meshes.reserve(meshes.size());
	for (auto& aux : meshes)
		m_meshes.emplace_back(new Mesh(*aux));
}

Model3D::Model3D(std::vector<Mesh*>&& meshes, Material* material, std::string const& name, std::string const& file_path)
	: m_position( 0.0f, 0.0f, 0.0f )
	, m_scale(1.0f, 1.0f, 1.0f)
	, m_trans_matrix(Matrix4x4f::make_translation_matrix(m_position))
	, m_scale_matrix(Matrix4x4f::make_scale_matrix(m_scale))
	, m_rotation_matrix(Matrix4x4f::make_identity())
	, m_meshes(std::move(meshes))
	, m_object_name(name)
	, m_file_path(file_path)
	, m_is_visible(true)
{
	set_material(material);
}

Model3D::Model3D(Model3D const& other)
	: m_trans_matrix(Matrix4x4f::make_translation_matrix(Vector3f(0.0f, 0.0f, 0.0f)))
	, m_scale_matrix(Matrix4x4f::make_scale_matrix(Vector3f(1.0f, 1.0f, 1.0f)))
	, m_rotation_matrix(Matrix4x4f::make_identity())
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
	: m_trans_matrix(Matrix4x4f::make_translation_matrix(Vector3f(0.0f, 0.0f, 0.0f)))
	, m_scale_matrix(Matrix4x4f::make_scale_matrix(Vector3f(1.0f, 1.0f, 1.0f)))
	, m_rotation_matrix(Matrix4x4f::make_identity())
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
	m_trans_matrix.set_tran_vec(trans);
}

void Model3D::set_scale(Vector3f const& scale)
{
	m_scale = scale;
	m_scale_matrix.set_scale_vec(scale);
}

void Model3D::set_rotation(Vector3f const& rotation)
{
	m_rotate = rotation;
	Quaternion quatx = Quaternion::make_rotate(m_rotate[0], { 1.0f, 0.0f, 0.0f });
	Quaternion quaty = Quaternion::make_rotate(m_rotate[1], { 0.0f, 1.0f, 0.0f });
	Quaternion quatz = Quaternion::make_rotate(m_rotate[2], { 0.0f, 0.0f, 1.0f });
	Quaternion quat = quatx * quaty * quatz;

	m_rotation_matrix = Quaternion::make_rotation_matrix(quat);
}

inline void Model3D::set_material(Material* material)
{
	// Create white material if material is null
	if (material)
		m_material.reset(material);
	else
		m_material.reset(new Material());
}

void Model3D::imgui_renderer()
{
	ImGui::DragFloat3("Translate", &m_position[0], 0.1f);
	ImGui::DragFloat3("Scale", &m_scale[0], 0.1f);
	ImGui::DragFloat3("Rotate", &m_rotate[0], 0.05f, -6.28f, 6.28f);
	ImGui::Separator();
	
	m_material->imgui_renderer();
	ImGui::Separator();
	
	ImGui::Checkbox("Visibility", &m_is_visible);

	set_translation(m_position);
	set_scale(m_scale);
	set_rotation(m_rotate);
}

void Model3D::load_model()
{
	Assimp::Importer import;
	const aiScene* scene = import.ReadFile(m_file_path, mesh_import_flags);

	if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode)
	{
		std::cout << "ERROR::ASSIMP::" << import.GetErrorString() << std::endl;
		throw;
	}

	process_assimp_node(scene->mRootNode, scene);
}

void Model3D::process_assimp_node(aiNode* node, aiScene const* scene)
{
	// process all the node's meshes (if any)
	m_meshes.reserve(node->mNumMeshes);
	for (unsigned int i = 0; i < node->mNumMeshes; i++)
	{
		aiMesh* mesh = scene->mMeshes[node->mMeshes[i]];
		m_meshes.emplace_back(process_mesh(mesh, scene));
	}
	// then do the same for each of its children
	for (unsigned int i = 0; i < node->mNumChildren; i++)
	{
		process_assimp_node(node->mChildren[i], scene);
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
		if (mesh->HasTangentsAndBitangents())
		{
			vertices[i].tangent = mesh->mTangents[i];
			vertices[i].bitagent = mesh->mBitangents[i];
		}
		if (mesh->HasTextureCoords(0))
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
	m_material.reset(new Material(*other.m_material));

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
	m_material.reset(other.m_material.release());

	// matrices
	set_translation(other.m_position);
	set_scale(other.m_scale);
	set_rotation(other.m_rotate);

	m_object_name = other.m_object_name;
	m_is_visible = other.m_is_visible;

	m_number_of_objects++;
}
