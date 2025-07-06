#pragma once
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <SFML/Graphics.hpp>
class AudioManager
{
public:
    AudioManager();
    void toggleMusic(); // Toggle music on/off
    void draw(sf::RenderWindow &window) const;
    void setVolume(float volume); // Set the volume of the music
    bool isMusicOn() const; // Check if music is on
    float getVolume() const; // Get the current volume of the music

private:
    sf::Music backgroundMusic;
    bool musicOn = true; // Track music state
};