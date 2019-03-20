#include <iostream>
#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "Gldebug.h"
#include "Texture.h"

#include "stb_image.h"


Texture::Texture(const std::string& FilePath)
:mTexture(0) , mFilePath(FilePath), mLocalBuffer(nullptr),
mBPP(0), mWidth(0), mHeight(0)
{
	stbi_set_flip_vertically_on_load(1); //flip the image - opengl start from the top left intead of the bottom lefet
	mLocalBuffer = stbi_load(mFilePath.c_str(), &mWidth, &mHeight, &mBPP, 4);
	
	GLcall(glGenTextures(1 , &mTexture));
	GLcall(glBindTexture(GL_TEXTURE_2D, mTexture));

	GLcall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR));
	GLcall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR));
	GLcall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE));
	GLcall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE));

	GLcall(glTexImage2D(GL_TEXTURE_2D, 0 , GL_RGBA8, mWidth, mHeight, 0, GL_RGBA, GL_UNSIGNED_BYTE, mLocalBuffer));
	GLcall(glBindTexture(GL_TEXTURE_2D, 0)); //unbind

	if (mLocalBuffer)
		stbi_image_free(mLocalBuffer);
}

Texture::~Texture()
{
	GLcall(glDeleteTextures(1, &mTexture));
}

void Texture::bind(unsigned int slot) const
{
	GLcall(glActiveTexture(GL_TEXTURE0 + slot));
	GLcall(glBindTexture(GL_TEXTURE_2D, mTexture));
}

void Texture::unbind() const
{
	GLcall(glBindTexture(GL_TEXTURE_2D, 0));
}
