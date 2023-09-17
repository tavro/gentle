#ifndef BUTTON_H
#define BUTTON_H

#include <SDL2/SDL.h>
#include "./texture.h"
#include "./ui_object.h"

const int BUTTON_WIDTH = 96;
const int BUTTON_HEIGHT = 32;

class Button : public UIObject
{
	public:
		Button();
};

#endif // BUTTON_H
