#pragma once

#include<glad/glad.h>

#include "Foundation\Gldebug.h"
#include "VertexArray.h"
#include "VertexBuffer.h"
#include "IndexBuffer.h"
#include "Shader.h"

class Renderer
{
public:
	Renderer() :m_is_active(false) {}
	virtual ~Renderer() = default;
	 
	virtual void draw_element(const IndexBuffer& ib) const = 0;
	virtual void clear() const = 0;

	virtual void set_active() {}
	virtual void set_disactive() {}

	inline bool is_active() { return m_is_active; }

protected:
	bool m_is_active;
};
