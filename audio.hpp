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
    void handleClick(const sf::Vector2i &mousePos, const sf::RenderWindow &window);

private:
    sf::Music backgroundMusic;
    sf::Texture iconTexture;
    sf::Sprite iconSprite; // will construct it properly in the .cpp
    bool musicOn = true;
};