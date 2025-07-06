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

    void handleEvent(const sf::Event &event, const sf::RenderWindow &window);
    void draw(sf::RenderWindow &window) const;

    float getVolume() const;
    void setVolume(float volume);

    void setAudioManager(AudioManager *a) { audio = a; }

private:
    sf::RectangleShape bar;
    sf::CircleShape knob;
    sf::Font font;
    sf::Text volumeText;
    bool isDragging; // Flag to check if the knob is being dragged

    float minX, maxX;

    void updateKnobPosition(float mouseX);

    AudioManager *audio = nullptr; // Pointer to AudioManager for volume control

    void updateText();
};

class OptionsScreen
{
public:
    OptionsScreen(AudioManager &audioManager);
    void draw(sf::RenderWindow &window);
    void handleEvent(const sf::Event &event, const sf::RenderWindow &window, AudioManager &audioManager);
    bool backClicked(sf::Vector2f pos);
    bool creditsClicked(sf::Vector2f pos);
    void syncWithAudio(AudioManager &audioManager);

private:
    sf::Texture bgTexture, optionTexture, musicTexture, backTexture, creditsTexture, musicOnTexture, musicOffTexture;
    sf::Sprite background, optionImage, musicImage, backButton, creditsButton, musicOnButton, musicOffButton;
    bool musicOn = true; // Track music state
    SliderVolume slider; // Slider for volume control

    AudioManager *audio = nullptr; // Pointer to AudioManager for volume control
};

#endif
