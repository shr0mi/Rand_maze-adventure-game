#include "menu.hpp"

MenuScreen::MenuScreen() :
    bgTexture("assets/menubg.png"),
    playTexture("assets/playbtn.png"),
    optionTexture("assets/optionbtn.png"),
    boardTexture("assets/leaderboard.png"),
    exitTexture("assets/exitbtn.png"),
    nameTexture("assets/gameName.png"),

    background(bgTexture),
    playButton(playTexture),
    optionButton(optionTexture),
    boardButton(boardTexture),
    exitButton(exitTexture),
    nameSprite(nameTexture),

    soundBuffer("assets/click-a.ogg"),
    sound(soundBuffer)
{
    background.setPosition({0, 0});

    playButton.setPosition({390, 160}); // 200
    playButton.setScale({0.3f, 0.3f});

    optionButton.setPosition({390, 260}); // 310
    optionButton.setScale({0.3f, 0.3f});

    boardButton.setPosition({370, 350});
    boardButton.setScale({0.2f, 0.2f});

    nameSprite.setPosition({120, 50});
    nameSprite.setScale({0.25f, 0.25f});

    exitButton.setPosition({390, 420}); // 420
    exitButton.setScale({0.3f, 0.3f});

}

bool MenuScreen::playClick(sf::Vector2f pos)
{
    return playButton.getGlobalBounds().contains(pos);
}

bool MenuScreen::optionClick(sf::Vector2f pos)
{
    return optionButton.getGlobalBounds().contains(pos);
}

bool MenuScreen::boardClick(sf::Vector2f pos)
{
    return boardButton.getGlobalBounds().contains(pos);
}

bool MenuScreen::exitClick(sf::Vector2f pos)
{
    return exitButton.getGlobalBounds().contains(pos);
}

void MenuScreen::draw(sf::RenderWindow &window)
{
    window.draw(background);
    window.draw(nameSprite);
    window.draw(playButton);
    window.draw(optionButton);
    window.draw(boardButton);
    window.draw(exitButton);
}
