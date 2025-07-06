// timer.h
#ifndef TIMER_H
#define TIMER_H

#include <SFML/Graphics.hpp>
#include <string>

class GameTimer
{
public:
    GameTimer();

    void update(float dt, bool isPaused, bool isOver);
    void draw(sf::RenderWindow &window);
    int get_minutes();
    int get_seconds();

    void setKeyCount(int count); // Set the number of keys collected

private:
    float totalTime;

    int keyCount = 0; // Initialize number of keys collected
    sf::Font font;
    sf::Text timerText;

    void updateText();
};

#endif
