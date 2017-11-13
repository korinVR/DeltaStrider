#ifndef ___GraphicsKit_h
#define ___GraphicsKit_h

#include <windows.h>

#include "Sparkler/Sparkler.h"
using namespace Sparkler;

class GraphicsKit {
public:
	static void Initialize(HWND hWnd, int width, int height, bool windowed, bool vsync);
	static void Cleanup();
	
	static void ToggleFullScreen();
	static bool IsDeviceLost();
	
	static void LoadMesh(int no, char* filename);
	static void RenderMesh(int no, const Matrix& worldMatrix);

	static void LoadTexture(int no, char* filename);

	static void SetWorldTransform(const Matrix& matrix);
	static void SetViewTransform(const Matrix& matrix);
	static void SetProjectionTransform(const Matrix& matrix);
	
	static const Vector GetScreenPosition(const Vector& position);
	
	static void Printf(float x, float y, const char* format, ...);
	
	static void SetDirectionalLight(const Vector& v, float diffuse, float specular);
	static void SetOrthogonalProjectionTransform();
	
	static void Clear();
	static void Fill(int r, int g, int b);
	static void Present();
	static void SetRecording(bool flag);

	static void SetLighting(bool flag);
	static void SetZTest(bool test, bool write);
	static void SetAlpha(bool flag);

	static void EnableFog(DWORD color, float start, float end);
	static void DisableFog();
	
	static void DefineSprite(int patternNo, int textureNo, 
		int x, int y, int width, int height, int centerX = 0, int centerY = 0);
	static void PutSprite(int patternNo, float x, float y, float scale = 1, float rotation = 0, int alpha = 255);
	static void PutEffect(int patternNo, const Vector& position, float scale = 1, float rotation = 0, int alpha = 255);

	static void RenderSprites();
	static void RenderEffects();
	
	static void RenderFlashedMesh(int no, const Matrix& worldMatrix, float r, float g, float b);
};

#endif // ___GraphicsKit_h


