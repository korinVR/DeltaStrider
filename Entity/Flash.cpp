#include "Flash.h"

Flash::Flash(Vector position)
{
	this->position = position;
	scale = 1;
	alpha = 255;
}

void Flash::Update()
{
	scale += 0.2f;
	alpha -= 1.0f;
	if (alpha <= 0) {
		Die();
	}
}

void Flash::Render()
{
	GraphicsKit::PutEffect(SPRITE_FLASH, position, scale, 0, static_cast<int>(alpha));
}


