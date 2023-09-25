#ifndef UI_PANEL_H
#define UI_PANEL_H

#include <SDL2/SDL.h>
#include <vector>
#include "./utils/vector2d.h"
#include "./ui_object.h"

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
            // TODO: Render background if any
            for (auto* obj: objs) 
            {
                obj->render(renderer);
            }
        }

        void setSpaceBetween(int space);

        void addObj(UIObject* obj);

    private:
        std::vector<UIObject*> objs;
        int spaceBetween = 16;          // TODO: Hardcoded values
        int uiObjMaxHeight = 32;
};

#endif // UI_PANEL_H