#include "../headers/ui_panel.h"

UIPanel::UIPanel(int x, int y, int w, int h) : UIObject( x, y, w, h )
{
    spaceBetween = 16; // TODO: Hardcoded value

    // NOTE: For debugging
    /*
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<int> distribution(0, 0xFF);
    int randomR = distribution(gen);
    int randomG = distribution(gen);
    int randomB = distribution(gen);
    
    randomUint8R = static_cast<uint8_t>(randomR);
    randomUint8G = static_cast<uint8_t>(randomG);
    randomUint8B = static_cast<uint8_t>(randomB);
    */
}

void UIPanel::alignObjs()
{
    int totalWidth = 0;
    int y = getPosition().getY();
    int row = 0;

    for(int i = 0; i < objs.size(); i++)
    {
        const int OBJ_WIDTH = objs[i]->getSize().getX();
        int x = getPosition().getX() + totalWidth;

        if(x > (getPosition().getX() /* <= hope i'm not stupid and hope that this doesnt cause any problems */ + getSize().getX() - OBJ_WIDTH))
        {
            row++;
            totalWidth = 0;
            x = getPosition().getX();
            y = getPosition().getY() + (uiObjMaxHeight + spaceBetween) * row;
        }

        totalWidth += OBJ_WIDTH + spaceBetween;
        objs[i]->setPosition(x, y);
    }

    for(int i = 0; i < childPanels.size(); i++)
    {
        childPanels[i]->alignObjs();
    }
}

void UIPanel::setSpaceBetween(int space)
{
    spaceBetween = space;
}

void UIPanel::addObj(UIObject* obj)
{
    objs.push_back(obj);
}

void UIPanel::addChildPanel(UIPanel* obj)
{
    addObj(obj);
    childPanels.push_back(obj);
}

void UIPanel::clearObjs()
{
    objs.clear();
}

void UIPanel::setMaxHeight()
{
    int max = 0;
    for(int i = 0; i < objs.size(); i++)
    {
        if(objs[i]->getSize().getY() > max)
        {
            max = objs[i]->getSize().getY();
        }
    }
    uiObjMaxHeight = max;
}
