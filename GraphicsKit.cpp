#include <windows.h>
#include <d3d8.h>
#include <d3dx8.h>

#include <algorithm>
#include <exception>
#include <stdexcept>
using namespace std;

#include <boost/utility.hpp>
using namespace boost;

#include "d3dfile.h"
#include "d3dfont.h"
#include "dxutil.h"

#include "Sparkler/Sparkler.h"
using namespace Sparkler;

#include "GraphicsKit.h"

#define LESS_LATENCY

namespace {
	LPDIRECT3D8 d3d = NULL;
	LPDIRECT3DDEVICE8 d3dDevice = NULL;
	
	D3DLIGHT8 light;

	HWND hWnd;
	D3DDISPLAYMODE desktopDisplayMode;
	int screenW;
	int screenH;
	bool windowed;
	bool vsync;

	bool recording = false;
	int recordingFrame = 0;
	
	class Texture : noncopyable {
	public:
		Texture(char* filename);
		~Texture();

		IDirect3DTexture8* GetD3DTexture() const { return d3dTexture; }
		int GetWidth() const { return width; }
		int GetHeight() const { return height; }

	private:
		IDirect3DTexture8* d3dTexture;
		int width;
		int height;
	};

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

	CD3DFont* d3dFont = NULL;
	
	const int MESH_MAX = 256;
	CD3DMesh* meshes[MESH_MAX];

	const int TEXTURE_MAX = 256;
	Texture* textures[TEXTURE_MAX];
	
	const int SPRITE_PATTERN_MAX = 4096;
	const int SPRITE_MAX = 4096;
	int spriteNum;
	
	struct SpritePattern {
		int textureNo;
		int x;
		int y;
		int width;
		int height;
		int centerX;
		int centerY;
	} spritePatterns[SPRITE_PATTERN_MAX];
	
	struct Sprite {
		int patternNo;
		float x;
		float y;
		float scale;
		float rotation;
		int alpha;
	} sprites[SPRITE_MAX];
	
	const int EFFECT_MAX = 4096;
	int effectNum;

	struct Effect {
		int patternNo;
		Vector position;
		float scale;
		float rotation;
		int alpha;

		bool operator<(const Effect& rhs) const
		{
			return (position.z < rhs.position.z);
		}
	} effects[EFFECT_MAX];
	
	Matrix viewTransform;
	Matrix projectionTransform;
	Matrix transformMatrix;
}

bool IsAvailableDepthFormat(D3DFORMAT depthFormat, D3DFORMAT backBufferFormat)
{
	if (SUCCEEDED(d3d->CheckDeviceFormat(D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL,
		backBufferFormat, D3DUSAGE_DEPTHSTENCIL, D3DRTYPE_SURFACE, depthFormat)))
	{
		if (SUCCEEDED(d3d->CheckDepthStencilMatch(D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL,
			backBufferFormat, backBufferFormat, depthFormat)))
		{
			return true;
		}
	}
	return false;
}

D3DPRESENT_PARAMETERS GetPresentParameters()
{
	D3DPRESENT_PARAMETERS pp;
	ZeroMemory(&pp, sizeof(D3DPRESENT_PARAMETERS));

	if (windowed) {
		pp.Windowed = TRUE;
		pp.BackBufferFormat = desktopDisplayMode.Format;
		pp.SwapEffect = D3DSWAPEFFECT_DISCARD;
//		pp.SwapEffect = D3DSWAPEFFECT_COPY_VSYNC;
	} else {
		pp.Windowed = FALSE;
		pp.BackBufferFormat = D3DFMT_X8R8G8B8;
		pp.FullScreen_PresentationInterval = vsync ? D3DPRESENT_INTERVAL_ONE : D3DPRESENT_INTERVAL_IMMEDIATE;
		pp.SwapEffect = D3DSWAPEFFECT_DISCARD;
	}
	pp.BackBufferCount	= 1;
	pp.BackBufferWidth	= screenW;
	pp.BackBufferHeight	= screenH;
//	pp.BackBufferWidth	= desktopDisplayMode.Width;
//	pp.BackBufferHeight	= desktopDisplayMode.Height;
	pp.hDeviceWindow	= hWnd;
#ifdef LESS_LATENCY
	pp.Flags = D3DPRESENTFLAG_LOCKABLE_BACKBUFFER;
#endif

	struct {
		char* name;
		D3DFORMAT format;
	} depthStencilFormats[] = {
//		"D32", D3DFMT_D32, 
//		"D24X8", D3DFMT_D24X8, 
//		"D24S8", D3DFMT_D24S8, 
//		"D24X4S4", D3DFMT_D24X4S4, 
		"D16", D3DFMT_D16, 
		"D15S1", D3DFMT_D15S1, 
		NULL, D3DFMT_UNKNOWN
	};

	int i;
	for (i = 0; depthStencilFormats[i].name; i++) {
		if (IsAvailableDepthFormat(depthStencilFormats[i].format, pp.BackBufferFormat))
			break;
	}
	if (depthStencilFormats[i].format == D3DFMT_UNKNOWN) {
		throw runtime_error("");
	}
	pp.EnableAutoDepthStencil = TRUE;
	pp.AutoDepthStencilFormat = depthStencilFormats[i].format;

	return pp;
}

void RestoreDeviceObjects()
{
	d3dFont->RestoreDeviceObjects();

	for (int i = 0; i < MESH_MAX; i++) {
		meshes[i]->RestoreDeviceObjects(d3dDevice);
	}

	d3dDevice->SetRenderState(D3DRS_DITHERENABLE, FALSE);
	d3dDevice->SetRenderState(D3DRS_SPECULARENABLE, TRUE);
	d3dDevice->SetRenderState(D3DRS_NORMALIZENORMALS, TRUE);

	d3dDevice->SetRenderState(D3DRS_ALPHATESTENABLE, TRUE); 
	d3dDevice->SetRenderState(D3DRS_ALPHAREF, (DWORD) 0x01);
	d3dDevice->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_GREATEREQUAL);

	d3dDevice->SetTextureStageState(0, D3DTSS_MINFILTER,	D3DTEXF_LINEAR);
	d3dDevice->SetTextureStageState(0, D3DTSS_MAGFILTER,	D3DTEXF_LINEAR);
	d3dDevice->SetTextureStageState(0, D3DTSS_MIPFILTER,	D3DTEXF_LINEAR);
}

void InvalidateDeviceObjects()
{
	d3dFont->InvalidateDeviceObjects();

	for (int i = 0; i < MESH_MAX; i++) {
		meshes[i]->InvalidateDeviceObjects();
	}
}

void ResetDevice()
{
	InvalidateDeviceObjects();
	
	D3DPRESENT_PARAMETERS pp = GetPresentParameters();
	d3dDevice->Reset(&pp);
	
	RestoreDeviceObjects();
}

void GraphicsKit::Initialize(HWND hWnd_, int width, int height, bool windowed_, bool vsync_)
{
	screenW = width;
	screenH = height;
	hWnd = hWnd_;
	windowed = windowed_;
	vsync = vsync_;
	
	d3d = Direct3DCreate8(D3D_SDK_VERSION);
	if (d3d == NULL) {
		throw runtime_error("");
	}
	
	if (FAILED(d3d->GetAdapterDisplayMode(D3DADAPTER_DEFAULT, &desktopDisplayMode))) {
		throw runtime_error("");
	}
    D3DPRESENT_PARAMETERS pp = GetPresentParameters();
    
   	D3DCAPS8 caps;
	ZeroMemory(&caps, sizeof(D3DCAPS8));
	d3d->GetDeviceCaps(D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, &caps);
	
	DWORD behavior;
	if (caps.DevCaps & D3DDEVCAPS_HWTRANSFORMANDLIGHT) {
		behavior = D3DCREATE_HARDWARE_VERTEXPROCESSING;
		if (caps.DevCaps & D3DDEVCAPS_PUREDEVICE) {
			behavior |= D3DCREATE_PUREDEVICE;
		}
	} else {
		behavior = D3DCREATE_SOFTWARE_VERTEXPROCESSING;
	}
	
	if (FAILED(d3d->CreateDevice(D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, hWnd, 
		behavior, &pp, &d3dDevice)))
	{
		throw runtime_error("");
	}
	
	d3dFont = new CD3DFont(_T("Arial"), 12, D3DFONT_BOLD);
	HRESULT hr;
	hr = d3dFont->InitDeviceObjects(d3dDevice);
	if (FAILED(hr)) {
		throw runtime_error("m_pFont->InitDeviceObjects");
	}

	for (int i = 0; i < MESH_MAX; i++) {
		meshes[i] = new CD3DMesh();
	}
	for (int i = 0; i < TEXTURE_MAX; i++) {
		textures[i] = NULL;
	}
	for (int i = 0; i < SPRITE_PATTERN_MAX; i++) {
		spritePatterns[i].textureNo = -1;
	}

	Clear();
	Fill(0, 0, 0);
	Present();
	
	RestoreDeviceObjects();
}

void GraphicsKit::Cleanup()
{
	InvalidateDeviceObjects();

	d3dFont->DeleteDeviceObjects();
	SAFE_DELETE(d3dFont);

	for (int i = 0; i < MESH_MAX; i++) {
		meshes[i]->Destroy();
		SAFE_DELETE(meshes[i]);
	}

	for (int i = 0; i < TEXTURE_MAX; i++) {
		SAFE_DELETE(textures[i]);
	}

	SAFE_RELEASE(d3dDevice);
	SAFE_RELEASE(d3d);
}

void GraphicsKit::ToggleFullScreen()
{
	windowed = !windowed;
	ResetDevice();
}


bool GraphicsKit::IsDeviceLost()
{
	HRESULT hr;

	if (FAILED(hr = d3dDevice->TestCooperativeLevel())) {
		if (hr == D3DERR_DEVICELOST) {
			return true;
		}
		if (hr == D3DERR_DEVICENOTRESET) {
			ResetDevice();
		}
	}
	return false;
}

void GraphicsKit::LoadMesh(int no, char* filename)
{
	Log::Printf("\tCreate mesh %s\n", filename);

	char buf[256];
	sprintf(buf, "model/%s", filename);
	meshes[no]->Create(d3dDevice, buf);
	meshes[no]->RestoreDeviceObjects(d3dDevice);
}

void GraphicsKit::RenderMesh(int no, const Matrix& worldMatrix)
{
	SetWorldTransform(worldMatrix);
	meshes[no]->Render(d3dDevice);
}

void GraphicsKit::LoadTexture(int no, char* filename)
{
	Log::Printf("\tCreate texture %s\n", filename);

	char buf[256];
	sprintf(buf, "texture/%s", filename);
	textures[no] = new Texture(buf);
}

void GraphicsKit::SetWorldTransform(const Matrix& matrix)
{
	d3dDevice->SetTransform(D3DTS_WORLD, reinterpret_cast<const D3DMATRIX*>(&matrix));
}

void GraphicsKit::SetViewTransform(const Matrix& matrix)
{
	d3dDevice->SetTransform(D3DTS_VIEW, reinterpret_cast<const D3DMATRIX*>(&matrix));
	
	viewTransform = matrix;
	transformMatrix = viewTransform * projectionTransform;
}

void GraphicsKit::SetProjectionTransform(const Matrix& matrix)
{
	d3dDevice->SetTransform(D3DTS_PROJECTION, reinterpret_cast<const D3DMATRIX*>(&matrix));
	
	projectionTransform = matrix;
	transformMatrix = viewTransform * projectionTransform;
}

const Vector GraphicsKit::GetScreenPosition(const Vector& position)
{
	Vector n = position * transformMatrix;
	n.x = n.x * 640 / 2 + 320;
	n.y = -n.y * 480 / 2 + 240;

	return n;
}

void GraphicsKit::Printf(float x, float y, const char* format, ...)
{
	static const int BUF_SIZE = 512;
	char buf[BUF_SIZE];

	va_list ap;
	va_start(ap, format);
	vsprintf(buf, format, ap);
	va_end(ap);

	d3dFont->DrawText(x, y, D3DCOLOR_ARGB(255, 255, 255, 0), buf);
}

void GraphicsKit::SetDirectionalLight(const Vector& v, float diffuse, float specular)
{
	ZeroMemory(&light, sizeof(D3DLIGHT8));
	light.Type = D3DLIGHT_DIRECTIONAL;
	light.Direction = D3DXVECTOR3(v.x, v.y, v.z);
	light.Diffuse.r = diffuse;
	light.Diffuse.g = diffuse;
	light.Diffuse.b = diffuse;
	light.Specular.r = specular;
	light.Specular.g = specular;
	light.Specular.b = specular;
	
	d3dDevice->SetLight(0, &light);
	d3dDevice->LightEnable(0, TRUE);
}

void GraphicsKit::SetOrthogonalProjectionTransform()
{
	float aspectRatio = static_cast<float>(screenW) / screenH;
	D3DXMATRIX projectionMatrix;
	D3DXMatrixOrthoLH(&projectionMatrix, 8 * aspectRatio, 8, 1, 200);
	d3dDevice->SetTransform(D3DTS_PROJECTION, &projectionMatrix);
}

void GraphicsKit::Clear()
{
#ifndef _DEBUG
	d3dDevice->Clear(0, NULL, D3DCLEAR_ZBUFFER, D3DCOLOR_XRGB(0, 0, 128), 1.0f, 0);
#else
	d3dDevice->Clear(0, NULL, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER, D3DCOLOR_XRGB(0, 0, 128), 1.0f, 0);
#endif
	d3dDevice->BeginScene();
	d3dDevice->SetRenderState(D3DRS_ZENABLE, TRUE);
	
	effectNum = 0;
	spriteNum = 0;
}

void GraphicsKit::Fill(int r, int g, int b)
{
	d3dDevice->Clear(0, NULL, D3DCLEAR_TARGET, D3DCOLOR_XRGB(r, g, b), 1.0f, 0);
}

void GraphicsKit::Present()
{
	d3dDevice->EndScene();

	if (recording) {
		LPDIRECT3DSURFACE8 surface;
		if (SUCCEEDED(d3dDevice->GetBackBuffer(0, D3DBACKBUFFER_TYPE_MONO, &surface))) {
			char buf[256];
			sprintf(buf, "T:\\movie\\frame%05d.bmp", recordingFrame++);
			D3DXSaveSurfaceToFile(buf, D3DXIFF_BMP, surface, NULL, NULL);
			surface->Release();
		}
	}
	d3dDevice->Present(NULL, NULL, NULL, NULL);

#ifdef LESS_LATENCY
	IDirect3DSurface8* backBuffer;
	if (SUCCEEDED(d3dDevice->GetBackBuffer(0, D3DBACKBUFFER_TYPE_MONO, &backBuffer))) {
		D3DLOCKED_RECT lockedRect;
		
		if (SUCCEEDED(backBuffer->LockRect(&lockedRect, NULL, D3DLOCK_READONLY))) {
			backBuffer->UnlockRect();
		}
		backBuffer->Release();
	}
#endif
}

void GraphicsKit::SetRecording(bool flag)
{
	recording = flag;
}

void GraphicsKit::SetLighting(bool flag)
{
	d3dDevice->SetRenderState(D3DRS_LIGHTING, flag ? TRUE : FALSE);
}

void GraphicsKit::SetZTest(bool test, bool write)
{
	d3dDevice->SetRenderState(D3DRS_ZENABLE, test ? D3DZB_TRUE : D3DZB_FALSE);
	d3dDevice->SetRenderState(D3DRS_ZWRITEENABLE, write ? D3DZB_TRUE : D3DZB_FALSE);
}

void GraphicsKit::SetAlpha(bool flag)
{
	d3dDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, flag ? TRUE : FALSE);
}

void GraphicsKit::EnableFog(DWORD color, float start, float end)
{
	d3dDevice->SetRenderState(D3DRS_FOGENABLE, TRUE);
	d3dDevice->SetRenderState(D3DRS_FOGCOLOR, color);
	d3dDevice->SetRenderState(D3DRS_FOGTABLEMODE, D3DFOG_LINEAR);
	d3dDevice->SetRenderState(D3DRS_FOGSTART, *(DWORD *) (&start));
	d3dDevice->SetRenderState(D3DRS_FOGEND, *(DWORD *) (&end));
}

void GraphicsKit::DisableFog()
{
	d3dDevice->SetRenderState(D3DRS_FOGENABLE, FALSE);
}

void GraphicsKit::DefineSprite(int patternNo, int textureNo, 
	int x, int y, int width, int height, int centerX, int centerY)
{
	if (patternNo < 0 || patternNo > SPRITE_PATTERN_MAX) {
		throw runtime_error("");
	}
	if (spritePatterns[patternNo].textureNo != -1) {
		throw runtime_error("");
	}
	spritePatterns[patternNo].textureNo	= textureNo;
	spritePatterns[patternNo].x			= x;
	spritePatterns[patternNo].y			= y;
	spritePatterns[patternNo].width		= width;
	spritePatterns[patternNo].height	= height;
	spritePatterns[patternNo].centerX	= centerX;
	spritePatterns[patternNo].centerY	= centerY;
}

void GraphicsKit::PutSprite(int patternNo, float x, float y, float scale, float rotation, int alpha)
{
	if (spriteNum >= SPRITE_MAX) {
		return;
	}
	sprites[spriteNum].patternNo	= patternNo;
	sprites[spriteNum].x			= x;
	sprites[spriteNum].y			= y;
	sprites[spriteNum].scale		= scale;
	sprites[spriteNum].rotation		= rotation;
	sprites[spriteNum].alpha		= alpha;
	
	spriteNum++;
}

void GraphicsKit::RenderSprites()
{
	const DWORD D3DFVF_SPRITE = D3DFVF_XYZRHW | D3DFVF_DIFFUSE | D3DFVF_TEX1;

	struct SpriteVertex {
		float x, y, z, rhw;
		D3DCOLOR diffuse;
		float tu, tv;

		void Set(float x, float y, float z, float rhw, D3DCOLOR& diffuse, float tu, float tv)
		{
			this->x = x;
			this->y = y;
			this->z = z;
			this->rhw = rhw;
			this->diffuse = diffuse;
			this->tu = tu;
			this->tv = tv;
		}
	} vertices[4];

	//d3dDevice->SetSamplerState(0, D3DSAMP_MINFILTER,	D3DTEXF_LINEAR);
	//d3dDevice->SetSamplerState(0, D3DSAMP_MAGFILTER,	D3DTEXF_LINEAR);
	//d3dDevice->SetSamplerState(0, D3DSAMP_MIPFILTER,	D3DTEXF_LINEAR);

	d3dDevice->SetRenderState(D3DRS_LIGHTING, FALSE);

	d3dDevice->SetRenderState(D3DRS_ZENABLE, D3DZB_FALSE);
	d3dDevice->SetRenderState(D3DRS_ZWRITEENABLE, D3DZB_FALSE);
	
	d3dDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);
	d3dDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	d3dDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);

	d3dDevice->SetTextureStageState(0, D3DTSS_COLOROP, D3DTOP_MODULATE);
	d3dDevice->SetTextureStageState(0, D3DTSS_COLORARG1, D3DTA_TEXTURE);
	d3dDevice->SetTextureStageState(0, D3DTSS_COLORARG2, D3DTA_DIFFUSE);
	d3dDevice->SetTextureStageState(0, D3DTSS_ALPHAOP, D3DTOP_MODULATE);
	d3dDevice->SetTextureStageState(0, D3DTSS_ALPHAARG1, D3DTA_TEXTURE);
	d3dDevice->SetTextureStageState(0, D3DTSS_ALPHAARG2, D3DTA_DIFFUSE);

	d3dDevice->SetTextureStageState(1, D3DTSS_COLOROP, D3DTOP_DISABLE);
	d3dDevice->SetTextureStageState(1, D3DTSS_ALPHAOP, D3DTOP_DISABLE);
	
	d3dDevice->SetVertexShader(D3DFVF_SPRITE);

	float screenScaleX = screenW / 640.0f;
	float screenScaleY = screenH / 480.0f;

	for (int i = 0; i < spriteNum; i++) {
		const float NEAR_Z = 10.0f;
		const float FAR_Z = 200.0f;
		
		SpritePattern& pattern = spritePatterns[sprites[i].patternNo];

		float x[4];
		float y[4];

		x[0] = x[3] = -pattern.centerX * sprites[i].scale;
		y[0] = y[1] = -pattern.centerY * sprites[i].scale;
		x[1] = x[2] = x[0] + pattern.width * sprites[i].scale;
		y[2] = y[3] = y[0] + pattern.height * sprites[i].scale;

		if (sprites[i].rotation != 0.0f) {
			float c = cos(-sprites[i].rotation);
			float s = sin(-sprites[i].rotation);

			for (int j = 0; j < 4; j++) {
				float tx = c * x[j] + s * y[j];
				float ty = -s * x[j] + c * y[j];
				x[j] = tx;
				y[j] = ty;
			}
		}
		for (int j = 0; j < 4; j++) {
			x[j] += sprites[i].x - 0.5f;
			y[j] += sprites[i].y - 0.5f;
		}

		float z = 0;
		float rhw = 1.0f / (z * (FAR_Z - NEAR_Z) + NEAR_Z);

		float textureWidth = static_cast<float>(textures[pattern.textureNo]->GetWidth());
		float textureHeight = static_cast<float>(textures[pattern.textureNo]->GetHeight());
		
		float sx0 = pattern.x / textureWidth;
		float sy0 = pattern.y / textureHeight;
		float sx1 = (pattern.x + pattern.width) / textureWidth;
		float sy1 = (pattern.y + pattern.height) / textureHeight;
		
		D3DCOLOR diffuse = D3DCOLOR_ARGB(sprites[i].alpha, 255, 255, 255);

		vertices[0].Set(x[0] * screenScaleX, y[0] * screenScaleY, z, rhw, diffuse, sx0, sy0);
		vertices[1].Set(x[1] * screenScaleX, y[1] * screenScaleY, z, rhw, diffuse, sx1, sy0);
		vertices[2].Set(x[2] * screenScaleX, y[2] * screenScaleY, z, rhw, diffuse, sx1, sy1);
		vertices[3].Set(x[3] * screenScaleX, y[3] * screenScaleY, z, rhw, diffuse, sx0, sy1);

		d3dDevice->SetTexture(0, textures[pattern.textureNo]->GetD3DTexture());
		d3dDevice->DrawPrimitiveUP(D3DPT_TRIANGLEFAN, 2, vertices, sizeof(SpriteVertex));
	}
	spriteNum = 0;
}

void GraphicsKit::PutEffect(int patternNo, const Vector& position, float scale, float rotation, int alpha)
{
	if (effectNum >= EFFECT_MAX) {
		return;
	}
	effects[effectNum].patternNo	= patternNo;
	effects[effectNum].position		= position;
	effects[effectNum].scale		= scale;
	effects[effectNum].rotation		= rotation;
	effects[effectNum].alpha		= alpha;

	effectNum++;
}

void GraphicsKit::RenderEffects()
{
	const DWORD D3DFVF_EFFECT = D3DFVF_XYZ | D3DFVF_DIFFUSE | D3DFVF_TEX1;

	struct EffectVertex {
		float x, y, z;
		D3DCOLOR diffuse;
		float tu, tv;

		void Set(float x, float y, float z, const D3DCOLOR& diffuse, float tu, float tv)
		{
			this->x = x;
			this->y = y;
			this->z = z;
			this->diffuse = diffuse;
			this->tu = tu;
			this->tv = tv;
		}
	} vertices[4];

	stable_sort(effects, effects + effectNum);

	d3dDevice->SetRenderState(D3DRS_LIGHTING, FALSE);

	d3dDevice->SetRenderState(D3DRS_ZENABLE, D3DZB_TRUE);
	d3dDevice->SetRenderState(D3DRS_ZWRITEENABLE, D3DZB_FALSE);
	
	d3dDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);
	d3dDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_ONE);
	d3dDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCCOLOR);

	d3dDevice->SetTextureStageState(0, D3DTSS_COLOROP, D3DTOP_MODULATE);
	d3dDevice->SetTextureStageState(0, D3DTSS_COLORARG1, D3DTA_TEXTURE);
	d3dDevice->SetTextureStageState(0, D3DTSS_COLORARG2, D3DTA_DIFFUSE);
	d3dDevice->SetTextureStageState(0, D3DTSS_ALPHAOP, D3DTOP_MODULATE);
	d3dDevice->SetTextureStageState(0, D3DTSS_ALPHAARG1, D3DTA_TEXTURE);
	d3dDevice->SetTextureStageState(0, D3DTSS_ALPHAARG2, D3DTA_DIFFUSE);

	d3dDevice->SetTextureStageState(1, D3DTSS_COLOROP, D3DTOP_DISABLE);
	d3dDevice->SetTextureStageState(1, D3DTSS_ALPHAOP, D3DTOP_DISABLE);

	d3dDevice->SetVertexShader(D3DFVF_EFFECT);

	for (int i = 0; i < effectNum; i++) {
		SpritePattern& pattern = spritePatterns[effects[i].patternNo];

		float x0 = static_cast<float>(-pattern.centerX);
		float y0 = static_cast<float>(-pattern.centerY);
		float x1 = static_cast<float>(pattern.width - pattern.centerX);
		float y1 = static_cast<float>(pattern.height - pattern.centerY);
		float z = 0;

		float textureWidth = static_cast<float>(textures[pattern.textureNo]->GetWidth());
		float textureHeight = static_cast<float>(textures[pattern.textureNo]->GetHeight());
		
		float sx0 = pattern.x / textureWidth;
		float sy0 = pattern.y / textureHeight;
		float sx1 = (pattern.x + pattern.width) / textureWidth;
		float sy1 = (pattern.y + pattern.height) / textureHeight;

		D3DCOLOR diffuse = D3DCOLOR_XRGB(effects[i].alpha, effects[i].alpha, effects[i].alpha);

		vertices[0].Set(x0, y0, z, diffuse, sx0, sy0);
		vertices[1].Set(x0, y1, z, diffuse, sx0, sy1);
		vertices[2].Set(x1, y1, z, diffuse, sx1, sy1);
		vertices[3].Set(x1, y0, z, diffuse, sx1, sy0);

		float scale = 0.05f * effects[i].scale;

		D3DXMATRIX matrix;
		D3DXMATRIX scalingMatrix;
		D3DXMATRIX rotationMatrix;
		D3DXMATRIX translationMatrix;

		D3DXMatrixScaling(&scalingMatrix, scale, scale, scale);
		D3DXMatrixRotationZ(&rotationMatrix, effects[i].rotation);
		D3DXMatrixTranslation(&translationMatrix, effects[i].position.x, effects[i].position.y, effects[i].position.z);

		matrix = scalingMatrix * rotationMatrix * translationMatrix;
		d3dDevice->SetTransform(D3DTS_WORLD, &matrix);

		d3dDevice->SetTexture(0, textures[pattern.textureNo]->GetD3DTexture());
		d3dDevice->DrawPrimitiveUP(D3DPT_TRIANGLEFAN, 2, vertices, sizeof(EffectVertex));
	}
	effectNum = 0;
}

void GraphicsKit::RenderFlashedMesh(int no, const Matrix& worldMatrix, float r, float g, float b)
{
	D3DLIGHT8 flashLight;
	ZeroMemory(&flashLight, sizeof(D3DLIGHT8));
	flashLight.Type = D3DLIGHT_DIRECTIONAL;
	flashLight.Direction = D3DXVECTOR3(1, 1, 1);
	flashLight.Diffuse.r = 0.0f;
	flashLight.Diffuse.g = 0.0f;
	flashLight.Diffuse.b = 0.0f;
	flashLight.Specular.r = 0.0f;
	flashLight.Specular.g = 0.0f;
	flashLight.Specular.b = 0.0f;
	flashLight.Ambient.r = r;
	flashLight.Ambient.g = g;
	flashLight.Ambient.b = b;
	d3dDevice->SetLight(0, &flashLight);
		
	RenderMesh(no, worldMatrix);

	d3dDevice->SetLight(0, &light);
}

