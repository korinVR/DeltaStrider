#include <exception>
#include <stdexcept>
using namespace std;

#include "Texture.h"

#include "dxutil.h"

Texture::Texture(char* filename)
{
    if (FAILED(D3DXCreateTextureFromFile(d3dDevice, filename, &d3dTexture))) {
        throw runtime_error("");
    }
    D3DSURFACE_DESC desc;
    d3dTexture->GetLevelDesc(0, &desc);
    width = desc.Width;
    height = desc.Height;
}

Texture::~Texture()
{
    SAFE_RELEASE(d3dTexture);
}


