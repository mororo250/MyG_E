#pragma once

enum class BufferUsage
{
	STATIC, DYNAMIC
};

class VertexBuffer
{
public:
	VertexBuffer(const float *verts, const unsigned int numVertex, const unsigned int Stride, BufferUsage usage = BufferUsage::STATIC);
	~VertexBuffer();
	
	void bind() const;
	void unbind() const;

	inline unsigned int GetStride() const { return mStride; }

private:
	unsigned int mVertexBuffer;	//OpenGl Id of Index Buffer
	unsigned int mStride; //Amount of bytes from the beginning of one element to the beginning of the following element in this case the amount of floats...
};
