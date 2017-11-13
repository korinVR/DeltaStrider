#ifndef ___Texture_h
#define ___Texture_h

#include <d3d8.h>
#include <d3dx8.h>

#include <boost/utility.hpp>
using namespace boost;

class Texture : noncopyable {
public:
	Texture(char* filename);
	~Texture();

	IDirect3DTexture8* GetD3DTexture() const { return d3dTexture; }
	int GetWidth() const { return width; }
	int GetHeight() const { return height; }

private:
	IDirect3DTexture9* d3dTexture;
	int width;
	int height;
};

#endif // ___Texture_h

