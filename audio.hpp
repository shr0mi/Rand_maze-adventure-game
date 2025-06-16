#pragma once
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <SFML/Graphics.hpp>
class AudioManager
{
public:
    AudioManager();
    void toggleMusic();
    void updateIconPosition(const sf::Vector2f &pos);
    void draw(sf::RenderWindow &window) const;
    void setVolume(float volume);

private:
    sf::Music backgroundMusic;
    sf::Texture iconTexture;
    sf::Sprite iconSprite;
    bool musicOn = true;
};