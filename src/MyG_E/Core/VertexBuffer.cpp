#include "VertexBuffer.h"

#include "glad/glad.h"

#include "Foundation/Gldebug.h"

VertexBuffer::VertexBuffer(const float *verts, const unsigned int num_vertex, const unsigned int stride, BufferUsage usage)
:m_stride(stride)
{
	GLcall(glGenBuffers(1, &m_vertex_buffer));
	GLcall(glBindBuffer(GL_ARRAY_BUFFER, m_vertex_buffer));
	// Pass the Vertex data into the buffer

	GLcall(glBufferData(GL_ARRAY_BUFFER, num_vertex * m_stride * sizeof(float), verts, usage));
}

VertexBuffer::~VertexBuffer()
{
	GLcall(glDeleteBuffers(1, &m_vertex_buffer));
}

void VertexBuffer::unmap() const
{
	if (!glUnmapBuffer(GL_ARRAY_BUFFER))
		std::cout << "The data store contents have become corrupt" << std::endl;
}

void VertexBuffer::bind() const
{
	GLcall(glBindBuffer(GL_ARRAY_BUFFER, m_vertex_buffer));
}

void VertexBuffer::unbind() const
{
	GLcall(glBindBuffer(GL_ARRAY_BUFFER, 0));
}
