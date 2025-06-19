#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include "option.hpp"
#include "audio.hpp"

int main()
{
    sf::RenderWindow window(sf::VideoMode({800, 800}), "Credits", sf::Style::None);
    window.setFramerateLimit(60);

    sf::Texture menubgTexture, creditTexture, backTexture;
    if (!menubgTexture.loadFromFile("menubg.png") || !creditTexture.loadFromFile("ui_credits.png") || !backTexture.loadFromFile("backbtn.png"))
    {
        return -1; // Error if images not found
    }

    sf::Sprite menuImage(menubgTexture);
    menuImage.setPosition({-100, 50}); 
    menuImage.setScale({1.3f, 1.3f});

    sf::Sprite creditImage(creditTexture);
    creditImage.setPosition({120, 50}); 
    creditImage.setScale({0.3f, 0.3f});

    sf::Sprite backButton(backTexture);
    backButton.setPosition({300, 600});
    backButton.setScale({0.3f, 0.3f});

    // AudioManager audioManager;

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

                // if (musicbtnImage.getGlobalBounds().contains(mousePos))
                // {
                //     audioManager.toggleMusic();
                //     isToggled = !isToggled; // Toggle the state
                // }

                if (backButton.getGlobalBounds().contains(mousePos))
                {
                    window.close(); // Close the options window
                    std::system("slider.exe"); // Open main menu
                }
            }
        }

        window.clear();
        window.draw(menuImage);
        window.draw(creditImage);
        window.draw(backButton);        
        window.display();
    }

    return 0;
}
