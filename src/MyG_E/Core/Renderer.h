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
	Renderer() = default;
	virtual ~Renderer() = default;
	 
	virtual void draw_element(const IndexBuffer& ib) const = 0;
	virtual void clear() const = 0;
};