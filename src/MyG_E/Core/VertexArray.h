#pragma once

#include "VertexBuffer.h"
#include "Foundation/Gldebug.h"
#include "Foundation\Uncopyable.h"


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

	void AddBuffer(const VertexBuffer& vb);
	void PushLayout(unsigned int num_elem, unsigned int type = GL_FLOAT, bool normalized = false, const unsigned int offset = 0);
	void bind() const;
	void unbind() const;

	inline void EnabelVertexArray() { GLcall(glEnableVertexAttribArray(m_vertex_array)); }
	inline std::vector<VertexLayout> GetVertexLayouts() { return m_layout_vector; }
	inline unsigned int GetVertexArray() const { return m_vertex_array; }
	

private:

	unsigned int m_vertex_array; 
	std::vector<VertexLayout> m_layout_vector;
};
