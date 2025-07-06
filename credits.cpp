#include "credits.hpp"

CreditsScreen::CreditsScreen() :
    bgTexture("assets/menubg.png"),
    creditsTexture("assets/ui_credits.png"),
    backTexture("assets/backbtn.png"),
    background(bgTexture),
    creditsImage(creditsTexture),
    backButton(backTexture)
{
    background.setPosition({1920, 0}); // Position for credits view

    creditsImage.setPosition({2200, 20});
    creditsImage.setScale({0.25f, 0.25f});

    backButton.setPosition({2330, 460});
    backButton.setScale({0.3f, 0.3f});
}

bool CreditsScreen::backClicked(sf::Vector2f pos)
{
    return backButton.getGlobalBounds().contains(pos);
}

void CreditsScreen::draw(sf::RenderWindow &window)
{
    window.draw(background);
    window.draw(creditsImage);
    window.draw(backButton);
}
