#include "pause.hpp"
#include "audio.hpp"

PauseMenu::PauseMenu() : pauseTexture("assets/ui_pause_menu_title.png"),
                         musicOnTexture("assets/ui_music_btn_on.png"),
                         musicOffTexture("assets/ui_music_btn_off.png"),
                         menuTexture("assets/MenuButton.png"),
                         resumeTexture("assets/ResumeButton.png"),

                         pauseTitle(pauseTexture),
                         musicOnButton(musicOnTexture),
                         musicOffButton(musicOffTexture),
                         menuButton(menuTexture),
                         resumeButton(resumeTexture),

                         slider(320, 350, 400) // Initialize the slider with position (320, 350) and width 400
{

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
    slider.handleEvent(event, window);          // Handle slider events
    audioManager.setVolume(slider.getVolume()); // Sync audio volume with slider

    if (event.is<sf::Event::MouseButtonPressed>())
    {
        sf::Vector2f pos = window.mapPixelToCoords(sf::Mouse::getPosition(window));

        if (musicOnButton.getGlobalBounds().contains(pos) || musicOffButton.getGlobalBounds().contains(pos))
        {
            audioManager.toggleMusic(); // Toggle music on/off
        }
    }
}

void PauseMenu::draw(sf::RenderWindow &window, AudioManager &audioManager)
{
    window.draw(pauseTitle);

    // Draw the music button based on the current state
    if (audioManager.isMusicOn())
    {
        window.draw(musicOnButton);
    }
    else
    {
        window.draw(musicOffButton);
    }

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