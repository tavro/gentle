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
}

void Explorer::render(SDL_Renderer* renderer)   // TODO: This function should be vastly improved
{
    UIPanel panel{0, 0, 256, 256};

    for(int i = 0; i < folders.size(); i++)
    {
        Image* image = new Image{0, 0, 32, 32}; // TODO: This should be UIStateObject
	    image->getTexture() = folderTexture;
        panel.addObj(image);
    }
    for(int i = 0; i < files.size(); i++)
    {
        Image* image = new Image{0, 0, 32, 32};
	    image->getTexture() = fileTexture;
        panel.addObj(image);
    }

    panel.alignObjs();
    panel.render(renderer);
}
