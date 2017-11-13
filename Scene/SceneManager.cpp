#include <cassert>
using namespace std;

#include "Scene.h"
#include "SceneManager.h"

SceneManager::SceneManager(Scene* scene) : scene(scene), nextScene(0)
{
    frame = 0;
}

SceneManager::~SceneManager()
{
    delete scene;
}

void SceneManager::Update()
{
    if (nextScene) {
        delete scene;
        scene = nextScene;
        nextScene = 0;
    }
    frame++;
    scene->Update(this);
}

void SceneManager::Render()
{
    scene->Render(this);
}

int SceneManager::GetFrame() const
{
    return frame;
}

void SceneManager::ChangeScene(Scene* scene)
{
    assert(nextScene == 0);
    nextScene = scene;
    frame = 0;
}

