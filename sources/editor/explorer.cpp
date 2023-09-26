// TODO: Implement
// * List standing directory
// * Be able to navigate

#include "../../headers/editor/explorer.h"

Explorer::Explorer(std::string path)
{
    if (!path.empty())
    {
        projectRoot = path;
    }
    goToProjectRoot();
}

void Explorer::enterDirectory(std::string dirName)
{
    currentPath = currentPath + "/" + dirName + "/";
    loadCurrentPath();
}

void Explorer::goBack()
{
    size_t index = currentPath.find_last_of('/');

    if (index != std::string::npos) {
        currentPath = currentPath.substr(0, index);
    }

    loadCurrentPath();
}

void Explorer::goToProjectRoot()
{
    currentPath = projectRoot;
    loadCurrentPath();
}

void Explorer::loadCurrentPath()
{
    folders.clear();
    files.clear();

    try {
        for (const auto& entry : std::filesystem::directory_iterator(currentPath)) {
            if (entry.is_directory()) {
                std::cout << "Directory: " << entry.path().filename() << "\n";
                folders.push_back(entry.path().filename());
            } else if (entry.is_regular_file()) {
                std::cout << "File: " << entry.path().filename() << "\n";
                files.push_back(entry.path().filename());
            }
        }
    } catch (const std::filesystem::filesystem_error& ex) {
        std::cerr << "Error: " << ex.what() << "\n";
    }

    directoryChanged = true;
}

void Explorer::render(SDL_Renderer* renderer)   // TODO: This function should be vastly improved
{
    if(directoryChanged)
    {
        panel.clearObjs();
        // NOTE: The problem is that the UIPanel position gets set after the image and text position gets set.
        // TODO: Solution: alignObjs() should call alignObjs recursively for child panels
        for(int i = 0; i < folders.size(); i++)
        {
            UIPanel* folderPanel = new UIPanel{0, 0, 33, 33};
            Text* text = new Text{folders[i], 0, 0};
            text->loadFont( "./resources/font.ttf", 12 );
            text->loadTexture(renderer);

            Image* image = new Image{0, 0, 32, 32}; // TODO: This should be UIStateObject
            image->getTexture() = folderTexture;

            std::cout << "image position before:" << image->getPosition().getX() << ", " << image->getPosition().getY() << std::endl; 
            std::cout << "text position before:" << text->getPosition().getX() << ", " << text->getPosition().getY() << std::endl; 

            folderPanel->addObj(image);
            folderPanel->addObj(text);
            folderPanel->setMaxHeight();
            folderPanel->alignObjs();

            std::cout << "image position after:" << image->getPosition().getX() << ", " << image->getPosition().getY() << std::endl; 
            std::cout << "text position after:" << text->getPosition().getX() << ", " << text->getPosition().getY() << std::endl; 
            
            panel.addObj(folderPanel);
        }
        for(int i = 0; i < files.size(); i++)
        {
            UIPanel* filePanel = new UIPanel{0, 0, 33, 33};
            Text* text = new Text{files[i], 0, 0};
            text->loadFont( "./resources/font.ttf", 12 );
            text->loadTexture(renderer);

            Image* image = new Image{0, 0, 32, 32};
            image->getTexture() = fileTexture;

            filePanel->addObj(text);
            filePanel->addObj(image);
            filePanel->setMaxHeight();
            filePanel->alignObjs();
            panel.addObj(filePanel);
        }
        panel.setMaxHeight();
        panel.alignObjs();

        directoryChanged = false;
    }
    panel.render(renderer);
}
