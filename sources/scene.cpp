#include "../headers/scene.h"

Scene::Scene()
{

}

void Scene::render(SDL_Renderer* renderer)
{
    if (isActive) 
    {
        for (auto* obj: objs)
        {
            obj->render(renderer);
        }
    }
}

void Scene::handleEvent(SDL_Event* e)
{
    if (isActive)
    {
        for (auto* obj: objs)
        {
            obj->handleEvent(e);
        }
    }
}

void Scene::addObj(GameObject* obj)
{
    objs.push_back(obj);
}

void Scene::setActive(bool state)
{
    isActive = state;
}

std::vector<GameObject*> Scene::getObjs()
{
    return objs;
}

GameObject* Scene::getObjFromName(std::string name)
{
    for (auto* obj: objs)
    {
        if(obj->getName() == name)
        {
            return obj;
        }
    }
}