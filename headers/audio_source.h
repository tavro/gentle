#ifndef AUDIOSOURCE_H
#define AUDIOSOURCE_H

#include <SDL2/SDL.h>
#include <SDL2/SDL_mixer.h>

#include <vector>
#include <string>

class AudioSource
{
    public:
        AudioSource();

        void addSound(std::string path);
        void addMusic(std::string path);

        Mix_Chunk* getSound(int index);
        Mix_Music* getMusic(int index);

    private:
        std::vector<Mix_Chunk*> sounds;
        std::vector<Mix_Music*> music;
};

#endif // AUDIOSOURCE_H