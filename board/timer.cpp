#include "timer.h"

GameTimer::GameTimer()
    : accumulated(sf::Time::Zero), isRunning(false) {}

void GameTimer::start() {
    accumulated = sf::Time::Zero;
    clock.restart();
    isRunning = true;
}

void GameTimer::pause() {
    if (isRunning) {
        accumulated += clock.getElapsedTime();
        isRunning = false;
    }
}

void GameTimer::resume() {
    if (!isRunning) {
        clock.restart();
        isRunning = true;
    }
}

int GameTimer::get_minutes() const {
    sf::Time totalTime = accumulated;
    if (isRunning) {
        totalTime += clock.getElapsedTime();
    }
    return static_cast<int>(totalTime.asSeconds()) / 60;
}

int GameTimer::get_seconds() const {
    sf::Time totalTime = accumulated;
    if (isRunning) {
        totalTime += clock.getElapsedTime();
    }
    return static_cast<int>(totalTime.asSeconds()) % 60;
}
