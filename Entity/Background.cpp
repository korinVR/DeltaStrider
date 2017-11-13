#include "Background.h"

Background::Background(int no) : no(no)
{
}

void Background::Update()
{
}

void Background::Render()
{
	GraphicsKit::SetLighting(false);
	GraphicsKit::SetZTest(false, false);
	GraphicsKit::DisableFog();
	GraphicsKit::RenderMesh(MESH_BACKGROUND_0 + no, IdentityMatrix());

	GraphicsKit::SetLighting(true);
	GraphicsKit::SetZTest(false, false);
	GraphicsKit::RenderMesh(MESH_BACKGROUND_FRAME, IdentityMatrix());
}


