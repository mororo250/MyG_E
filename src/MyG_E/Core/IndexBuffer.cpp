#include "IndexBuffer.h"

#include <glad/glad.h>

#include "Foundation/Gldebug.h"

IndexBuffer::IndexBuffer(const unsigned int *indices, const unsigned int num_indices)
:m_num_index(num_indices)
{
	GLcall(glGenBuffers(1, &m_index_buffer));
	GLcall(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_index_buffer));
	// Pass the index data into the buffer
	GLcall(glBufferData(GL_ELEMENT_ARRAY_BUFFER, num_indices * sizeof(unsigned int), indices, GL_STATIC_DRAW));
}

IndexBuffer::~IndexBuffer()
{
	GLcall(glDeleteBuffers(1, &m_index_buffer));
}

void IndexBuffer::bind() const
{
	GLcall(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_index_buffer));
}

void IndexBuffer::unbind() const
{
	GLcall(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0));
}
