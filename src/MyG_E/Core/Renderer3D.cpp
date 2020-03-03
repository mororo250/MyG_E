#include "Renderer3D.h"

#include<glad/glad.h>

#include "Foundation/Gldebug.h"
#include "VertexArray.h"
#include "VertexBuffer.h"
#include "IndexBuffer.h"

bool Renderer3D::s_has_init = false;

Renderer3D::Renderer3D()
{
}

Renderer3D::~Renderer3D()
{
	end();
	clear();
}

void Renderer3D::draw_element(IndexBuffer const& ib)
{
	GLcall(glDrawElements(GL_TRIANGLES, ib.GetNumIndex(), GL_UNSIGNED_INT, nullptr));
}

void Renderer3D::draw_quad()
{
	static constexpr float quad_vertices[] = {
		// positions        // texture Coords
		-1.0f,  1.0f, 0.0f, 0.0f, 1.0f,
		-1.0f, -1.0f, 0.0f, 0.0f, 0.0f,
		 1.0f,  1.0f, 0.0f, 1.0f, 1.0f,
		 1.0f, -1.0f, 0.0f, 1.0f, 0.0f,
	};

	static constexpr unsigned int quad_index[] = {
		0, 1, 2,
		2, 1, 3
	};
	static VertexArray vertex_array;
	static VertexBuffer const vertex_buffer(quad_vertices, sizeof(quad_vertices)/sizeof(float), 5);
	static IndexBuffer const index_buffer(quad_index, sizeof(quad_index) / sizeof(float));
	
	if (vertex_array.get_vertex_layout().size() == 0) // it will run just once.
	{
		vertex_array.PushLayout(3, GL_FLOAT, false, 0);  // Position
		vertex_array.PushLayout(2, GL_FLOAT, false, 3);  // texcoord

		vertex_array.AddBuffer(vertex_buffer);
	}
	
	vertex_array.bind();
	index_buffer.bind();
	draw_element(index_buffer);
	vertex_array.unbind();
	index_buffer.unbind();
}

void Renderer3D::init()
{
	s_has_init = true;
	GLcall(glEnable(GL_DEPTH_TEST));
	GLcall(glEnable(GL_MULTISAMPLE));
}

void Renderer3D::end()
{
	s_has_init = false;
	GLcall(glDisable(GL_DEPTH_TEST));
	GLcall(glDisable(GL_MULTISAMPLE))
}

void Renderer3D::clear(Vector4f const& color)
{
	GLcall(glClearColor(color[0], color[1], color[2], color[3]));
	GLcall(glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT));
}

void Renderer3D::clear_buffer()
{
	GLcall(glClear(GL_DEPTH_BUFFER_BIT));
}
