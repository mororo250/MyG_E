#pragma once

#include "Foundation\Uncopyable.h"

class IndexBuffer : public Uncopyable
{
public:
	explicit IndexBuffer(unsigned int const *indices, unsigned int const num_indices);
	~IndexBuffer();

	void bind() const;
	void unbind() const;

	inline unsigned int GetNumIndex() const { return m_num_index; }

private:
	unsigned int m_index_buffer;	//OpenGl Id of Index Buffer
	unsigned int m_num_index;
};
