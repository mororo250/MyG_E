#include <iostream>
#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "Gldebug.h"
#include "VertexArray.h"
#include "VertexBuffer.h"

VertexArray::VertexArray()
{
	GLcall(glGenVertexArrays(1, &mVertexArray));
	GLcall(glBindVertexArray(mVertexArray));
}

VertexArray::~VertexArray()
{
	GLcall(glDeleteVertexArrays(1, &mVertexArray))
}


void VertexArray::AddBuffer(const VertexBuffer& vb)
{
	vb.bind();
	for (unsigned int i = 0; i < mLayoutVector.size(); i++)
	{
		GLcall(glEnableVertexAttribArray(i));
		GLcall(glVertexAttribPointer(
			i,								//Atribute Index
			mLayoutVector[i].NumElem,		//Number of components
			mLayoutVector[i].type,			//type of the components
			mLayoutVector[i].normalized,	//Normalize a valor
			sizeof(float) * vb.GetStride(),				//Amount of bytes between two vertex (Stride) {incomplete}
			reinterpret_cast<void*>(mLayoutVector[i].offset * sizeof(float))	//offset from start of the vertex to this attribute
		));
	}
}

void VertexArray::bind() const
{
	GLcall(glBindVertexArray(mVertexArray));
}

void VertexArray::unbind() const
{
	GLcall(glBindVertexArray(mVertexArray));
}

void VertexArray::PushLayout(unsigned int NumElem, unsigned int type, bool normalized, const unsigned int offset)
{
	mLayoutVector.push_back({ NumElem, type, normalized, offset});
}
