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
    currentPath = currentPath + dirName + "/";
    loadCurrentPath();
}

size_t findNextToLastDelimiter(const std::string &str, char delimiter) {
    size_t lastPos = str.find(delimiter);
    size_t nextToLastPos = std::string::npos;

    while (lastPos != std::string::npos) {
        nextToLastPos = lastPos;
        lastPos = str.find(delimiter, lastPos + 1);
    }

    return nextToLastPos;
}

void Explorer::goBack()
{
    if(currentPath.length() > 2) {
        size_t index = findNextToLastDelimiter(currentPath.substr(0, currentPath.length()-1), '/');
        currentPath = currentPath.substr(0, index+1);
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
    folderPtrs.clear();
    filePtrs.clear();

    try {
        for (const auto& entry : std::filesystem::directory_iterator(currentPath)) {
            if (entry.is_directory()) {
                //std::cout << "Directory: " << entry.path().filename() << "\n";
                folders.push_back(entry.path().filename());
            } else if (entry.is_regular_file()) {
                //std::cout << "File: " << entry.path().filename() << "\n";
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

            UIStateObject* uiStateObj = new UIStateObject{0, 0, 32, 32};
            uiStateObj->getTexture() = folderTexture;

            uiStateObj->addSpriteClip( { 0, 0, 32, 32 } );
            uiStateObj->addSpriteClip( { 0, 32, 32, 32 } );
            uiStateObj->addSpriteClip( { 0, 64, 32, 32 } );
            uiStateObj->addSpriteClip( { 0, 96, 32, 32 } );
            folderPtrs.push_back(uiStateObj);

            folderPanel->addObj(uiStateObj);
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

            UIStateObject* uiStateObj = new UIStateObject{0, 0, 32, 32};
            uiStateObj->getTexture() = fileTexture;

            uiStateObj->addSpriteClip( { 0, 0, 32, 32 } );
            uiStateObj->addSpriteClip( { 0, 32, 32, 32 } );
            uiStateObj->addSpriteClip( { 0, 64, 32, 32 } );
            uiStateObj->addSpriteClip( { 0, 96, 32, 32 } );
            filePtrs.push_back(uiStateObj);

            filePanel->addObj(uiStateObj);
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

void Explorer::handleEvent( SDL_Event* e )
{
    for(int i = 0; i < folderPtrs.size(); i++)
    {
        folderPtrs[i]->handleEvent(e);
        if(folderPtrs[i]->isToggled())
        {
            enterDirectory(folders[i]);
            folderPtrs[i]->setToggle(false);
            return;
        }
    }

    for(int i = 0; i < filePtrs.size(); i++)
    {
        filePtrs[i]->handleEvent(e);
        if(filePtrs[i]->isToggled())
        {
            currentFile = currentPath + files[i];
            fileChanged = true;
            filePtrs[i]->setToggle(false);
            return;
        }
    }
}

bool Explorer::fileHasChanged()
{
    return fileChanged;
}

void Explorer::toggleFileChanged()
{
    fileChanged = !fileChanged;
}