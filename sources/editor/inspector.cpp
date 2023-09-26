// TODO: Implement
// Show visual representation of fields of selected object from heirarchy

Inspector::Inspector()
{
    
}

void Inspector::setActiveObj(GameObject* gameObject)
{
    activeObj = gameObject;
}

void Inspector::loadFont()
{
    nameText.loadFont    ( "./resources/font.ttf", 28 );
    posText.loadFont     ( "./resources/font.ttf", 28 );
    sizeText.loadFont    ( "./resources/font.ttf", 28 );
    velocityText.loadFont( "./resources/font.ttf", 28 );
}

void Inspector::render(SDL_Renderer* renderer)
{
    std::string name = activeObj.getName();

    int xPos = activeObj.getPosition().getX();
    int yPos = activeObj.getPosition().getY();

    int xSize = activeObj.getSize().getX();
    int ySize = activeObj.getSize().getY();

    int xVel = activeObj.getVelocity().getX();
    int yVel = activeObj.getVelocity().getY();

    nameText.updateContent("Name: '" + name + "'");
    posText.updateContent("Position: (X:" + xPos + ", Y:" + yPos + ")");
    sizeText.updateContent("Size: (X:" + xSize + ", Y:" + ySize + ")");
    velocityText.updateContent("Velocity: (X:" + xVel + ", Y:" + yVel + ")");

    posText.loadTexture(renderer);
    nameText.loadTexture(renderer);
    sizeText.loadTexture(renderer);
    velocityText.loadTexture(renderer);
}