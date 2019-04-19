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
	void Draw(const IndexBuffer& ib) const;
	void Clear() const;
};