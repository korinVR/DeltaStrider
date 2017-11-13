#include "BaseShield.h"

#include "MyShip.h"

BaseShield::BaseShield(Vector position)
{
    this->position = position;
    rotation = Random::GetReal(0, PI2);
    flash = false;
}

void BaseShield::Update()
{
    if (InSight()) {
        CollisionDetector::RegisterN(CollisionBall(this, position + UnitVectorZAligned(rotation + Radian(90)) * 18, 2));
        CollisionDetector::RegisterN(CollisionBall(this, position + UnitVectorZAligned(rotation - Radian(90)) * 18, 2));
    }
}

void BaseShield::Render()
{
    if (InSight()) {
        if (flash) {
            flash = false;
            GraphicsKit::RenderFlashedMesh(MESH_BASE_SHIELD, RotationMatrixZ(rotation) * TranslationMatrix(position), 2, 2, 2);
        } else {
            GraphicsKit::RenderMesh(MESH_BASE_SHIELD, RotationMatrixZ(rotation) * TranslationMatrix(position));
        }
    }
}

void BaseShield::Collided()
{
    SoundKit::PlaySE(SE_HIT_KIKI);
    flash = true;
}


