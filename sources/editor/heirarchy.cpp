// TODO: Implement
// * List GameObjects in active scene
// * Select GameObject to show in inspector

#include "../../headers/editor/heirarchy.h"

Heirarchy::Heirarchy(int x, int y, int w, int h)
{
    panel.getPosition().set(x, y);
    panel.getSize().set(w, h);
}

void Heirarchy::setActiveScene(Scene* scene, SDL_Renderer* renderer)
{
    activeScene = scene;
    index = 0;

    std::vector<GameObject*> objs = activeScene->getObjs();
    for (auto* obj: objs)
    {
        Text* text = new Text{obj->getName(), 0, 0};
        text->loadFont( "./resources/font.ttf", 28 );
        text->loadTexture(renderer);
        panel.addObj(text);
        
        names.push_back(obj->getName());
    }
    panel.setMaxHeight();
    panel.alignObjs();

    maxIndex = names.size();
}

void Heirarchy::render(SDL_Renderer* renderer)
{
    // TODO: Render (selection)rect at index
    panel.render(renderer);
}