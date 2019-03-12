#include <iostream>
#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include "Gldebug.h"
#include "VertexBuffer.h"

VertexBuffer::VertexBuffer(const float *verts, const unsigned int NumVertex, const unsigned int Stride)
:mStride(Stride)
{
	GLcall(glGenBuffers(1, &mVertexBuffer));
	GLcall(glBindBuffer(GL_ARRAY_BUFFER, mVertexBuffer));
	// Pass the Vertex data into the buffer
	GLcall(glBufferData(GL_ARRAY_BUFFER, NumVertex * mStride * sizeof(float), verts, GL_STATIC_DRAW));
}

VertexBuffer::~VertexBuffer()
{
	GLcall(glDeleteBuffers(1, &mVertexBuffer));
}

void VertexBuffer::bind() const
{
	GLcall(glBindBuffer(GL_ARRAY_BUFFER, mVertexBuffer));
}

void VertexBuffer::unbind() const
{
	GLcall(glBindBuffer(GL_ARRAY_BUFFER, 0));
}
