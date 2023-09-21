#include "../headers/scene.h"

Scene::Scene()
{

}

void Scene::render(SDL_Renderer* renderer)
{
    for (auto* obj: objs) {
        obj->render(renderer);
    }
}

void Scene::addObj(GameObject* obj)
{
    objs.push_back(obj);
}