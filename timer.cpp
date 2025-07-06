// timer.cpp
#include "timer.h"
#include <sstream>
#include <iostream>

GameTimer::GameTimer() : totalTime(0.0f), timerText(font, "", 30)
{
    if (!font.openFromFile("assets/arial.ttf"))  // Make sure the font exists
    {
        std::cerr << "Failed to load font!" << std::endl;
    }

    timerText.setFillColor(sf::Color::White);
    timerText.setOutlineColor(sf::Color::Black);
    timerText.setOutlineThickness(2);
    timerText.setPosition({10, 10});
}

void GameTimer::update(float dt, bool isPaused)
{
    if (!isPaused)
    {
        totalTime += dt;
        updateText();
    }
}

void GameTimer::updateText()
{
    int totalSeconds = static_cast<int>(totalTime);
    int minutes = totalSeconds / 60;
    int seconds = totalSeconds % 60;

    std::stringstream ss;
    ss << "Time: ";
    ss << (minutes < 10 ? "0" : "") << minutes << ":";
    ss << (seconds < 10 ? "0" : "") << seconds;

    timerText.setString(ss.str());
}


void GameTimer::draw(sf::RenderWindow &window)
{
    sf::View currentView = window.getView();
    window.setView(window.getDefaultView()); // Draw timer in screen space
    window.draw(timerText);
    window.setView(currentView); // Restore previous view
}

//Return Minutes
int GameTimer::get_minutes(){
    int totalSeconds = static_cast<int>(totalTime);
    int minutes = totalSeconds / 60;
    return minutes;
}

//Return seconds
int GameTimer::get_seconds(){
    int totalSeconds = static_cast<int>(totalTime);
    int seconds = totalSeconds % 60;
    return seconds;
}
