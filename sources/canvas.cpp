#include "../headers/canvas.h"

Canvas::Canvas()
{

}

void Canvas::render(SDL_Renderer* renderer)
{
    if (isActive) 
    {
        for (auto* obj: objs) 
        {
            obj->render(renderer);
        }
    }
}

void Canvas::handleEvent(SDL_Event* e)
{
    if (isActive)
    {
        for (auto* obj: objs)
        {
            obj->handleEvent(e);
        }
    }
}

void Canvas::freeTextures()
{
    for (auto* obj: objs)
    {
        obj->freeTexture();
    }
}

void Canvas::addObj(UIObject* obj)
{
    objs.push_back(obj);
}

void Canvas::setActive(bool state)
{
    isActive = state;
}