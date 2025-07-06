#ifndef LEADERBOARD_HPP
#define LEADERBOARD_HPP

#include <SFML/Graphics.hpp>
#include <vector>
#include <string>

class LeaderboardScreen
{
public:
    LeaderboardScreen();
    void draw(sf::RenderWindow &window);
    bool backClicked(sf::Vector2f pos);
    void reload(); // Reload scores from the file

private:
    void loadScores();

    sf::Texture bgTexture, leaderboardTexture, backTexture;
    sf::Sprite background, leaderboardImage, backButton;
    sf::Font font;
    std::vector<sf::Text> scoreTexts;
};

#endif
