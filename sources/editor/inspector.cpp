#include "../../headers/editor/inspector.h"

Inspector::Inspector(int x, int y)
{
    position.set(x, y);

    nameText.setPosition(x, 0);
    posText.setPosition(x, 28+16);
    sizeText.setPosition(x, (28+16)*2);
    velocityText.setPosition(x, (28+16)*3);

    //TODO
    //nameField.setPosition(x+,0);
    //posXField.setPosition(x+,28+16);
    //posYField.setPosition(x+,28+16);
    //velXField.setPosition(x+,(28+16)*2);
    //velYField.setPosition(x+,(28+16)*2);
    //sizeXField.setPosition(x+, (28+16)*3);
    //sizeYField.setPosition(x+, (28+16)*3);
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

    //TODO: For each field... may be a good idea to create buttonCanvas or something like that
    /*
    .getText().loadFont( "./resources/font.ttf", 28 );
	.getTexture().loadFromFile( "./resources/buttonsheet.png", renderer );
	for( int i = 0; i < 4; ++i )
	{
		.getSpriteClip( i ).x = 0;
		.getSpriteClip( i ).y = i * BUTTON_HEIGHT;
        .getSpriteClip( i ).w = BUTTON_WIDTH;
        .getSpriteClip( i ).h = BUTTON_HEIGHT;
	}

    .setText("sample.scene");
	.loadTextTexture(renderer);
    */

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
}