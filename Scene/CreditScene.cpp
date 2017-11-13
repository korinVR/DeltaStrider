#include "CreditScene.h"

#include "../InputKit.h"
#include "../SoundKit.h"
#include "../GraphicsKit.h"

#include "TitleScene.h"

#include "../ResourceId.h"

CreditScene::CreditScene()
{
}

void CreditScene::Update(SceneManager* manager)
{
    if (InputKit::fireButtonCount == 1) {
        SoundKit::PlaySE(SE_SELECT);
        ChangeScene(manager, new TitleScene(TitleScene::ITEM_CREDIT));
    }
}

void CreditScene::Render(SceneManager* manager)
{
    GraphicsKit::Clear();
    GraphicsKit::PutSprite(SPRITE_CREDIT, 0, 0);
    GraphicsKit::RenderSprites();
    GraphicsKit::Present();
}


