#pragma once

#include "VertexBuffer.h"
#include "Foundation/Uncopyable.h"


struct VertexLayout
{
	unsigned int num_elem;
	unsigned int type;
	bool normalized;
	unsigned int offset;
};

class VertexArray : public Uncopyable
{
public:
	VertexArray();
	~VertexArray();

	void AddBuffer(VertexBuffer const& vb) const;
	void PushLayout(unsigned int num_elem, unsigned int type = 0x1406 , bool normalized = false, const unsigned int offset = 0); // type = GL_FLOAT
	void bind() const;
	void unbind() const;

	void EnabelVertexArray() const;
	inline std::vector<VertexLayout> get_vertex_layout() const { return m_layout_vector; }
	inline unsigned int get_vertex_array() const { return m_vertex_array; }
	

private:

	unsigned int m_vertex_array; 
	std::vector<VertexLayout> m_layout_vector;
};
