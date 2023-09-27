// TODO: Implement
// * List standing directory
// * Be able to navigate

#include "../../headers/editor/explorer.h"

Explorer::Explorer(std::string path, int x, int y, int width, int height)
{
    if (!path.empty())
    {
        projectRoot = path;
    }
    goToProjectRoot();
    panel.getPosition().set(x, y);
    panel.getSize().set(width, height);
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
    texture.render(panel.getPosition().getX(), panel.getPosition().getY(), NULL, 0.0, NULL, SDL_FLIP_NONE, renderer);
    if(directoryChanged)
    {
        panel.clearObjs();
        for(int i = 0; i < folders.size(); i++)
        {
            UIPanel* folderPanel = new UIPanel{0, 0, 32, 48};
            Text* text = new Text{folders[i], 0, 0};
            text->loadFont( "./resources/font.ttf", 12 );
            text->loadTexture(renderer);

            Image* image = new Image{0, 0, 32, 32}; // TODO: This should be UIStateObject
            image->getTexture() = folderTexture;

            folderPanel->addObj(image);
            folderPanel->addObj(text);
            folderPanel->setMaxHeight();

            panel.addChildPanel(folderPanel);
        }
        for(int i = 0; i < files.size(); i++)
        {
            UIPanel* filePanel = new UIPanel{0, 0, 32, 48};
            Text* text = new Text{files[i], 0, 0};
            text->loadFont( "./resources/font.ttf", 12 );
            text->loadTexture(renderer);

            Image* image = new Image{0, 0, 32, 32};
            image->getTexture() = fileTexture;

            filePanel->addObj(image);
            filePanel->addObj(text);
            filePanel->setMaxHeight();

            panel.addChildPanel(filePanel);
        }
        panel.setMaxHeight();
        panel.alignObjs();

        directoryChanged = false;
    }
    panel.render(renderer);
}
