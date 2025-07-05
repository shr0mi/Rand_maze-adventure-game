#include "leaderboard.hpp"
#include <algorithm>
#include <fstream>
#include <iostream>

Leaderboard::Leaderboard()
    : window(sf::VideoMode({960, 540}), "Leaderboard")
    //   leaderboardTexture(),
    //   bgTexture(),
    //   leaderboardTitle(leaderboardTexture),
    //   bgImage(bgTexture)
{
    window.setFramerateLimit(60);

    // font.openFromFile("arial.ttf");
    if (!font.openFromFile("assets/arial.ttf"))
    {
        std::cerr << "Failed to load font!\n";
    }

    loadDummyScores();
}

// void Leaderboard::loadDummyScores()
// {
//     std::vector<int> top_scores;

//     std::ifstream infile("scoreboard.txt");
//     int x;
//     while (infile >> x)
//     {
//         top_scores.push_back(x);
//     }
//     infile.close();

//     std::sort(top_scores.begin(), top_scores.end()); // lower time = better

//     float y = 100.0f;
//     for (size_t i = 0; i < top_scores.size() && i < 7; ++i)
//     {
//         int score = top_scores[i];
//         int minutes = score / 60;
//         int seconds = score % 60;

//         sf::Text line(font, "TOP SCORES", 24);
//         line.setFillColor(sf::Color::Red);
//         line.setStyle(sf::Text::Bold);
//         line.setString("#" + std::to_string(i + 1) + " Score: " +
//                        std::to_string(minutes) + "m " +
//                        std::to_string(seconds) + "s");
//         line.setPosition({320, y + i * 40});
//         scoreTexts.push_back(line);
//     }
// }

void Leaderboard::loadDummyScores()
{
    std::vector<int> top_scores;

    std::ifstream infile("scoreboard.txt");
    int x;
    while (infile >> x)
    {
        top_scores.push_back(x);
    }
    infile.close();

    std::sort(top_scores.begin(), top_scores.end()); // Ascending: lower time = better

    float y = 100.0f;
    for (size_t i = 0; i < top_scores.size() && i < 7; ++i)
    {
        int score = top_scores[i];
        int minutes = score / 60;
        int seconds = score % 60;

        std::stringstream ss;
        ss << "#" << (i + 1) << " Score: " << minutes << "m " << seconds << "s";
        std::string scoreLine = ss.str();

        sf::Text line(font, scoreLine, 24);
        line.setFillColor(sf::Color::Red);
        line.setPosition({320.0f, y + i * 40});

        scoreTexts.push_back(line);
    }

    // Show fallback if no scores were loaded
    if (top_scores.empty())
    {
        sf::Text line(font, "No scores found!", 24);
        line.setFillColor(sf::Color::Red);
        line.setPosition({320.0f, y});
        scoreTexts.push_back(line);
    }
}


void Leaderboard::handleEvents()
{
        while (std::optional<sf::Event> event = window.pollEvent())
        {
            if (event->is<sf::Event::Closed>())
                window.close();
        }
}

void Leaderboard::render()
{
    window.clear(/*sf::Color(30, 30, 30)*/); // Dark background
    // window.draw(bgImage);
    // window.draw(leaderboardTitle);
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
