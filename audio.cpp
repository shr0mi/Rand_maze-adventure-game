// audio.cpp
#include "option.hpp"
#include "audio.hpp"
#include <iostream>
AudioManager::AudioManager() : iconSprite(iconTexture)
{
    if (!backgroundMusic.openFromFile("Preview.ogg"))
    {
        std::cerr << "Failed to load music.\n";
    }
    else
    {
        backgroundMusic.setLooping(true);
        backgroundMusic.play();
    }

}

void AudioManager::toggleMusic()
{
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

void AudioManager::updateIconPosition(const sf::Vector2f &pos)
{
    iconSprite.setPosition(pos);
}

void AudioManager::draw(sf::RenderWindow &window) const
{
    window.draw(iconSprite);
}

void AudioManager::setVolume(float volume) {
    backgroundMusic.setVolume(volume);  // Volume is 0 to 100
}
