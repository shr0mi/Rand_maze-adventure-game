// Library
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <optional>

//  Main program
int main()
{
    // Create window
    sf::RenderWindow window(sf::VideoMode({450, 400}), "Main Menu");

    // Load font
    sf::Font font("arial.ttf");
    // Load textures
    sf::Texture playTexture, optionTexture, exitTexture;
    if (!playTexture.loadFromFile("button_rectangle_border.png") || !optionTexture.loadFromFile("button_rectangle_border.png") || !exitTexture.loadFromFile("button_rectangle_border.png")) 
    {
        return -1; // Error if images not found
    }
    // Load sound
    sf::SoundBuffer buffer("click-a.ogg");
    sf::Sound sound(buffer);

    // Create Title
    sf::Text text(font, "Unstructured Adventure", 30);
    text.setPosition({50, 60});
    text.setFillColor(sf::Color::Black);
    text.setStyle(sf::Text::Italic | sf::Text::Bold);

    // Define Play Button
    // Create sprite
    sf::Sprite playButton(playTexture);
    playButton.setPosition({100, 100});
    
    sf::Text playText(font, "Play", 24);
    playText.setFillColor(sf::Color::Black);
    playText.setPosition({160, 110});
    playText.setStyle(sf::Text::Bold);

    // Define Option Button
    // Create sprite
    sf::Sprite optionButton(optionTexture);
    optionButton.setPosition({100, 180});
    
    sf::Text optionText(font, "Option", 24);
    optionText.setFillColor(sf::Color::Black);
    optionText.setPosition({155, 190});
    optionText.setStyle(sf::Text::Bold);

    // Define Exit Button
    // Create sprite
    sf::Sprite exitButton(exitTexture);
    exitButton.setPosition({100, 260});

    sf::Text exitText(font, "Exit", 24);
    exitText.setFillColor(sf::Color::Black);
    exitText.setPosition({165, 270});
    exitText.setStyle(sf::Text::Bold);

    // While window is still open
    while (window.isOpen())
    {
        // Handle events
        while (std::optional event = window.pollEvent())
        {
            // When close button is clicked
            if (event->is<sf::Event::Closed>())
            {
                // Close window
                window.close();
            }
            if (event->is<sf::Event::MouseButtonPressed>())
            {
                auto mousePos = sf::Vector2f(sf::Mouse::getPosition(window));

                if (playButton.getGlobalBounds().contains(mousePos))
                {
                    sound.play(); // Play sound
                    window.close(); // Exit from main window

                    // Play button clicked: open a new window briefly
                    sf::RenderWindow playWindow(sf::VideoMode({450, 400}), "Game Window");
                    playWindow.clear(sf::Color::White);
                    playWindow.display();

                    sf::sleep(sf::seconds(2)); // Show for 2 seconds
                    playWindow.close();
                    
                }
                else if (optionButton.getGlobalBounds().contains(mousePos))
                {
                    sound.play(); // Play sound
                    window.close(); // Exit from main window

                    // Option button clicked: open a new window briefly
                    sf::RenderWindow playWindow(sf::VideoMode({450, 400}), "Settings");
                    playWindow.clear(sf::Color::White);
                    playWindow.display();

                    sf::sleep(sf::seconds(2)); // Show for 2 seconds
                    playWindow.close();
                    
                }
                else if (exitButton.getGlobalBounds().contains(mousePos))
                {
                    sound.play(); // Play sound
                    sf::sleep(sf::milliseconds(200)); // Small delay to let sound play
                    window.close(); // Exit directly
                }
            }
        }

        window.clear(sf::Color::White);

        window.draw(text);
        window.draw(playButton);
        window.draw(playText);

        window.draw(optionButton);
        window.draw(optionText);

        window.draw(exitButton);
        window.draw(exitText);
        window.display();
    }

    return 0;
}
