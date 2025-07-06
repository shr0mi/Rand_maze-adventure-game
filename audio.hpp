#pragma once
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <SFML/Graphics.hpp>
class AudioManager
{
public:
    AudioManager();
    void toggleMusic();
    void draw(sf::RenderWindow &window) const;
    void setVolume(float volume);
    bool isMusicOn() const;
    float getVolume() const;

private:
    sf::Music backgroundMusic;
    bool musicOn = true;
};