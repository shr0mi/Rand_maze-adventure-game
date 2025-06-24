#include "pause.hpp"

PauseMenu::PauseMenu() : bgTexture("menubg.png"),
                         pauseTexture("ui_pause_menu_title.png"),
                         musicOnTexture("ui_music_btn_on.png"),
                         musicOffTexture("ui_music_btn_off.png"),
                         menuTexture("MenuButton.png"),
                         resumeTexture("ResumeButton.png"),

                         background(bgTexture),
                         pauseTitle(pauseTexture),
                         musicOnButton(musicOnTexture),
                         musicOffButton(musicOffTexture),
                         menuButton(menuTexture),
                         resumeButton(resumeTexture),

                         slider(200, 250, 400),
                         musicOn(true)
{

    background.setPosition({-100, 50});
    background.setScale({1.3f, 1.3f});

    pauseTitle.setPosition({210, 50});
    pauseTitle.setScale({0.3f, 0.3f});

    musicOnButton.setPosition({250, 300});
    musicOnButton.setScale({0.2f, 0.2f});

    musicOffButton.setPosition({250, 300});
    musicOffButton.setScale({0.2f, 0.2f});

    menuButton.setPosition({300, 450});
    menuButton.setScale({0.3f, 0.3f});

    resumeButton.setPosition({300, 600});
    resumeButton.setScale({0.3f, 0.3f});
}

void PauseMenu::handleEvent(const sf::Event &event, const sf::RenderWindow &window, AudioManager &audioManager)
{
    slider.handleEvent(event, window);
    audioManager.setVolume(slider.getVolume());

    if (event.is<sf::Event::MouseButtonPressed>())
    {
        sf::Vector2f pos = window.mapPixelToCoords(sf::Mouse::getPosition(window));

        if (musicOnButton.getGlobalBounds().contains(pos))
        {
            audioManager.toggleMusic();
            musicOn = !musicOn;
        }
    }
}

void PauseMenu::draw(sf::RenderWindow &window)
{
    window.draw(background);
    window.draw(pauseTitle);
    if (musicOn)
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

bool PauseMenu::isMusicToggled() const
{
    return musicOn;
}
