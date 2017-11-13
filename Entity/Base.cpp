#include "Base.h"

#include "../Radar.h"

#include "BaseShield.h"
#include "Debris.h"
#include "EnemyShot.h"
#include "Explosion.h"
#include "MyShip.h"

const float SHOT_SPEEDS[] = {0.04f, 0.08f, 0.12f, 0.14f };
const int SHOT_INTERVALS[] = { 900, 400, 200, 100 };

Base::Base(Vector position)
{
    this->position = position;
    rotation = Random::GetReal(0, PI2);
    shield = shared_ptr<BaseShield>(new BaseShield(position));
    
    if (Game::rank == 0) {
        hp = 15;
    } else {
        hp = 25;
    }
    flash = false;

    shotSpeed = SHOT_SPEEDS[Game::rank];
    shotInterval = SHOT_INTERVALS[Game::rank] - Game::stage * 10;
    if (shotInterval < 100) {
        shotInterval = 100;
    }
    shotCount = 0;
}

void Base::Update()
{
    GameObject::Update();
    shotCount++;
    
    if (InSight()) {
        CollisionDetector::RegisterN(CollisionBall(this, position, 9.5f));

        float distance = Length(Game::myShip->GetPosition() - position);

        if (distance < 50 && shotCount > shotInterval) {
            Vector v = Unit(Game::myShip->GetPosition() - position);
            float angle = atan2(v.y, v.x);
            angle += Random::GetReal(-Radian(30), Radian(30));

            Vector shotVelocity(cos(angle) * shotSpeed, sin(angle) * shotSpeed);
            Game::enemyShots.Append(new EnemyShot(position, shotVelocity));
            shotCount = 0;
        }
    }
    shield->Update();
}

void Base::Render()
{
    if (InSight()) {
        if (flash) {
            flash = false;
            GraphicsKit::RenderFlashedMesh(MESH_BASE, RotationMatrixZ(rotation) * TranslationMatrix(position), 4, 2, 2);
        } else {
            GraphicsKit::RenderMesh(MESH_BASE, RotationMatrixZ(rotation) * TranslationMatrix(position));
        }
    }
    Radar::RenderTargetEnemyBlip(position);
    shield->Render();
}

void Base::Collided()
{
    SoundKit::PlaySE(SE_HIT);
    flash = true;
    Game::scoreCount = 10;

    if (--hp <= 0) {
        SoundKit::PlaySE(SE_EXPLOSION_BIG);
        CreateBigExplosionSuite(position, Vector(0, 0, 0));
        for (int i = 0; i < 40; i++) {
            CreateDebris(MESH_METAL_DEBRIS_1, position, Vector(0, 0, 0));
            CreateDebris(MESH_METAL_DEBRIS_2, position, Vector(0, 0, 0));
        }
        Game::score += 2000;
        
        shield->Die();
        Die();
    }
}


