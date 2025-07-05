#include "leaderboard.hpp"
#include <algorithm>

Leaderboard::Leaderboard()
    : window(sf::VideoMode({960, 540}), "Leaderboard"),
      leaderboardTexture(),
      bgTexture(),
      leaderboardTitle(leaderboardTexture),
      bgImage(bgTexture)
{
    window.setFramerateLimit(60);

    font.openFromFile("assets/arial.ttf");

    loadDummyScores();
}

void Leaderboard::loadDummyScores()
{
    scores = {
        {"Alice", 300},
        {"Bob", 250},
        {"Charlie", 200},
        {"Dave", 150},
        {"Eve", 100}};

    std::sort(scores.begin(), scores.end(),
              [](auto &a, auto &b)
              { return a.second > b.second; });

    float y = 100.0f;
    for (size_t i = 0; i < scores.size(); ++i)
    {
        sf::Text line(font, "", 24);
        line.setFillColor(sf::Color::White);
        line.setString(std::to_string(i + 1) + ". " + scores[i].first + " - " + std::to_string(scores[i].second));
        line.setPosition({320, y + i * 40});
        scoreTexts.push_back(line);
    }
}

void Leaderboard::handleEvents()
{
    while (window.isOpen())
    {
        while (std::optional<sf::Event> event = window.pollEvent())
        {
            if (event->is<sf::Event::Closed>())
                window.close();
        }
    }
}

void Leaderboard::render()
{
    window.clear(sf::Color(30, 30, 30)); // Dark background
    window.draw(bgImage);
    window.draw(leaderboardTitle);
    for (auto &text : scoreTexts)
    {
        window.draw(text);
    }
    window.display();
}

void Leaderboard::run()
{
    while (window.isOpen())
    {
        handleEvents();
        render();
    }
}
