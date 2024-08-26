#include "Q2BspLoader.h"
#include "Q2BspMap.h"

Q2BspMap* Q2BspLoader::LoadBSP(std::string filename)
{
	Q2BspMap* q2map = new Q2BspMap;

	std::fstream bsp_file(filename, std::ios::in | std::ios::binary | std::ios::ate);


	std::cout << "Opening " << filename << std::endl;

	if (bsp_file.is_open())
	{
		bsp_file.seekg(std::ios::beg);
		mBinary_stream.resize(sizeof(bsp_header));
		bsp_file.read(mBinary_stream.data(), sizeof(bsp_header));
		

		std::cout << "Reading the header.." << std::endl;

		// read the header
		std::memcpy((void*)&q2map->mHeader, mBinary_stream.data(), sizeof(bsp_header));

		std::cout << "BSP VERSION: " << q2map->mHeader.version << "\n" << std::endl;

		LoadBSPLump(q2map, bsp_file, q2map->mEntities, BSP_LUMP::ENTITIES);
		LoadBSPLump(q2map, bsp_file, q2map->mPlanes, BSP_LUMP::PLANES_T);
		LoadBSPLump(q2map, bsp_file, q2map->mTextures, BSP_LUMP::TEXTURES);
		LoadBSPLump(q2map, bsp_file, q2map->mVertices, BSP_LUMP::VERTICES);
		LoadBSPLump(q2map, bsp_file, q2map->mPVS, BSP_LUMP::VIS);
		LoadBSPLump(q2map, bsp_file, q2map->mNodes, BSP_LUMP::NODES);
		LoadBSPLump(q2map, bsp_file, q2map->mFaces, BSP_LUMP::FACES);
		LoadBSPLump(q2map, bsp_file, q2map->mLeaves, BSP_LUMP::LEAVES);
		LoadBSPLump(q2map, bsp_file, q2map->mLeafFacetable, BSP_LUMP::LEAFFACETABLE);
		LoadBSPLump(q2map, bsp_file, q2map->mEdges, BSP_LUMP::EDGES);
		LoadBSPLump(q2map, bsp_file, q2map->mFaceEdgetable, BSP_LUMP::EDGEFACETABLE);

		bsp_file.close();
	}


	return q2map;
}