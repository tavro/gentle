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
        UIPanel* textPanel = new UIPanel{0, 0, panel.getSize().getX(), 28};
        
        Text* text = new Text{obj->getName(), 0, 0};
        text->loadFont( "./resources/font.ttf", 28 );
        text->loadTexture(renderer);
        
        textPanel->addObj(text);
        
        panel.addChildPanel(textPanel);
        
        names.push_back(obj->getName());
    }
    panel.setMaxHeight();
    panel.alignObjs();

    texture.loadFromFile( "./resources/main-menu-background.png", renderer );
    texture.setColor( 0xFF, 165, 0 );

    texture2.loadFromFile( "./resources/heirarchy.png", renderer );

    maxIndex = names.size();
}

void Heirarchy::render(SDL_Renderer* renderer)
{
    texture2.render( panel.getPosition().getX(), panel.getPosition().getY(), NULL, 0.0, NULL, SDL_FLIP_NONE, renderer );
    
    SDL_Rect box = { 0, 0, panel.getSize().getX(), 28 };
    texture.render( panel.getPosition().getX(), panel.getPosition().getY() + ((28 + 16) * index), &box, 0.0, NULL, SDL_FLIP_NONE, renderer );
    
    panel.render(renderer);
}

void Heirarchy::increaseIndex()
{
    index++;
    if(index >= maxIndex)
    {
        index = 0;
    }
}

void Heirarchy::decreaseIndex()
{
    index--;
    if(index < 0)
    {
        index = maxIndex-1;
    }
}

Scene* Heirarchy::getActiveScene()
{
    return activeScene;
}

std::string Heirarchy::getActiveObjName()
{
    return names[index];
}