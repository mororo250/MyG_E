#pragma once

#include <string>

class Texture
{
public:
	Texture(const std::string& FilePath);
	~Texture();

	void bind(unsigned int slot = 0) const;
	void unbind() const;
	
	inline int GetWidth() const { return mWidth; } 
	inline int GetHeight() const { return mHeight; }
private:
	unsigned int mTexture;
	std::string mFilePath;
	unsigned char *mLocalBuffer;
	int mWidth, mHeight, mBPP;
};
