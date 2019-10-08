#pragma once

#include "Renderer2D.h"
#include "Sprite.h"

template <class T>
class BatchRenderer
{
public:
	BatchRenderer(unsigned int max_sprites, unsigned int indices_count, unsigned int vertex_count)//indices_count - Number of indices per sprite / vertex_count - Number of Vertex per sprite  
		: m_max_sprites(max_sprites)
		, m_vertex_size(sizeof(T))
		, m_indices_size(max_sprites * indices_count)
		, m_sprite_size(m_vertex_size * vertex_count)
		, m_buffer_size(max_sprites * m_sprite_size)
		, m_renderer()
	{
		GLcall(glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA));
		m_vao = std::make_unique<VertexArray>();
		m_vb = std::make_unique<VertexBuffer>(nullptr, vertex_count * max_sprites, m_vertex_size / sizeof(float), BufferUsage::DYNAMIC);

		m_vao->PushLayout(2, GL_FLOAT, GL_FALSE, 0);
		m_vao->PushLayout(4, GL_FLOAT, GL_FALSE, 2);
		m_vao->AddBuffer(*m_vb);


		unsigned int* indices = new unsigned int[m_indices_size];

		int offset = 0;
		for (unsigned int i = 0; i < m_indices_size; i += 6)
		{
			indices[i] = offset + 0;
			indices[i + 1] = offset + 1;
			indices[i + 2] = offset + 2;

			indices[i + 3] = offset + 2;
			indices[i + 4] = offset + 3;
			indices[i + 5] = offset + 0;

			offset += 4;
		}

		m_ib = std::make_unique<IndexBuffer>(indices, m_indices_size);
	}
	~BatchRenderer()
	{}

	bool isEnoughRoom(unsigned int num_sprites) const { return num_sprites <= m_max_sprites; }
	//int getPriority() const;

	void begin()
	{
		m_vb->map<T>(m_buffer);
		
	}
	void end()
	{
		m_vb->unmap();
	}

	void add(std::vector<T>& sprites)
	{
		std::size_t size = sprites.size();
		if (size > m_buffer_size)
			std::cout << "not enough room" << std::endl;
		else
		{
			std::memcpy(m_buffer, sprites.data(), sizeof(T) * size);
			m_buffer += size;
		}
	}
	void add(void* sprites, unsigned int size)
	{
		std::memcpy(m_buffer, sprites, sizeof(T) * size);
		m_buffer += size;
	}
	void Render()
	{
		m_renderer->Clear();
		m_vao->bind();
		m_ib->bind();
		m_renderer->Draw(*m_ib);
		m_vao->unbind();
		m_ib->unbind();
	}

private:
	const unsigned int m_max_sprites;
	const unsigned int m_vertex_size;
	const unsigned int m_indices_size;
	const unsigned int m_sprite_size;
	const unsigned int m_buffer_size;
	
	
	std::unique_ptr<VertexArray> m_vao;
	std::unique_ptr<VertexBuffer> m_vb;
	std::unique_ptr<IndexBuffer> m_ib;
	std::unique_ptr<Renderer2D> m_renderer;
	T* m_buffer;
};