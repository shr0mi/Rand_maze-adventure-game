#include "options.hpp"
#include "audio.hpp"
#include <iostream>

AudioManager::AudioManager()
{
    if (!backgroundMusic.openFromFile("assets/One_Man_Symphony_In_The_Blink_Of_An_Eye_(Free)_02_An_Instant_Burst_(Action_02).mp3"))
    {
        std::cerr << "Failed to load music.\n";
    }
    else
    {
        backgroundMusic.setLooping(true);
        backgroundMusic.setVolume(20.f); // Set default volume
        backgroundMusic.play();
    }

    musicOn = true; // Initialize musicOn to true
}

void AudioManager::toggleMusic()
{
    // Toggle the music state
    if (musicOn)
    {
        backgroundMusic.pause();
        musicOn = false;
    }
    else
    {
        backgroundMusic.play();
        musicOn = true;
    }
}

void AudioManager::setVolume(float volume)
{
    backgroundMusic.setVolume(volume); // Volume range is 0 to 100
}

bool AudioManager::isMusicOn() const
{
    return musicOn;
}

float AudioManager::getVolume() const
{
    return backgroundMusic.getVolume();
}
