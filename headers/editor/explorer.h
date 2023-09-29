#ifndef EXPLORER_H
#define EXPLORER_H

#include <SDL2/SDL.h>

#include <string>
#include <vector>
#include <iostream>
#include <filesystem>

#include "../ui_panel.h"
#include "../ui_state_object.h"
#include "../image.h"
#include "../text.h"

class Explorer
{
    public:
        Explorer(std::string path, int x, int y, int width, int height);

        void enterDirectory(std::string dirName);

        void goBack();

        void goToProjectRoot();

        void loadCurrentPath();

        void render(SDL_Renderer* renderer);

        void handleEvent( SDL_Event* e );

        bool fileHasChanged();

        void toggleFileChanged();

        Texture folderTexture;
        Texture fileTexture;
        Texture texture;

        std::string currentFile;

    private:
        UIPanel panel{0, 0, 256, 256};

        std::vector<UIStateObject*> folderPtrs;
        std::vector<UIStateObject*> filePtrs;

        std::vector<std::string> folders;
        std::vector<std::string> files;

        std::string projectRoot = "./";
        std::string currentPath;

        bool directoryChanged;
        bool fileChanged;
};

#endif // EXPLORER_H