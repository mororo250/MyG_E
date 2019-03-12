#pragma once

class VertexBuffer
{
public:
	VertexBuffer(const float *verts, const unsigned int numVertex, const unsigned int Stride);
	~VertexBuffer();
	
	void bind() const;
	void unbind() const;

	inline unsigned int GetStride() const { return mStride; }

private:
	unsigned int mVertexBuffer;	//OpenGl Id of Index Buffer
	unsigned int mStride;
};
