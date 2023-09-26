#include "../headers/ui_panel.h"

UIPanel::UIPanel(int x, int y, int w, int h) : UIObject( x, y, w, h )
{

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

        if(x >= (getSize().getX() - OBJ_WIDTH))
        {
            row++;
            totalWidth = 0;
            x = getPosition().getX();
            y = getPosition().getY() + (uiObjMaxHeight + spaceBetween) * row;
        }

        totalWidth += OBJ_WIDTH + spaceBetween;
        objs[i]->setPosition(x, y);
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

void UIPanel::clearObjs()
{
    objs.clear();
}
