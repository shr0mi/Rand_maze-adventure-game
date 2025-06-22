#ifndef MENU_HPP
#define MENU_HPP

#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>

class MenuScreen {
public:
    MenuScreen();
    bool playClick(sf::Vector2f pos);
    bool optionClick(sf::Vector2f pos);
    bool exitClick(sf::Vector2f pos);
    void draw(sf::RenderWindow& window);

private:
    sf::Texture playTexture, optionTexture, exitTexture, bgTexture, nameTexture;
    sf::Sprite playButton, optionButton, exitButton, background, nameSprite;
    // sf::Font font;
    sf::SoundBuffer soundBuffer;
    sf::Sound sound;
};

#endif
