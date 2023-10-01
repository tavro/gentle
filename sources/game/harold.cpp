#include "../../headers/game/harold.h"

namespace game
{
    Harold::Harold(Vector2D pos)
        : GameObject(pos, {32, 32}, {0, 0}, "Harold", "./resources/harold/haroldstill.png")
    {
        // TODO: init walk and idle animation
    }

    void Harold::updateTexture(SDL_Renderer *renderer)
    {
        loadTexture(renderer);
    }
}
