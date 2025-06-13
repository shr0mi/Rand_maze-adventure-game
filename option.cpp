#include "option.hpp"
#include <SFML/Graphics.hpp>
#include <algorithm> // for std::clamp

SliderVolume::SliderVolume(float x, float y, float width)
    : isDragging(false), volumeText(font, "", 18)
{
    bar.setSize(sf::Vector2f(width, 5));
    bar.setFillColor(sf::Color(150, 150, 255));
    bar.setPosition({x, y});

    knob.setRadius(10);
    knob.setFillColor(sf::Color::Red);
    knob.setOrigin({knob.getRadius(), knob.getRadius()});
    knob.setPosition({x + width / 2, y + bar.getSize().y / 2});

    minX = x;
    maxX = x + width;

    if (!font.openFromFile("arial.ttf")) {
        // Fallback: just ignore text rendering
    }

    // volumeText.setFont(font);
    // volumeText.setCharacterSize(18);
    volumeText.setFillColor(sf::Color::White);
    updateText();
}

void SliderVolume::handleEvent(const sf::Event& event, const sf::RenderWindow& window)
{
    if (const auto* mousePressed = event.getIf<sf::Event::MouseButtonPressed>())
    {
        if (mousePressed->button == sf::Mouse::Button::Left)
        {
            sf::Vector2i pixel(mousePressed->position.x, mousePressed->position.y);
            sf::Vector2f mousePos = window.mapPixelToCoords(pixel);
            if (knob.getGlobalBounds().contains(mousePos))
            {
                isDragging = true;
            }
        }
    }
    else if (const auto* mouseReleased = event.getIf<sf::Event::MouseButtonReleased>())
    {
        if (mouseReleased->button == sf::Mouse::Button::Left)
        {
            isDragging = false;
        }
    }
    else if (const auto* mouseMoved = event.getIf<sf::Event::MouseMoved>())
    {
        if (isDragging)
        {
            sf::Vector2i pixel(mouseMoved->position.x, mouseMoved->position.y);
            sf::Vector2f mousePos = window.mapPixelToCoords(pixel);
            updateKnobPosition(mousePos.x);
        }
    }
}

void SliderVolume::updateText()
{
    int volume = static_cast<int>(getVolume());
    volumeText.setString(std::to_string(volume));

    // Center text above knob
    sf::FloatRect bounds = volumeText.getLocalBounds();
    volumeText.setOrigin({bounds.size.x / 2, bounds.size.y});
    volumeText.setPosition({knob.getPosition().x, knob.getPosition().y - 20});
}


void SliderVolume::updateKnobPosition(float mouseX)
{
    float x = std::clamp(mouseX, minX, maxX);
    knob.setPosition({x, bar.getPosition().y + bar.getSize().y / 2});
    updateText();
}

float SliderVolume::getVolume() const
{
    float position = knob.getPosition().x;
    float volume = ((position - minX) / (maxX - minX)) * 100.0f;
    return volume;
}

void SliderVolume::draw(sf::RenderWindow& window) const
{
    window.draw(bar);
    window.draw(knob);
    window.draw(volumeText);
}
