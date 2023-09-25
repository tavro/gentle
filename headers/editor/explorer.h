#ifndef EXPLORER_H
#define EXPLORER_H

#include <SDL2/SDL.h>

#include <string>
#include <vector>
#include <iostream>
#include <filesystem>

#include "../ui_panel.h"
#include "../image.h"

class Explorer
{
    public:
        Explorer(std::string path);

        void enterDirectory(std::string dirName);

        void goBack();

        void goToProjectRoot();

        void loadCurrentPath();

        void render(SDL_Renderer* renderer);

        Texture folderTexture;
        Texture fileTexture;
        
    private:
        std::vector<std::string> folders;
        std::vector<std::string> files;

        std::string projectRoot = "./";
        std::string currentPath;

};

#endif // EXPLORER_H