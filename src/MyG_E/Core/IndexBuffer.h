#pragma once

class IndexBuffer
{
public:
	IndexBuffer(const unsigned int *indices, const unsigned int numIndices);
	~IndexBuffer();

	void bind() const;
	void unbind() const;

	inline unsigned int GetNumIndex() const { return mNumIndex; }

private:
	unsigned int mIndexBuffer;	//OpenGl Id of Index Buffer
	unsigned int mNumIndex;
};