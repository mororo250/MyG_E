#pragma once

#include "Foundation\Uncopyable.h"

#include "glad/glad.h"


enum class BufferUsage
{
	STATIC, DYNAMIC
};

class VertexBuffer : public Uncopyable
{
public:
	VertexBuffer(const float *verts, const unsigned int num_vertex, const unsigned int vetex, BufferUsage usage = BufferUsage::STATIC);
	~VertexBuffer();
	
	template<class T>
	void map(T*& buffer)
	{
		bind();
		GLcall(buffer = static_cast<T*>(glMapBuffer(GL_ARRAY_BUFFER, GL_WRITE_ONLY)));
	}
	void unmap() const;
	void bind() const;
	void unbind() const;

	inline unsigned int GetStride() const { return m_stride; }

private:
	unsigned int m_vertex_buffer;	//OpenGl Id of Index Buffer
	unsigned int m_stride; //Amount of bytes from the beginning of one element to the beginning of the following element in this case the amount of floats...
};
