#include "BatchRenderer.h"

BatchRenderer::BatchRenderer(unsigned int MaxSprites, unsigned int IndicesCount, unsigned int VertexCount)
	:mMaxSprites(MaxSprites), 
	mVertexSize(sizeof(VertexData2C)),
	mIndicesSize(MaxSprites * IndicesCount), 
	mSpriteSize(mVertexSize * VertexCount),
	mBufferSize(MaxSprites * mSpriteSize),
	mRenderer()
{
	GLcall(glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA));
	mVao = std::make_unique<VertexArray>();
	mVb = std::make_unique<VertexBuffer>(nullptr, VertexCount * MaxSprites, mVertexSize/sizeof(float), BufferUsage::DYNAMIC);
	
	mVao->PushLayout(2, GL_FLOAT, GL_FALSE, 0);
	mVao->PushLayout(4, GL_FLOAT, GL_FALSE, 2);
	mVao->AddBuffer(*mVb);


	unsigned int* indices = new unsigned int[mIndicesSize];

	int offset = 0;
	for (unsigned int i = 0; i < mIndicesSize; i += 6)
	{
		indices[i] = offset + 0;
		indices[i + 1] = offset + 1;
		indices[i + 2] = offset + 2;

		indices[i + 3] = offset + 2;
		indices[i + 4] = offset + 3;
		indices[i + 5] = offset + 0;

		offset += 4;
	}

	mIb = std::make_unique<IndexBuffer>(indices, mIndicesSize);
}

BatchRenderer::~BatchRenderer()
{
}

void BatchRenderer::begin()
{
	mVb->bind();
	GLcall(mBuffer = static_cast<VertexData2C *>(glMapBuffer(GL_ARRAY_BUFFER, GL_WRITE_ONLY)));
}

void BatchRenderer::end()
{
	if (!glUnmapBuffer(GL_ARRAY_BUFFER))
		std::cout << "The data store contents have become corrupt" << std::endl;
}

void BatchRenderer::add(std::vector<VertexData2C>& Sprites)
{
	unsigned int size = Sprites.size();
	if (size > mBufferSize)
		std::cout << "not enough room" << std::endl;
	else
	{
		memcpy(mBuffer, Sprites.data(), sizeof(VertexData2C) * size);
		mBuffer += size;
	}
}

void BatchRenderer::add(void *Sprites, unsigned int size)
{
	memcpy(mBuffer, Sprites, sizeof(VertexData2C) * size);
}


void BatchRenderer::Render(Shader& shader)
{
	mRenderer->Clear();
	mVb->bind();
	mRenderer->Draw(*mVao, *mIb, shader);
}
