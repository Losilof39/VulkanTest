#pragma once
#include "bsp.h"
#include "TextureManager.h"
#include <string>
#include <vector>
#include <map>
#include <vulkan/vulkan.h>

struct BspVertex
{
	point3f pos;
	float texture[2];	// texture coords
	float lightmap[2];
};

struct Facebuffer
{
	unsigned int vertexCount;
	unsigned int vertexOffset;
	unsigned int indexCount;
	uint16_t indexOffset;
	int descriptor;
	//descriptor
};

class Q2BspMap
{
public:
	unsigned int FindCurrentLeaf();
	uint8_t* ClusterPVS(unsigned int cluster);
	uint8_t* DecompressVis(uint8_t* in, unsigned int numClusters);
	bool isClusterVisible(unsigned int cluster);
	void CalculateVisibleFaces();
	unsigned int PointOnPlane(const point3f& pos, const point3f& normal, float dist);
	void LoadTextures();
	void PrepareFaces();
	void RecalcVertices(const bsp_face& face, int& vertexCount, int& indexCount);
	point3f* GetCameraPos();
	void SetCameraPos(float x, float y, float z);

	void OnRender();
	void RenderFaces();


	bsp_header mHeader = {};

	std::vector <char> mEntities;
	std::vector <bsp_plane> mPlanes;
	std::vector <bsp_texinfo> mTextures;
	std::vector <point3f> mVertices;
	std::vector <bsp_vis_offset> mPVS;
	std::vector <bsp_node> mNodes;
	std::vector <bsp_face> mFaces;
	std::vector <bsp_texinfo> mLightmaps;
	std::vector <bsp_leaf> mLeaves;
	std::vector <uint16_t> mLeafFacetable;
	std::vector <bsp_edge> mEdges;
	std::vector <int32_t> mFaceEdgetable;

	std::vector<GameTexture*> gameTextures;

	std::vector<BspVertex> mFaceVertices;
	//std::vector<uint16_t> mFaceIndices;
	std::vector<Facebuffer> mFaceBuffers;
	std::vector<int> mVisibleFaces;

private:

	uint8_t palette[256];
	point3f mPos = { 128.0f , -320.0f , 64.0f };
	uint8_t* old_visdata = nullptr;
};