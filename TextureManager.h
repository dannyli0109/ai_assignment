#pragma once
#include <unordered_map>
#include "raylib.h"
#include <assert.h>
#include <vector>
#include <string>

class TextureManager
{
public:
	static void Create();
	static void Destroy();
	static TextureManager* GetInstance();
	void LoadTexture(std::string, std::string path);
	Texture* Get(std::string key);
	void Set(std::string key, Texture* texture);


private:
	TextureManager();
	~TextureManager();
	static TextureManager* mInstance;
	std::unordered_map<std::string, Texture*> mTextureMap;
	std::vector<Texture*> mTexturePtrs;
};