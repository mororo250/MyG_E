#include <iostream>
#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "Foundation/Gldebug.h"
#include "VertexBuffer.h"

VertexBuffer::VertexBuffer(const float *verts, const unsigned int NumVertex, const unsigned int Stride, BufferUsage usage)
:mStride(Stride)
{
	GLcall(glGenBuffers(1, &mVertexBuffer));
	GLcall(glBindBuffer(GL_ARRAY_BUFFER, mVertexBuffer));
	// Pass the Vertex data into the buffer
	switch (usage)
	{
	case BufferUsage::STATIC:
		GLcall(glBufferData(GL_ARRAY_BUFFER, NumVertex * mStride * sizeof(float), verts, GL_STATIC_DRAW));
		break;

	case BufferUsage::DYNAMIC:
		GLcall(glBufferData(GL_ARRAY_BUFFER, NumVertex * mStride * sizeof(float), verts, GL_DYNAMIC_DRAW));
		break;
	}
}

VertexBuffer::~VertexBuffer()
{
	GLcall(glDeleteBuffers(1, &mVertexBuffer));
}

void VertexBuffer::unmap() const
{
	if (!glUnmapBuffer(GL_ARRAY_BUFFER))
		std::cout << "The data store contents have become corrupt" << std::endl;
}

void VertexBuffer::bind() const
{
	GLcall(glBindBuffer(GL_ARRAY_BUFFER, mVertexBuffer));
}

void VertexBuffer::unbind() const
{
	GLcall(glBindBuffer(GL_ARRAY_BUFFER, 0));
}
