#include "Q2BspLoader.h"
#include "Q2BspMap.h"

void Q2BspLoader::CalcSurfaceExtents(const Q2BspMap* q2map, msurface_t* s)
{
	float	mins[2], maxs[2], val;
	int		i, j, e;
	point3f v;
	bsp_texinfo tex;
	int		bmins[2], bmaxs[2];

	mins[0] = mins[1] = 999999;
	maxs[0] = maxs[1] = -99999;

	tex = q2map->mTextures[s->texture_info];

	for (i = 0; i < s->num_edges; i++)
	{
		e = q2map->mFaceEdgetable[s->first_edge + i];
		if (e >= 0)
			v = q2map->mVertices[q2map->mEdges[e].v[0]];
		else
			v = q2map->mVertices[q2map->mEdges[-e].v[1]];

		for (j = 0; j < 2; j++)
		{
			val = v.x * tex.vecs[j][0] +
				v.y * tex.vecs[j][1] +
				v.z * tex.vecs[j][2] +
				tex.vecs[j][3];
			if (val < mins[j])
				mins[j] = val;
			if (val > maxs[j])
				maxs[j] = val;
		}
	}

	for (i = 0; i < 2; i++)
	{
		bmins[i] = std::floor(mins[i] / 16);
		bmaxs[i] = std::ceil(maxs[i] / 16);

		s->textureMins[i] = bmins[i] * 16;
		s->uvExtents[i] = (bmaxs[i] - bmins[i]) * 16;
	}
}

void Q2BspLoader::LoadFaceLump(Q2BspMap* map, std::fstream& file, std::vector<msurface_t>& arr, BSP_LUMP lump)
{
	unsigned int countElements = map->mHeader.lump[lump].length / sizeof(bsp_face);
	unsigned int offset = map->mHeader.lump[lump].offset;
	msurface_t surf = {};

	arr.reserve(countElements);
	file.seekg(offset);

	for (unsigned int i = 0; i < countElements; i++)
	{
		bsp_face face = {};
		file.read((char*)&face, sizeof(bsp_face));

		surf.first_edge = face.first_edge;
		surf.num_edges = face.num_edges;
		surf.plane = face.plane;
		surf.plane_side = face.plane_side;
		surf.texture_info = face.texture_info;
		std::memcpy(&surf.lightmap_styles, &face.lightmap_styles, sizeof(surf.lightmap_styles));
		surf.lightmap_offset = face.lightmap_offset;

		CalcSurfaceExtents(map, &surf);

		if (surf.lightmap_offset == -1)
			surf.samples = NULL;
		else
			surf.samples = map->lightmapData.data() + surf.lightmap_offset;

		arr.emplace_back(surf);
	}
}

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
		LoadBSPLump(q2map, bsp_file, q2map->lightmapData, BSP_LUMP::LIGHTMAPS);
		//LoadBSPLump(q2map, bsp_file, q2map->mFaces, BSP_LUMP::FACES);
		LoadBSPLump(q2map, bsp_file, q2map->mLeaves, BSP_LUMP::LEAVES);
		LoadBSPLump(q2map, bsp_file, q2map->mLeafFacetable, BSP_LUMP::LEAFFACETABLE);
		LoadBSPLump(q2map, bsp_file, q2map->mEdges, BSP_LUMP::EDGES);
		LoadBSPLump(q2map, bsp_file, q2map->mFaceEdgetable, BSP_LUMP::EDGEFACETABLE);
		LoadFaceLump(q2map, bsp_file, q2map->mSurfaces, BSP_LUMP::FACES);

		bsp_file.close();
	}


	return q2map;
}