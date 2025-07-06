#include "controls.hpp"

ControlsScreen::ControlsScreen() : bgTexture("assets/menubg.png"),
                                   controlsTexture("assets/controls.png"),
                                   backTexture("assets/backbtn.png"),
                                   background(bgTexture),
                                   controlsImage(controlsTexture),
                                   backButton(backTexture)
{
    background.setPosition({3840, 0});          // Fill full screen
    controlsImage.setPosition({4100, 50});      // Centered horizontally
    controlsImage.setScale({0.1667f, 0.1667f}); // Keep as-is if size looks good

    backButton.setPosition({4220, 460}); // Bottom-left
    backButton.setScale({0.3f, 0.3f});
}

bool ControlsScreen::backClicked(sf::Vector2f pos)
{
    return backButton.getGlobalBounds().contains(pos);
}

void ControlsScreen::draw(sf::RenderWindow &window)
{
    window.draw(background);
    window.draw(controlsImage);
    window.draw(backButton);
}
