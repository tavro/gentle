#include "../../headers/editor/inspector.h"

Inspector::Inspector(int x, int y)
{
    position.set(x, y);

    nameText.setPosition(x, 0);
    posText.setPosition(x, 28+16);
    sizeText.setPosition(x, (28+16)*2);
    velocityText.setPosition(x, (28+16)*3);

    nameField.setPosition(x+96,0);
    posXField.setPosition(x+96,28+16);
    posYField.setPosition(x+96+96,28+16);
    velXField.setPosition(x+96,(28+16)*2);
    velYField.setPosition(x+96+96,(28+16)*2);
    sizeXField.setPosition(x+96, (28+16)*3);
    sizeYField.setPosition(x+96+96, (28+16)*3);

    fields.push_back(&nameField);
    fields.push_back(&posXField);
    fields.push_back(&posYField);
    fields.push_back(&velXField);
    fields.push_back(&velYField);
    fields.push_back(&sizeXField);
    fields.push_back(&sizeYField);
}

void Inspector::setActiveObj(GameObject* gameObject)
{
    activeObj = gameObject;
}

void Inspector::loadFont(SDL_Renderer* renderer)
{
    nameText.loadFont    ( "./resources/font.ttf", 28 );
    posText.loadFont     ( "./resources/font.ttf", 28 );
    sizeText.loadFont    ( "./resources/font.ttf", 28 );
    velocityText.loadFont( "./resources/font.ttf", 28 );

    for (auto* field: fields)
    {
        field->getText().loadFont( "./resources/font.ttf", 28 );
        field->getTexture().loadFromFile( "./resources/buttonsheet.png", renderer );
        for( int i = 0; i < 4; ++i )
        {
            field->getSpriteClip( i ).x = 0;
            field->getSpriteClip( i ).y = i * 32;
            field->getSpriteClip( i ).w = 96;
            field->getSpriteClip( i ).h = 32;
        }

        field->setText("xx");
        field->loadTextTexture(renderer);
    }

    texture.loadFromFile( "./resources/inspector.png", renderer );
}

void Inspector::render(SDL_Renderer* renderer)
{
    texture.render( position.getX(), position.getY(), NULL, 0.0, NULL, SDL_FLIP_NONE, renderer );

    std::string name = activeObj->getName();

    int xPos = activeObj->getPosition().getX();
    int yPos = activeObj->getPosition().getY();

    int xSize = activeObj->getSize().getX();
    int ySize = activeObj->getSize().getY();

    int xVel = activeObj->getVelocity().getX();
    int yVel = activeObj->getVelocity().getY();

    std::string namestr = "Name: '" + name + "'";
    std::string positionstr = "Position: (X:" + std::to_string(xPos) + ", Y:" + std::to_string(yPos) + ")";
    std::string sizestr = "Size: (X:" + std::to_string(xSize) + ", Y:" + std::to_string(ySize) + ")";
    std::string velocitystr = "Velocity: (X:" + std::to_string(xVel) + ", Y:" + std::to_string(yVel) + ")";

    nameText.updateContent(namestr);
    posText.updateContent(positionstr);
    sizeText.updateContent(sizestr);
    velocityText.updateContent(velocitystr);

    posText.loadTexture(renderer);
    nameText.loadTexture(renderer);
    sizeText.loadTexture(renderer);
    velocityText.loadTexture(renderer);

    posText.render(renderer);
    nameText.render(renderer);
    sizeText.render(renderer);
    velocityText.render(renderer);

    for (auto* field: fields)
    {
        field->render(renderer);
    }
}