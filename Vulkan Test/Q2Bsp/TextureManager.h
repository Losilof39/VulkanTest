#pragma once

#include <string>
#include <vector>
#include "GameTexture.h"

class TextureManager
{
public:

	static TextureManager* GetInstance();
	void LoadPalette();

	GameTexture* LoadTextureWal(std::string filename);
	//CreateDescriptor

	std::vector<GameTexture> gameTextures;
	//std::vector<vkDescriptorSet> DescriptorSets
	
private:
	unsigned int mPalette[256];
};

