#include "../../headers/game/cursor.h"

#include "../../headers/utils/constants.h"

namespace game
{
    Cursor::Cursor(std::string name) : GameObject({SCREEN_WIDTH, SCREEN_HEIGHT}, {32, 32}, {0, 0}, name, "./resources/hand-point.png") {}

    void Cursor::updateTexture(SDL_Renderer *renderer)
    {
        if (isClosed)
        {
            setTexturePath("./resources/hand-closed.png");
        }
        else if (isHovering)
        {
            setTexturePath("./resources/hand.png");
        }
        else
        {
            setTexturePath("./resources/hand-point.png");
        }
        loadTexture(renderer);
    }
}
