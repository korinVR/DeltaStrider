#include "StageCamera.h"

#include "MyShip.h"

const float CAMERA_Z = -60.0f;
const float FOV_MIN = 30.0f;
const float FOV_MAX = 60.0f;
const float MYSHIP_MAXSPEED = 0.2f;

const float Z_NEAR = 1.0f;
const float Z_FAR = 200.0f;

const float FOLLOW = 0.01f;
const float OFFSET_FOLLOW = 0.03f;

const float OFFSET_W = 4.0f;
const float OFFSET_H = 2.0f;

StageCamera::StageCamera()
{
    position = Vector(0, -30, CAMERA_Z);
    offset = Vector(0, 0, 0);
    fov = 45.0f;
}

void StageCamera::Update()
{
    float myShipRotation = Game::myShip->GetRotation();

    Vector offsetDist;
    offsetDist.x = cos(myShipRotation) * OFFSET_W;
    offsetDist.y = sin(myShipRotation) * OFFSET_H;
    offset += (offsetDist - offset) * OFFSET_FOLLOW;

    Vector myShipPosition = Game::myShip->GetPosition();
    Vector myShipVelocity = Game::myShip->GetVelocity();

    Vector destination = myShipPosition + myShipVelocity * 150.0f + offset;
    destination.z = CAMERA_Z;

    position += (destination - position) * FOLLOW;

    float fovDist = FOV_MIN + Length(myShipVelocity) * (FOV_MAX - FOV_MIN) / MYSHIP_MAXSPEED;
    fov += (fovDist - fov) * 0.01f;
}

void StageCamera::Render()
{
    Vector myShipVelocity = Game::myShip->GetVelocity();

    Vector camera = position; // + Vector(0, -20, 0);
    Vector target = Vector(position.x, position.y, 0);
    Vector up     = Vector(0, 1, 0);
    GraphicsKit::SetViewTransform(LookAtMatrix(camera, target, up));
    
    float aspectRatio = static_cast<float>(Game::SCREEN_W) / Game::SCREEN_H;
    GraphicsKit::SetProjectionTransform(PerspectiveMatrix(Radian(fov), aspectRatio, Z_NEAR, Z_FAR));
}


