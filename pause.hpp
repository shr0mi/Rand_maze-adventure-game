#ifndef PAUSEMENU_HPP
#define PAUSEMENU_HPP

#include <SFML/Graphics.hpp>
#include "audio.hpp"
#include "options.hpp"

class PauseMenu
{
public:
    PauseMenu();
    void draw(sf::RenderWindow &window, AudioManager &audioManager);
    void handleEvent(const sf::Event &event, const sf::RenderWindow &window, AudioManager &audioManager);

    bool isResumeClicked(sf::Vector2f pos) const;
    bool isMenuClicked(sf::Vector2f pos) const;

    void syncWithAudio(AudioManager &audioManager); // New

private:
    sf::Texture pauseTexture, musicOnTexture, musicOffTexture, menuTexture, resumeTexture;
    sf::Sprite pauseTitle, musicOnButton, musicOffButton, menuButton, resumeButton;
    SliderVolume slider;
};

#endif // PAUSEMENU_HPP
