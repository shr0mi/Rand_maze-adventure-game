#ifndef SLIDER_VOLUME_HPP
#define SLIDER_VOLUME_HPP

#include <SFML/Graphics.hpp>
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <variant>
#include <algorithm>

class SliderVolume {
public:
    SliderVolume(float x, float y, float width);

    void handleEvent(const sf::Event& event, const sf::RenderWindow& window);
    void draw(sf::RenderWindow& window) const;

    float getVolume() const;

private:
    sf::RectangleShape bar;
    sf::CircleShape knob;
    sf::Font font;
    sf::Text volumeText;
    bool isDragging;

    float minX, maxX;

    void updateKnobPosition(float mouseX);
    void updateText();
};

#endif
