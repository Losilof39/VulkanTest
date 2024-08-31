#include "TextureManager.h"
#include "bsp.h"
#include <fstream>

TextureManager* TextureManager::GetInstance()
{
    static TextureManager instance;
    return &instance;
}

GameTexture* TextureManager::LoadTextureWal(std::string filename)
{
    std::ifstream           wal(filename, std::ios::in | std::ios::binary | std::ios::ate);
    uint32_t                size = 0;
    std::vector<char>       wal_stream;
    wal_header*             walHdr;
    uint8_t*                data;
    int                     s;
    std::vector<unsigned int>    trans;
    unsigned int            width, height;
    GameTexture*            texture = new GameTexture(filename, 0, 0);

    if (wal.is_open())
    {
        size = (uint32_t)wal.tellg();
        wal.seekg(std::ios::beg);
        wal_stream.resize(size);
        wal.read(wal_stream.data(), size);
        wal.close();
    }

    walHdr = (wal_header*)wal_stream.data();

    data = (uint8_t*)walHdr + walHdr->offset[0];

    width  = walHdr->width;
    height = walHdr->height;

    texture->width = width;
    texture->height = height;

    /*s = width * height;

    trans.resize(s);*/

    //for (int i = 0; i < s; i++)
    //{
    //    int p = data[i];
    //    trans[i] = mPalette[p];

    //    /* transparent, so scan around for
    //       another color to avoid alpha fringes */
    //    if (p == 255)
    //    {
    //        if ((i > width) && (data[i - width] != 255))
    //        {
    //            p = data[i - width];
    //        }
    //        else if ((i < s - width) && (data[i + width] != 255))
    //        {
    //            p = data[i + width];
    //        }
    //        else if ((i > 0) && (data[i - 1] != 255))
    //        {
    //            p = data[i - 1];
    //        }
    //        else if ((i < s - 1) && (data[i + 1] != 255))
    //        {
    //            p = data[i + 1];
    //        }
    //        else
    //        {
    //            p = 0;
    //        }

    //        /* copy rgb components */
    //        ((uint8_t*)&trans[i])[0] = ((uint8_t*)&mPalette[p])[0];
    //        ((uint8_t*)&trans[i])[1] = ((uint8_t*)&mPalette[p])[1];
    //        ((uint8_t*)&trans[i])[2] = ((uint8_t*)&mPalette[p])[2];
    //    }
    //}

    texture->mData = trans;

    //LightScaleTexture(trans, width, height);

    return texture;
}

void TextureManager::LoadPalette()
{
    uint8_t temp[256] = {};
        uint32_t size = 0;
        pcx_t* p = nullptr;

        std::ifstream pal_file("assets/colormap.pcx", std::ios::in | std::ios::binary | std::ios::ate);

        std::vector<char> pal_stream;

        if (pal_file.is_open())
        {
            size = pal_file.tellg();
            pal_file.seekg(std::ios::beg);
            pal_stream.resize(size);
            pal_file.read(pal_stream.data(), size);


            pal_file.close();
        }

        p = (pcx_t*)pal_stream.data();

        std::memcpy(temp, (uint8_t*)p + size - 768, 768);

        for (unsigned int i = 0; i < 256; i++)
        {
            unsigned	v;
            int	r, g, b;

            r = temp[i * 3 + 0];
            g = temp[i * 3 + 1];
            b = temp[i * 3 + 2];

            v = (255U << 24) + (r << 0) + (g << 8) + (b << 16);
            mPalette[i] = v;
            //std::cout << v << std::endl;
        }

        mPalette[255] &= 0xffffff;
}