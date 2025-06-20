#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include "option.hpp"
#include "audio.hpp"

int main()
{
    sf::RenderWindow window(sf::VideoMode({800, 800}), "SFML Volume Slider");
    window.setFramerateLimit(60);

    sf::Texture pauseTexture, menubgTexture, musicbtnTexture, musicbtnoffTexture, menuTexture, resumeTexture;
    if (!pauseTexture.loadFromFile("ui_pause_menu_title.png") || !menubgTexture.loadFromFile("menubg.png") || !musicbtnTexture.loadFromFile("ui_music_btn_on.png") || !musicbtnoffTexture.loadFromFile("ui_music_btn_off.png") || !menuTexture.loadFromFile("MenuButton.png") || !resumeTexture.loadFromFile("ResumeButton.png"))
    {
        return -1; // Error if images not found
    }

    // Menu Image
    sf::Sprite menuImage(menubgTexture);
    menuImage.setPosition({-100, 50});
    menuImage.setScale({1.3f, 1.3f});

    sf::Sprite pauseImage(pauseTexture);
    pauseImage.setPosition({210, 50});
    pauseImage.setScale({0.3f, 0.3f});

    sf::Sprite musicbtnImage(musicbtnTexture);
    musicbtnImage.setPosition({250, 300});
    musicbtnImage.setScale({0.2f, 0.2f});

    sf::Sprite musicbtnoffImage(musicbtnoffTexture);
    musicbtnoffImage.setPosition({250, 300});
    musicbtnoffImage.setScale({0.2f, 0.2f});

    sf::Sprite menuButton(menuTexture);
    menuButton.setPosition({300, 450});
    menuButton.setScale({0.3f, 0.3f});

    sf::Sprite resumeButton(resumeTexture);
    resumeButton.setPosition({300, 600});
    resumeButton.setScale({0.3f, 0.3f});

    AudioManager audioManager;
    SliderVolume slider(200, 250, 400);

    sf::Texture musicTexture;
    if (!musicTexture.loadFromFile("musicbtn.png"))
    {
        return -1; // Error if images not found
    }

    // Define Play Button
    // Create sprite
    sf::Sprite musicButton(musicTexture);
    musicButton.setPosition({100, 210});
    musicButton.setScale({0.3f, 0.3f}); // Scale down the button for better fit

    bool isToggled = true; // Initial state of the music button

    while (window.isOpen())
    {
        while (const std::optional event = window.pollEvent())
        {
            if (event->is<sf::Event::Closed>())
            {
                window.close();
            }
            if (event->is<sf::Event::MouseButtonPressed>())
            {
                // auto mousePos = sf::Vector2f(sf::Mouse::getPosition(window));
                auto mousePos = window.mapPixelToCoords(sf::Mouse::getPosition(window));

                if (musicbtnImage.getGlobalBounds().contains(mousePos))
                {
                    audioManager.toggleMusic();
                    isToggled = !isToggled; // Toggle the state
                }
                if (menuButton.getGlobalBounds().contains(mousePos))
                {
                    if (isToggled)
                    {
                        audioManager.toggleMusic(); // Ensure it's stopped
                        isToggled = false;
                    }
                    window.close(); // Close the options window
                    std::system("menu.exe");
                }
                if (resumeButton.getGlobalBounds().contains(mousePos))
                {
                    if (isToggled)
                    {
                        audioManager.toggleMusic(); // Ensure it's stopped
                        isToggled = false;
                    }
                    window.close();          // Close the options window
                    //std::system("main.exe"); // resume the game
                }
            }
            slider.handleEvent(*event, window);
        }

        // 🔊 Set volume based on slider
        audioManager.setVolume(slider.getVolume());

        window.clear();
        window.draw(menuImage);
        window.draw(pauseImage);
        if (isToggled)
        {
            window.draw(musicbtnImage);
        }
        else
        {
            window.draw(musicbtnoffImage);
        }
        window.draw(menuButton);
        window.draw(resumeButton);
        slider.draw(window);
        window.draw(musicButton);

        window.display();
    }

    return 0;
}