#pragma once
#include "bsp.h"
#include "Q2BspMap.h"
#include <iostream>
#include <fstream>
#include <vector>
#include <string>

class Q2BspLoader
{
public:
	Q2BspMap* LoadBSP(std::string filename);
private:

	void LoadFaceLump(Q2BspMap* map, std::fstream& file, std::vector<msurface_t>& arr, BSP_LUMP lump);
	void CalcSurfaceExtents(const Q2BspMap* q2map, msurface_t* s);

	template<class T>
	void LoadBSPLump(Q2BspMap* map, std::fstream& file, std::vector<T>& arr, BSP_LUMP lump);

	std::vector<char> mBinary_stream = {};
	uint32_t mSize = 0;
};

template<class T>
void Q2BspLoader::LoadBSPLump(Q2BspMap* map, std::fstream& file, std::vector<T>& arr, BSP_LUMP lump)
{

	unsigned int countElements = map->mHeader.lump[lump].length / sizeof(T);
	unsigned int offset = map->mHeader.lump[lump].offset;

	arr.reserve(countElements);
	file.seekg(offset);

	for (unsigned int i = 0; i < countElements; i++)
	{
		T element = {};
		file.read((char*)&element, sizeof(T));
		arr.emplace_back(element);
	}
}