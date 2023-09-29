#include "../headers/scene.h"

Scene::Scene()
{

}

Scene::Scene(std::string path)
{
    load(path);
}

void Scene::render(SDL_Renderer* renderer)
{
    if (isActive) 
    {
        for (auto* obj: objs)
        {
            obj->render(renderer);
        }
    }
}

void Scene::handleEvent(SDL_Event* e)
{
    if (isActive)
    {
        for (auto* obj: objs)
        {
            obj->handleEvent(e);
        }
    }
}

void Scene::addObj(GameObject* obj)
{
    objs.push_back(obj);
}

void Scene::setActive(bool state)
{
    isActive = state;
}

std::vector<GameObject*> Scene::getObjs()
{
    return objs;
}

GameObject* Scene::getObjFromName(std::string name)
{
    for (auto* obj: objs)
    {
        if(obj->getName() == name)
        {
            return obj;
        }
    }
}

void Scene::loadTextures(SDL_Renderer* renderer)
{
    for (auto* obj: objs)
    {
        obj->loadTexture(renderer);
    }
}

void Scene::load(std::string path)
{
    std::cout << "load:" << path << std::endl;
    objs.clear();
    std::ifstream file(path);
    if (!file.is_open()) 
    {
        std::cerr << "Failed to open file: " << path << std::endl;
        return;
    }

    std::string line;
    std::vector<std::string> objectData;
    while (std::getline(file, line)) 
    {
        //line.erase(line.find_first_not_of(" \t"), line.find_last_not_of(" \t") + 1);

        if (line.empty()) 
        {
            continue;
        }

        if (line.find("[") != std::string::npos) // Start of object
        {
            objectData.clear();
        } 
        else if (line.find("]") != std::string::npos) // End of object
        {
            if (objectData.size() >= 5) {
                bool createObject = false;
                
                Vector2D position{};
                Vector2D velocity{};
                Vector2D size{};

                std::string name = "";
                std::string texturePath = "";

                for (const std::string& data : objectData) {
                    std::istringstream iss(data);
                    std::string key, value;
                    std::getline(iss, key, ':');
                    std::getline(iss, value, ';');
                    
                    if (key == "Type")
                    {
                        if(value == "GameObject")
                        {
                            createObject = true;
                        }
                    }
                    else if (key == "Name")
                    {
                        name = value;
                    }
                    else if (key == "Texture")
                    {
                        texturePath = value;
                    } 
                    else if (key == "Position")
                    {
                        std::istringstream posStream(value);

                        int x, y;

                        char delimiter1, delimiter2, delimiter3;
                        posStream >> delimiter1 >> x >> delimiter2 >> y >> delimiter3;

                        if (!(delimiter1 == '(' && delimiter2 == ',' && delimiter3 == ')' && !iss.fail()))
                        {
                            std::cerr << "Failed to parse the input string." << std::endl;
                        }

                        position.set(x, y);
                    } 
                    else if (key == "Size")
                    {
                        std::istringstream posStream(value);

                        int x, y;

                        char delimiter1, delimiter2, delimiter3;
                        posStream >> delimiter1 >> x >> delimiter2 >> y >> delimiter3;

                        if (!(delimiter1 == '(' && delimiter2 == ',' && delimiter3 == ')' && !iss.fail()))
                        {
                            std::cerr << "Failed to parse the input string." << std::endl;
                        }

                        size.set(x, y);
                    } 
                    else if (key == "Velocity")
                    {
                        std::istringstream posStream(value);

                        int x, y;

                        char delimiter1, delimiter2, delimiter3;
                        posStream >> delimiter1 >> x >> delimiter2 >> y >> delimiter3;

                        if (!(delimiter1 == '(' && delimiter2 == ',' && delimiter3 == ')' && !iss.fail()))
                        {
                            std::cerr << "Failed to parse the input string." << std::endl;
                        }

                        velocity.set(x, y);
                    }
                }
                if(createObject)
                {
                    addObj(new GameObject{position, size, velocity, name, texturePath});
                }
            }
        } 
        else // In object
        {
            objectData.push_back(line);
        }
    }

    // Close the file
    file.close();
}
