#ifndef CANVAS_H
#define CANVAS_H

#include <SDL2/SDL.h>
#include <vector>
#include "./ui_object.h"

class Canvas
{
	public:
		Canvas();

        void render(SDL_Renderer* renderer);

        void addObj(UIObject* obj);

    private:
        std::vector<UIObject*> objs;
};

#endif // CANVAS_H