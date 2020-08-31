#include "TextureManager.h"

TextureManager* TextureManager::mInstance = nullptr;

void TextureManager::Create()
{
	if (mInstance == nullptr)
		mInstance = new TextureManager;
}

void TextureManager::Destroy()
{
	delete mInstance;
	mInstance = nullptr;
}

TextureManager* TextureManager::GetInstance()
{
	return mInstance;
}

void TextureManager::LoadTexture(std::string key, std::string path)
{
	assert(mTextureMap.find(key) == mTextureMap.end() && "Texture key existed");
	Image image = LoadImage(path.c_str());
	Texture* texture = new Texture(LoadTextureFromImage(image));
	mTexturePtrs.push_back(texture);
	mTextureMap[key] = texture;
}

Texture* TextureManager::Get(std::string key)
{
	if (mTextureMap.find(key) == mTextureMap.end()) return nullptr;
	return mTextureMap[key];
}

void TextureManager::Set(std::string key, Texture* texture)
{
	if (mTextureMap.find(key) == mTextureMap.end()) {
		mTextureMap[key] = texture;
	}
}

TextureManager::TextureManager() {}

TextureManager::~TextureManager()
{
	for (Texture* texture : mTexturePtrs)
	{
		delete texture;
	}
}
