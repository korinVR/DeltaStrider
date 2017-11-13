#include "SceneManager.h"
#include "Scene.h"

Scene::Scene()
{
}

Scene::~Scene()
{
}

void Scene::ChangeScene(SceneManager* manager, Scene* scene)
{
	manager->ChangeScene(scene);
}


