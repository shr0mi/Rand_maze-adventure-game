#ifndef OPTIONS_HPP
#define OPTIONS_HPP

#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <algorithm>
#include "audio.hpp"

class SliderVolume 
{
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

class OptionsScreen 
{
public:
    OptionsScreen();
    void draw(sf::RenderWindow& window);
    void handleEvent(const sf::Event& event, const sf::RenderWindow& window, AudioManager& audioManager);
    bool backClicked(sf::Vector2f pos);
    bool creditsClicked(sf::Vector2f pos);

private:
    sf::Texture bgTexture, backTexture, creditsTexture, musicOnTexture, musicOffTexture;
    sf::Sprite background, backButton, creditsButton, musicButton;
    bool musicOn = true;
    SliderVolume slider;
};

#endif
