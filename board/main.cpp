#include <SFML/Graphics.hpp>
#include <iostream>
#include <vector>
#include <algorithm>
#include "timer.h"            // GameTimer class
#include "leaderboard.hpp"    // Leaderboard class

#include <fstream>  // for file I/O

void OpenLeaderboard(GameTimer &gameTimer)
{
    std::cout << "You Win! Your Record: " << gameTimer.get_minutes() << " : " << gameTimer.get_seconds() << std::endl;

    std::vector<int> top_seven_scores;

    // Load from "scoreboard.txt"
    std::ifstream infile("scoreboard.txt");
    int x;
    while (infile >> x)
    {
        top_seven_scores.push_back(x);
    }
    infile.close();

    // Add current score
    int current_score = gameTimer.get_minutes() * 60 + gameTimer.get_seconds();
    top_seven_scores.push_back(current_score);
    std::sort(top_seven_scores.begin(), top_seven_scores.end());

    // Save top 7 scores
    std::ofstream outfile("scoreboard.txt");
    for (int i = 0; i < std::min(7, (int)top_seven_scores.size()); i++)
    {
        outfile << top_seven_scores[i] << std::endl;
        std::cout << top_seven_scores[i] << std::endl;
    }
    outfile.close();
}


int main()
{
    GameTimer gameTimer;
    gameTimer.start();

    // Simulate a game loop or a short wait to represent gameplay
    sf::Clock clock;
    while (clock.getElapsedTime().asSeconds() < 5) // simulate 5 seconds of gameplay
    {
        // Pretend game is running
    }

    gameTimer.pause(); // simulate game end

    // Save score and open leaderboard
    OpenLeaderboard(gameTimer);

    // Display leaderboard window
    Leaderboard leaderboard;
    leaderboard.run();

    return 0;
}
