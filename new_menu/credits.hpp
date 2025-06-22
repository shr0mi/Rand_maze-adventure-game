#ifndef CREDITS_HPP
#define CREDITS_HPP

#include <SFML/Graphics.hpp>

class CreditsScreen {
public:
    CreditsScreen();
    void draw(sf::RenderWindow& window);
    bool backClicked(sf::Vector2f pos);

private:
    sf::Texture bgTexture, creditsTexture, backTexture;
    sf::Sprite background, creditsImage, backButton;
};

#endif
