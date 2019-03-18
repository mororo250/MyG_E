#pragma once

enum class BufferUsage
{
	STATIC, DYNAMIC
};

class VertexBuffer
{
public:
	VertexBuffer(const float *verts, const unsigned int numVertex, const unsigned int Vete, BufferUsage usage = BufferUsage::STATIC);
	~VertexBuffer();
	
	template<class T>
	void map(T*& Buffer)
	{
		bind();
		GLcall(Buffer = static_cast<T*>(glMapBuffer(GL_ARRAY_BUFFER, GL_WRITE_ONLY)));
	}
	void unmap() const;
	void bind() const;
	void unbind() const;

	inline unsigned int GetStride() const { return mStride; }

private:
	unsigned int mVertexBuffer;	//OpenGl Id of Index Buffer
	GLsizeiptr mStride; //Amount of bytes from the beginning of one element to the beginning of the following element in this case the amount of floats...
};
