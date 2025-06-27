#ifndef PAUSEMENU_HPP
#define PAUSEMENU_HPP

#include <SFML/Graphics.hpp>
#include "audio.hpp"
#include "options.hpp"

class PauseMenu
{
public:
    PauseMenu();
    void draw(sf::RenderWindow& window);
    void handleEvent(const sf::Event& event, const sf::RenderWindow& window, AudioManager& audioManager);

    bool isResumeClicked(sf::Vector2f pos) const;
    bool isMenuClicked(sf::Vector2f pos) const;
    bool isMusicToggled() const;

private:
    sf::Texture pauseTexture, /*bgTexture,*/ musicOnTexture, musicOffTexture, menuTexture, resumeTexture;
    sf::Sprite /*background,*/ pauseTitle, musicOnButton, musicOffButton, menuButton, resumeButton;
    SliderVolume slider;
    bool musicOn = true;
};

#endif // PAUSEMENU_HPP
