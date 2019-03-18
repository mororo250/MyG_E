#include <iostream>
#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include "Gldebug.h"
#include "IndexBuffer.h"

IndexBuffer::IndexBuffer(const unsigned int *indices, const unsigned int numIndices)
:mNumIndex(numIndices)
{
	GLcall(glGenBuffers(1, &mIndexBuffer));
	GLcall(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mIndexBuffer));
	// Pass the index data into the buffer
	GLcall(glBufferData(GL_ELEMENT_ARRAY_BUFFER, numIndices * sizeof(unsigned int), indices, GL_STATIC_DRAW));
}

IndexBuffer::~IndexBuffer()
{
	GLcall(glDeleteBuffers(1, &mIndexBuffer));
}

void IndexBuffer::bind() const
{
	GLcall(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mIndexBuffer));
}

void IndexBuffer::unbind() const
{
	GLcall(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0));
}
