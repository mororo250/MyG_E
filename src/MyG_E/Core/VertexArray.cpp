#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "Foundation/Gldebug.h"
#include "VertexArray.h"
#include "VertexBuffer.h"

VertexArray::VertexArray()
{
	GLcall(glGenVertexArrays(1, &m_vertex_array));
	GLcall(glBindVertexArray(m_vertex_array));
}

VertexArray::~VertexArray()
{
	GLcall(glDeleteVertexArrays(1, &m_vertex_array))
}


void VertexArray::AddBuffer(const VertexBuffer& vb)
{
	vb.bind();
	for (unsigned int i = 0; i < m_layout_vector.size(); i++)
	{
		GLcall(glEnableVertexAttribArray(i));
		GLcall(glVertexAttribPointer(
			i,								//Atribute Index
			m_layout_vector[i].num_elem,		//Number of components
			m_layout_vector[i].type,			//type of the components
			m_layout_vector[i].normalized,	//Normalize a valor
			sizeof(float) * vb.GetStride(),				//Amount of bytes between two vertex (Stride) {incomplete}
			reinterpret_cast<void*>(m_layout_vector[i].offset * sizeof(float))	//offset from start of the vertex to this attribute
		));
	}
}

void VertexArray::bind() const
{
	GLcall(glBindVertexArray(m_vertex_array));
}

void VertexArray::unbind() const
{
	GLcall(glBindVertexArray(0));
}

void VertexArray::PushLayout(unsigned int num_elem, unsigned int type, bool normalized, const unsigned int offset)
{
	m_layout_vector.push_back({ num_elem, type, normalized, offset});
}
