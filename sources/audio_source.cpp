#include "../headers/audio_source.h"

AudioSource::AudioSource() { }

void AudioSource::addSound(std::string path)
{
    Mix_Chunk* sound = Mix_LoadWAV(path.c_str());
    this->sounds.push_back(sound);
}

void AudioSource::addMusic(std::string path)
{
    Mix_Music* music = Mix_LoadMUS(path.c_str());
    this->music.push_back(music);
}

Mix_Chunk* AudioSource::getSound(int index)
{
    return this->sounds[index];
}

Mix_Music* AudioSource::getMusic(int index)
{
    return this->music[index];
}