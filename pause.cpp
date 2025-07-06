#include "pause.hpp"
#include "audio.hpp"

PauseMenu::PauseMenu() : // bgTexture("assets/menubg.png"),
                         pauseTexture("assets/ui_pause_menu_title.png"),
                         musicOnTexture("assets/ui_music_btn_on.png"),
                         musicOffTexture("assets/ui_music_btn_off.png"),
                         menuTexture("assets/MenuButton.png"),
                         resumeTexture("assets/ResumeButton.png"),

                         // background(bgTexture),
                         pauseTitle(pauseTexture),
                         musicOnButton(musicOnTexture),
                         musicOffButton(musicOffTexture),
                         menuButton(menuTexture),
                         resumeButton(resumeTexture),

                         slider(320, 350, 400)
{

    // background.setPosition({-100, 50});
    // background.setScale({1.3f, 1.3f});

    pauseTitle.setPosition({270, 200});
    pauseTitle.setScale({0.4f, 0.4f});

    musicOnButton.setPosition({350, 450});
    musicOnButton.setScale({0.2f, 0.2f});

    musicOffButton.setPosition({350, 450});
    musicOffButton.setScale({0.2f, 0.2f});

    menuButton.setPosition({410, 600});
    menuButton.setScale({0.3f, 0.3f});

    resumeButton.setPosition({410, 750});
    resumeButton.setScale({0.3f, 0.3f});
}

void PauseMenu::handleEvent(const sf::Event &event, const sf::RenderWindow &window, AudioManager &audioManager)
{
    slider.handleEvent(event, window);
    audioManager.setVolume(slider.getVolume());

    if (event.is<sf::Event::MouseButtonPressed>())
    {
        sf::Vector2f pos = window.mapPixelToCoords(sf::Mouse::getPosition(window));

        if (musicOnButton.getGlobalBounds().contains(pos) || musicOffButton.getGlobalBounds().contains(pos))
        {
            audioManager.toggleMusic();
        }
    }
}

void PauseMenu::draw(sf::RenderWindow &window, AudioManager &audioManager)
{
    // window.draw(background);
    window.draw(pauseTitle);

    if (audioManager.isMusicOn())
        window.draw(musicOnButton);
    else
        window.draw(musicOffButton);

    window.draw(menuButton);
    window.draw(resumeButton);
    slider.draw(window);
}

bool PauseMenu::isResumeClicked(sf::Vector2f pos) const
{
    return resumeButton.getGlobalBounds().contains(pos);
}

bool PauseMenu::isMenuClicked(sf::Vector2f pos) const
{
    return menuButton.getGlobalBounds().contains(pos);
}

void PauseMenu::syncWithAudio(AudioManager &audioManager)
{
    slider.setVolume(audioManager.getVolume());
}