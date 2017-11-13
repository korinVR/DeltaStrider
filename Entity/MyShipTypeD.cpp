#include "MyShipTypeD.h"

#include "../Radar.h"

#include "Debris.h"
#include "Explosion.h"
#include "MyShotTypeD.h"

const float ACCELERATION = 0.0007f;
const float RESISTANCE = 0.997f;

const float REBOUND = 0.05f;

const float RECOIL = 0.01f;
const float MYSHOT_SPEED = 0.25f;

const float INVINCIBLE_TIME = 2000;
const float DEAD_TIME = 500;

const float COLLISION_BALL_RADIUS = 0.7f;

MyShipTypeD::MyShipTypeD()
{
    position = Vector(0, -30, 0);
    SetState(STATE_ADVENT);
    showMuzzleFlash = false;
}

void MyShipTypeD::SetState(int state)
{
    MyShip::SetState(state);

    switch (state) {
    case STATE_ADVENT:
        position.z = -60;
        velocity = Vector(0, 0.07f, 0);
        rotation = Radian(90);
        rotationVelocity = 0;

        roll = 0;
        rapidFireCount = 0;
        jetScale = 0;

        SoundKit::PlaySE(SE_MYSHIP_ADVENT);
        return;
    }
}

void MyShipTypeD::Update()
{
    GameObject::Update();
    
    if (state == STATE_ADVENT) {
        if (position.z < 0) {
            position.z += min(-position.z * 0.007f, 0.15f);
        }

        if (count >= INVINCIBLE_TIME) {
            SetState(STATE_NORMAL);
        }
    }
    if (state == STATE_DEAD) {
        velocity *= 0.995f;

        if (count == DEAD_TIME) {
            Game::left--;
            if (Game::left > 0) {
                SetState(STATE_ADVENT);
            }
        }
        return;
    }

    Vector lever(InputKit::stickX, InputKit::stickY);

    if (Length(lever) > 0) {
        float direction = atan2(InputKit::stickY, InputKit::stickX);
        float d = Norm(direction - rotation);
        rotationVelocity += d * 0.002f;
    }
    rotationVelocity *= 0.9f;
    rotation = Norm(rotation + rotationVelocity);
    roll = rotationVelocity * 80.0f;

    velocity += Vector(InputKit::stickX * ACCELERATION, InputKit::stickY * ACCELERATION);
    velocity *= RESISTANCE;

    bool bounced = false;
    if (position.x + velocity.x > Game::FIELD_SIZE) {
        bounced = true;
        velocity.x = -velocity.x - REBOUND;
    }
    if (position.x + velocity.x < -Game::FIELD_SIZE) {
        bounced = true;
        velocity.x = -velocity.x + REBOUND;
    }
    if (position.y + velocity.y > Game::FIELD_SIZE) {
        bounced = true;
        velocity.y = -velocity.y - REBOUND;
    }
    if (position.y + velocity.y < -Game::FIELD_SIZE) {
        bounced = true;
        velocity.y = -velocity.y + REBOUND;
    }
    if (bounced) {
        rotationVelocity += Random::GetReal(-0.5f, 0.5f);
    }

    position += velocity;
    
    if (state != STATE_STAGE_CLEAR && InputKit::fireButtonPressed && position.z > -10.0f) {
        rapidFireCount++;
        
        if (rapidFireCount % 20 == 0) {
            Vector myShotPosition = position;
            Vector myShotVelocity = velocity;
            
            myShotPosition.x += cos(rotation) * 3;
            myShotPosition.y += sin(rotation) * 3;
            myShotVelocity.x += cos(rotation) * MYSHOT_SPEED;
            myShotVelocity.y += sin(rotation) * MYSHOT_SPEED;
            
            Game::myShots.Append(new MyShotTypeD(myShotPosition, myShotVelocity, rotation));

            SoundKit::PlaySE(SE_MYSHOT_TYPED);
            
            velocity.x -= cos(rotation) * RECOIL;
            velocity.y -= sin(rotation) * RECOIL;

            showMuzzleFlash = true;
        }
    }

    jetScale = (Length(Vector(InputKit::stickX, InputKit::stickY, 0))
        - fabs(rotationVelocity * 10) + Random::GetReal(0, 0.25f)) * 0.6f;
    
    if (state == STATE_NORMAL) {
        CollisionDetector::RegisterM(CollisionBall(this, position, COLLISION_BALL_RADIUS));
    }
}

void MyShipTypeD::Render()
{
    if (state == STATE_DEAD) {
        return;
    }
    Matrix worldMatrix = 
        TranslationMatrix(-0.5f, 0, 0) *
        RotationMatrixX(roll) * RotationMatrixZ(rotation) * 
        TranslationMatrix(position);

    if ((state == STATE_ADVENT || state == STATE_STAGE_CLEAR) && (count % 20 > 12)) {
        GraphicsKit::RenderFlashedMesh(MESH_MYSHIP_TYPED, worldMatrix, 4, 3, 2);
    } else {
        GraphicsKit::RenderMesh(MESH_MYSHIP_TYPED, worldMatrix);
    }
    GraphicsKit::PutEffect(SPRITE_JET, position, jetScale, rotation);

    if (showMuzzleFlash) {
        showMuzzleFlash = false;

        GraphicsKit::PutEffect(SPRITE_MUZZLE_FLASH + (rapidFireCount / 20) % 4, position, 1.4f, rotation);
    }

    Radar::RenderMyShipBlip(position, rotation);
}

void MyShipTypeD::Collided()
{
    SoundKit::PlaySE(SE_EXPLOSION_MYSHIP);
    CreateExplosionSuite(position, velocity);
    for (int i = 0; i < 20; i++) {
        CreateDebris(MESH_METAL_DEBRIS_1, position, Vector(0, 0, 0));
        CreateDebris(MESH_METAL_DEBRIS_2, position, Vector(0, 0, 0));
    }
    SetState(STATE_DEAD);
}


