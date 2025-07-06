#ifndef CONTROLS_HPP
#define CONTROLS_HPP

#include <SFML/Graphics.hpp>

class ControlsScreen
{
public:
    ControlsScreen();
    void draw(sf::RenderWindow &window);
    bool backClicked(sf::Vector2f pos);

private:
    sf::Texture bgTexture, controlsTexture, backTexture;
    sf::Sprite background, controlsImage, backButton;
};

#endif
