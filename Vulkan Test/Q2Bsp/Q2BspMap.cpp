#include "Q2BspMap.h"
#include <fstream>
#include <iostream>

unsigned int Q2BspMap::FindCurrentLeaf()
{
    int leafIndex = 0;

    while (leafIndex >= 0)
    {
        
        // children.x - front node; children.y - back node
        if (PointOnPlane(
            mPos,
            mPlanes[mNodes[leafIndex].plane].normal,
            mPlanes[mNodes[leafIndex].plane].distance) == 1)
        {
            leafIndex = mNodes[leafIndex].front_child;
        }
        else
        {
            leafIndex = mNodes[leafIndex].back_child;
        }
    }

    return ~leafIndex;
}

/*vis[cluster>>3] & (1<<(cluster&7))*/

uint8_t* Q2BspMap::ClusterPVS(unsigned int cluster)
{
    if (cluster != -1)
    {
        old_visdata = DecompressVis((uint8_t*)mPVS.data() + mPVS.data()->bitofs[cluster][DVIS_PVS], mPVS[0].numclusters);
    }

    return old_visdata;
}

// taken from vkQuake2 source code here: https://github.com/kondrak/vkQuake2/blob/d7762ebac82fcc242c177ec5deac75d8f7d4ba20/ref_vk/vk_model.c#L79

#define MAX_MAP_LEAFS 65536
uint8_t* Q2BspMap::DecompressVis(uint8_t* in, unsigned int numClusters)
{
    static uint8_t	decompressed[MAX_MAP_LEAFS / 8];
    int		c;
    uint8_t* out;
    int		row;

    row = (numClusters + 7) >> 3;
    out = decompressed;

    if (!in)
    {	// no vis info, so make all visible
        while (row)
        {
            *out++ = 0xff;
            row--;
        }
        return decompressed;
    }

    do
    {
        if (*in)
        {
            *out++ = *in++;
            continue;
        }

        c = in[1];
        in += 2;
        while (c)
        {
            *out++ = 0;
            c--;
        }
    } while (out - decompressed < row);

    return decompressed;
}

bool Q2BspMap::isClusterVisible(unsigned int cluster)
{
    return false;
}

void Q2BspMap::CalculateVisibleFaces()
{
    int             count;
    unsigned int    cameraLeaf;
    int             currentCluster;
    uint8_t*        vis;
    int             lindex;

    if (old_visdata == nullptr)
    {
        old_visdata = new uint8_t(MAX_MAP_LEAFS / 8);
    }

    count = 0;
    cameraLeaf = FindCurrentLeaf();
    currentCluster = mLeaves[cameraLeaf].cluster;
    vis = ClusterPVS(currentCluster);

    for (auto& l : mLeaves)
    {
        int cluster = l.cluster;

        if (cluster == -1)
            continue;

        if (vis[cluster >> 3] & (1 << (cluster & 7)))
        {
            count++;

            // loop through every face in the visible leaf
            for (unsigned int i = 0; i < l.num_leaf_faces; i++)
            {
                lindex = mLeafFacetable[l.first_leaf_face + i];
                mVisibleFaces.push_back(lindex);
            }
        }
    }
}

unsigned int Q2BspMap::PointOnPlane(const point3f& pos, const point3f& normal, float dist)
{
    if( (pos.x * normal.x + pos.y * normal.y + pos.z * normal.z - dist) > 0)
        return 1;
    
    return 0;
}

void Q2BspMap::LoadTextures()
{
    char filename[64] = {};
    GameTexture* temp = {};

    //TextureManager::GetInstance()->LoadPalette();
    
    for (auto& t : mTextures)
    {
        std::sprintf(filename, "assets/textures/%s.wal", t.texture_name);

        temp = TextureManager::GetInstance()->LoadTextureWal(filename);

        std::memcpy((void*)temp->vecs, (void*)t.vecs, sizeof(t.vecs));

        gameTextures.emplace_back(temp);
    }
}

void Q2BspMap::PrepareFaces()
{
    /*int         vertexCount;
    int         indexCount;
    Facebuffer  fb;

    vertexCount = 0;
    indexCount = 0;
    fb = {};

    for (auto& f : mFaces)
    {

        fb.vertexOffset = vertexCount;
        fb.indexOffset = indexCount;

        RecalcVertices(f, vertexCount, indexCount);


        fb.vertexCount = f.num_edges;
        fb.indexCount = (f.num_edges - 2) * 3;

        mFaceBuffers.push_back(fb);
    }*/
}

void Q2BspMap::RecalcVertices(const msurface_t& face, int& vertexCount, int& indexCount)
{
    BspVertex      v;
    int         startEdge;
    bsp_edge    edge;
    uint16_t    texinfo;
    int         lindex;
    int         numTriangles;

    v = {};

    for (unsigned int i = 0; i < face.num_edges; i++)
    {
        lindex = face.first_edge + i;
        startEdge = mFaceEdgetable[lindex];

        if (startEdge > 0)
        {
            edge = mEdges[startEdge];
            v.pos = mVertices[edge.v[0]];
        }
        else
        {
            edge = mEdges[-startEdge];
            v.pos = mVertices[edge.v[1]];
        }

        texinfo = face.texture_info;

        // s
        v.texture[0] = gameTextures[texinfo]->vecs[0][3] + v.pos.x * gameTextures[texinfo]->vecs[0][0] +
            v.pos.y * gameTextures[texinfo]->vecs[0][1] + v.pos.z * gameTextures[texinfo]->vecs[0][2];

        v.texture[0] /= gameTextures[texinfo]->width;

        // t
        v.texture[1] = gameTextures[texinfo]->vecs[1][3] + v.pos.x * gameTextures[texinfo]->vecs[1][0] +
            v.pos.y * gameTextures[texinfo]->vecs[1][1] + v.pos.z * gameTextures[texinfo]->vecs[1][2];

        v.texture[1] /= gameTextures[texinfo]->height;

        mFaceVertices.push_back(v);
    }

    /*numTriangles = face.num_edges - 2;
    
    for (unsigned int i = 1; i <= numTriangles; i++)
    {
        lindex = face.first_edge + i;

        mFaceIndices.push_back(indexCount + face.first_edge);
        mFaceIndices.push_back(indexCount + lindex         );
        mFaceIndices.push_back(indexCount + lindex + 1     );

    }

    indexCount += numTriangles * 3;*/

    numTriangles = face.num_edges - 2;

    indexCount += numTriangles * 3;

    vertexCount += face.num_edges;
}

point3f* Q2BspMap::GetCameraPos()
{
    return &mPos;
}

void Q2BspMap::SetCameraPos(float x, float y, float z)
{
    mPos = {x, y, z};
}