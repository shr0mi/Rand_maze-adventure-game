#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include "option.hpp"
#include "audio.hpp"

int main()
{
    sf::RenderWindow window(sf::VideoMode({800, 800}), "SFML Volume Slider");
    window.setFramerateLimit(60);

    sf::Texture optionTexture, menubgTexture, musicbtnTexture, musicbtnoffTexture, creditTexture, backTexture;
    if (!optionTexture.loadFromFile("ui_options_title.png") || !menubgTexture.loadFromFile("menubg.png") || !musicbtnTexture.loadFromFile("ui_music_btn_on.png") || !musicbtnoffTexture.loadFromFile("ui_music_btn_off.png") || !creditTexture.loadFromFile("ui_credits_btn.png") || !backTexture.loadFromFile("backbtn.png"))
    {
        return -1; // Error if images not found
    }

    //Menu Image
    sf::Sprite menuImage(menubgTexture);
    menuImage.setPosition({-100, 50}); 
    menuImage.setScale({1.3f, 1.3f});

    sf::Sprite optionImage(optionTexture);
    optionImage.setPosition({210, 50}); 
    optionImage.setScale({0.2f, 0.2f});

    sf::Sprite musicbtnImage(musicbtnTexture);
    musicbtnImage.setPosition({250, 300}); 
    musicbtnImage.setScale({0.2f, 0.2f});

    sf::Sprite musicbtnoffImage(musicbtnoffTexture);
    musicbtnoffImage.setPosition({250, 300}); 
    musicbtnoffImage.setScale({0.2f, 0.2f});

    sf::Sprite creditButton(creditTexture);
    creditButton.setPosition({300, 450});
    creditButton.setScale({0.2f, 0.2f});

    sf::Sprite backButton(backTexture);
    backButton.setPosition({300, 600});
    backButton.setScale({0.3f, 0.3f});

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
            }
            slider.handleEvent(*event, window);
        }

        // 🔊 Set volume based on slider
        audioManager.setVolume(slider.getVolume());

        window.clear();
        window.draw(menuImage);
        window.draw(optionImage);
        if (isToggled)
        {
            window.draw(musicbtnImage);
        }
        else
        {
            window.draw(musicbtnoffImage);
        }
        window.draw(creditButton);
        window.draw(backButton);
        slider.draw(window);
        window.draw(musicButton);
        
        window.display();
    }

    return 0;
}
