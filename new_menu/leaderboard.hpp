#pragma once
#include <SFML/Graphics.hpp>
#include <vector>
#include <string>

class Leaderboard {
public:
    Leaderboard();
    void run();

private:
    void handleEvents();
    void render();
    void loadDummyScores();

    sf::RenderWindow window;
    sf::Font font;
    std::vector<std::pair<std::string, int>> scores;
    std::vector<sf::Text> scoreTexts;
    sf::Texture leaderboardTexture, bgTexture;
    sf::Sprite leaderboardTitle, bgImage;
    // sf::Text title;
    // sf::Text backText;
};
