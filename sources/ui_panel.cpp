#include "../headers/ui_panel.h"

UIPanel::UIPanel()
{

}

void UIPanel::alignObjs()
{
    int totalWidth = 0;
    int y = getPostion().getY();
    int row = 0;

    for(int i = 0; i < objs.length(); i++)
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
        // TODO: SET TEXT POSITION?
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