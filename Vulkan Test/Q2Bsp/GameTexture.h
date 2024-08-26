#pragma once
#include <string>
#include <vector>

class GameTexture
{
public:

	GameTexture(std::string filename, unsigned int width, unsigned int height);

	std::string filename;
	std::vector<unsigned> mData;
	unsigned int width;
	unsigned int height;
	float vecs[2][4]; // [s/t][xyz offset]
	// format
	// vkTexture
	// vkSampler
	// Descriptor Set
};