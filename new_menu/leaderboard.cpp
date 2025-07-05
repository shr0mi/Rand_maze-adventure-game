#include "leaderboard.hpp"
#include <fstream>
#include <algorithm>
#include <sstream>

LeaderboardScreen::LeaderboardScreen() :
    bgTexture("assets/menubg.png"),
    leaderboardTexture("assets/leaderboard.png"),
    backTexture("assets/backbtn.png"),
    background(bgTexture),
    leaderboardImage(leaderboardTexture),
    backButton(backTexture)
{
    background.setPosition({3000, 0}); // Further right than credits

    leaderboardImage.setPosition({3000, 20});
    leaderboardImage.setScale({0.3f, 0.3f});

    backButton.setPosition({3000, 460});
    backButton.setScale({0.3f, 0.3f});

    if (!font.openFromFile("assets/arial.ttf")) // Ensure this font exists
    {
        // error
    }
    loadScores();

    // Render score text objects
    float y = 150;
    for (int i = 0; i < scores.size(); ++i) {
        sf::Text text(font);
        std::stringstream ss;
        ss << (i + 1) << ". " << scores[i];
        text.setString(ss.str());
        text.setFont(font);
        text.setCharacterSize(36);
        text.setFillColor(sf::Color::White);
        text.setPosition({4120, y});
        y += 50;
        scoreTexts.push_back(text);
    }
}

void LeaderboardScreen::loadScores()
{
    std::ifstream file("scoreboard.txt");
    int x;
    while (file >> x) {
        scores.push_back(x);
    }

    std::sort(scores.begin(), scores.end());
    std::reverse(scores.begin(), scores.end());

    if (scores.size() > 7)
        scores.resize(7); // Keep only top 7
}

bool LeaderboardScreen::backClicked(sf::Vector2f pos)
{
    return backButton.getGlobalBounds().contains(pos);
}

void LeaderboardScreen::draw(sf::RenderWindow& window)
{
    window.draw(background);
    window.draw(leaderboardImage);
    window.draw(backButton);
    for (const auto& text : scoreTexts)
        window.draw(text);
}
