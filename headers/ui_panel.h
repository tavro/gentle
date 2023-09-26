#ifndef UI_PANEL_H
#define UI_PANEL_H

#include <SDL2/SDL.h>
#include <vector>
#include <iostream>
#include "./utils/vector2d.h"
#include "./ui_object.h"
#include <random>

// TODO: If panel lager than screen, implement scroll functionality

class UIPanel : public UIObject
{
    public:
        UIPanel(int x, int y, int w, int h);

        void alignObjs();

        void handleEvent(SDL_Event* e) override
        {
            for (auto* obj: objs)
            {
                obj->handleEvent(e);
            }
        }

        void render(SDL_Renderer* renderer) override
        {
            //SDL_Rect box = { getPosition().getX(), getPosition().getY(), getSize().getX(), getSize().getY() };
            //getTexture().loadFromFile( "./resources/main-menu-background.png", renderer );
            //getTexture().setColor( randomUint8R, randomUint8G, randomUint8B );
            //getTexture().render( getPosition().getX(), getPosition().getY(), &box, 0.0, NULL, SDL_FLIP_NONE, renderer );
            
            for (auto* obj: objs) 
            {
                obj->render(renderer);
            }
        }

        void setSpaceBetween(int space);

        void addObj(UIObject* obj);

        void addChildPanel(UIPanel* obj);

        void clearObjs();

        void setMaxHeight();

    private:
        std::vector<UIPanel*> childPanels;

        std::vector<UIObject*> objs;
        int spaceBetween;
        int uiObjMaxHeight;

        //uint8_t randomUint8R;
        //uint8_t randomUint8G;
        //uint8_t randomUint8B;
};

#endif // UI_PANEL_H