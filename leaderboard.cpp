#include "leaderboard.hpp"
#include <fstream>
#include <algorithm>
#include <sstream>
#include <iostream>

LeaderboardScreen::LeaderboardScreen() : bgTexture("assets/menubg.png"),
                                         leaderboardTexture("assets/leaderboard.png"),
                                         backTexture("assets/backbtn.png"),
                                         background(bgTexture),
                                         leaderboardImage(leaderboardTexture),
                                         backButton(backTexture)
{
    background.setPosition({2600, 0});

    leaderboardImage.setPosition({3200, 20});
    leaderboardImage.setScale({0.3f, 0.3f});

    backButton.setPosition({3280, 460});
    backButton.setScale({0.3f, 0.3f});

    if (!font.openFromFile("assets/arial.ttf")) // Ensure this font exists
    {
        // error
    }
    loadScores();
}

void LeaderboardScreen::loadScores()
{

    float y = 130.0f;
    FILE *fptr;
    fptr = fopen("scoreboard.dat", "rb");
    int x;
    int i = 0;
    while (fread(&x, sizeof(int), 1, fptr) == 1)
    {
        std::cout << x << std::endl;
        // Format the score as minutes and seconds
        std::stringstream ss;
        ss << "#" << (i + 1) << " Score: " << x / 60 << "m " << x % 60 << "s";
        std::string scoreLine = ss.str();

        // Create a text object for each score
        sf::Text line(font, scoreLine, 24);
        line.setFillColor(sf::Color::Yellow);
        line.setStyle(sf::Text::Bold);
        line.setPosition({3280.0f, y + i * 40});

        scoreTexts.push_back(line); // Store the text object in the vector
        i++;
    }
    fclose(fptr);
}

bool LeaderboardScreen::backClicked(sf::Vector2f pos)
{
    return backButton.getGlobalBounds().contains(pos);
}

void LeaderboardScreen::draw(sf::RenderWindow &window)
{
    window.draw(background);
    window.draw(leaderboardImage);
    window.draw(backButton);

    // Draw all score texts
    for (auto &text : scoreTexts)
    {
        window.draw(text);
    }
}

void LeaderboardScreen::reload()
{
    scoreTexts.clear(); // Clear previous texts
    loadScores();       // Reload from file
}

