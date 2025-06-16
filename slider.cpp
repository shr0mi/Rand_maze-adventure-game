#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include "option.hpp"
#include "audio.hpp"

int main()
{
    sf::RenderWindow window(sf::VideoMode({800, 800}), "SFML Volume Slider");
    window.setFramerateLimit(60);

    sf::Texture optionTexture, menubgTexture, creditTexture;
    if (!optionTexture.loadFromFile("ui_options_title.png") || !menubgTexture.loadFromFile("menubg.png") || !creditTexture.loadFromFile("ui_credits_btn.png"))
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

    sf::Sprite creditButton(creditTexture);
    creditButton.setPosition({210, 500});
    creditButton.setScale({0.3f, 0.3f});

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

                if (musicButton.getGlobalBounds().contains(mousePos))
                {
                    audioManager.toggleMusic();
                }
            }
            slider.handleEvent(*event, window);
        }

        // 🔊 Set volume based on slider
        audioManager.setVolume(slider.getVolume());

        window.clear();
        window.draw(menuImage);
        window.draw(optionImage);
        window.draw(creditButton);
        slider.draw(window);
        window.draw(musicButton);
        
        window.display();
    }

    return 0;
}
