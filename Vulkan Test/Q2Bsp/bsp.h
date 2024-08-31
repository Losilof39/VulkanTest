#pragma once

#include <cstdint>

// bsp structures

#define BLOCK_WIDTH 128
#define BLOCK_HEIGHT 128

#define MAXLIGHTMAPS 4
#define MAX_LIGHTMAPS 128
#define LIGHTMAP_BYTES 4
#define MAX_LIGHTSTYLES 4

#define	SURF_LIGHT		0x1		// value will hold the light strength

#define	SURF_SLICK		0x2		// effects game physics

#define	SURF_SKY		0x4		// don't draw, but add to skybox
#define	SURF_WARP		0x8		// turbulent water warp
#define	SURF_TRANS33	0x10
#define	SURF_TRANS66	0x20
#define	SURF_FLOWING	0x40	// scroll towards angle
#define	SURF_NODRAW		0x80	// don't bother referencing the texture

enum BSP_LUMP
{
    ENTITIES = 0,
    PLANES_T = 1,
    VERTICES,
    VIS,       
    NODES,    
    TEXTURES, 
    FACES,     
    LIGHTMAPS,      
    LEAVES,        
    LEAFFACETABLE, 
    LEAFBRUSHTABLE, 
    EDGES,           
    EDGEFACETABLE,  
    MODELS,         
    BRUSHES,        
    BRUSHSIDES,     
};

struct lightstyle_t
{
    float		rgb[3];			// 0.0 - 2.0
    float		white;			// highest of rgb
};


struct bsp_lump
{

    uint32_t    offset;     // offset (in bytes) of the data from the beginning of the file
    uint32_t    length;     // length (in bytes) of the data

};

struct bsp_header
{

    uint32_t    magic;      // magic number ("IBSP")
    uint32_t    version;    // version of the BSP format (38)

    bsp_lump  lump[19];   // directory of the lumps
};

struct point3f
{

    float x;
    float y;
    float z;

};

struct point3s
{

    int16_t x;
    int16_t y;
    int16_t z;

};

struct bsp_edge
{
    uint16_t v[2];
};

struct bsp_face
{

    uint16_t   plane;             // index of the plane the face is parallel to
    uint16_t   plane_side;        // set if the normal is parallel to the plane normal
          
    uint32_t   first_edge;        // index of the first edge (in the face edge array)
    uint16_t   num_edges;         // number of consecutive edges (in the face edge array)
          
    uint16_t   texture_info;      // index of the texture info structure	
          
    uint8_t    lightmap_styles[4]; // styles (bit flags) for the lightmaps
    uint32_t   lightmap_offset;   // offset of the lightmap (in bytes) in the lightmap lump
};

struct msurface_t
{
    uint16_t   plane;             // index of the plane the face is parallel to
    uint16_t   plane_side;        // set if the normal is parallel to the plane normal

    uint32_t   first_edge;        // index of the first edge (in the face edge array)
    uint16_t   num_edges;         // number of consecutive edges (in the face edge array)

    uint16_t   texture_info;      // index of the texture info structure	

    uint8_t    lightmap_styles[4];// styles (bit flags) for the lightmaps
    uint32_t   lightmap_offset;   // offset of the lightmap (in bytes) in the lightmap lump

    int16_t    textureMins[2];
    int16_t    uvExtents[2];
    int32_t    light_s;
    int32_t    light_t;
    uint8_t*   samples;
    int        lightmapIndex;
};

struct bsp_plane
{

    point3f   normal;      // A, B, C components of the plane equation
    float     distance;    // D component of the plane equation
    uint32_t    type;        // ?

};

struct bsp_node
{

    uint32_t   plane;             // index of the splitting plane (in the plane array)
         
    int32_t    front_child;       // index of the front child node or leaf
    int32_t    back_child;        // index of the back child node or leaf
         
    point3s  bbox_min;          // minimum x, y and z of the bounding box
    point3s  bbox_max;          // maximum x, y and z of the bounding box
         
    uint16_t   first_face;        // index of the first face (in the face array)
    uint16_t   num_faces;         // number of consecutive edges (in the face array)

};

struct bsp_leaf
{

    int32_t   content;          // ?
          
    int16_t   cluster;           // -1 for cluster indicates no visibility information
    uint16_t   area;              // ?
          
    point3s  bbox_min;          // bounding box minimums
    point3s  bbox_max;          // bounding box maximums
          
    uint16_t   first_leaf_face;   // index of the first face (in the face leaf array)
    uint16_t   num_leaf_faces;    // number of consecutive edges (in the face leaf array)
          
    uint16_t   first_leaf_brush;  // ?
    uint16_t   num_leaf_brushes;  // ?

};

struct bsp_texinfo
{

    float		vecs[2][4]; // [s/t][xyz offset]

    int32_t   flags;
    int32_t   value;
          
    char     texture_name[32];
          
    int32_t   next_texinfo;

};

// the visibility lump consists of a header with a count, then
// byte offsets for the PVS and PHS of each cluster, then the raw
// compressed bit vectors
#define	DVIS_PVS	0
#define	DVIS_PHS	1
struct bsp_vis_offset
{
    int32_t numclusters;
    int32_t bitofs[8][2];
};

struct wal_header
{

    char    name[32];        // name of the texture

    uint32_t  width;           // width (in pixels) of the largest mipmap level
    uint32_t  height;          // height (in pixels) of the largest mipmap level
          
    int32_t   offset[4];       // byte offset of the start of each of the 4 mipmap levels
          
    char      next_name[32];   // name of the next texture in the animation
          
    uint32_t  flags;           // ?
    uint32_t  contents;        // ?
    uint32_t  value;           // ?

};

struct pcx_t
{
    char	manufacturer;
    char	version;
    char	encoding;
    char	bits_per_pixel;
    uint16_t	xmin, ymin, xmax, ymax;
    uint16_t	hres, vres;
    uint8_t	palette[48];
    char	reserved;
    char	color_planes;
    uint16_t	bytes_per_line;
    uint16_t	palette_type;
    char	filler[58];
    uint8_t	data;			// unbounded
};