#pragma once
#include <SFML/System.hpp>

class GameTimer {
public:
    GameTimer();

    void start();
    void pause();
    void resume();

    int get_minutes() const;
    int get_seconds() const;

private:
    sf::Clock clock;
    sf::Time accumulated;
    bool isRunning;
};
