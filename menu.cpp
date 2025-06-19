// Library
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <optional>

//  Main program
int main()
{
    // Create window
    sf::RenderWindow window(sf::VideoMode({960, 540}), "Main Menu", sf::Style::None);

    // Load font
    sf::Font font("arial.ttf");
    // Load textures
    sf::Texture menuTexture, menubgTexture, playTexture, optionTexture, exitTexture, nameTexture;
    if (!playTexture.loadFromFile("playbtn.png") || !optionTexture.loadFromFile("optionbtn.png") || !exitTexture.loadFromFile("exitbtn.png") || !nameTexture.loadFromFile("gameName.png") || !menuTexture.loadFromFile("menupic.png") || !menubgTexture.loadFromFile("menubg.png"))
    {
        return -1; // Error if images not found
    }

    //Menu background
    sf::Sprite menuBackground(menuTexture);
    menuBackground.setPosition({0, 0}); 
    menuBackground.setScale({1.0f, 1.0f}); 

    //Menu Image
    sf::Sprite menuImage(menubgTexture);
    menuImage.setPosition({-100, 0}); 
    menuImage.setScale({0.93f, 0.93f}); 

    // Load sound
    sf::SoundBuffer buffer("click-a.ogg");
    sf::Sound sound(buffer);

    // Load name texture
    sf::Sprite nameSprite(nameTexture);
    nameSprite.setPosition({120, 50}); // Position the name at the top
    nameSprite.setScale({0.25f, 0.25f}); // Scale down the name for better fit

    // Define Play Button
    // Create sprite
    sf::Sprite playButton(playTexture);
    playButton.setPosition({390, 200});
    playButton.setScale({0.3f, 0.3f}); // Scale down the button for better fit
    
    

    // Define Option Button
    // Create sprite
    sf::Sprite optionButton(optionTexture);
    optionButton.setPosition({390, 310});
    optionButton.setScale({0.3f, 0.3f});
    
    

    // Define Exit Button
    // Create sprite
    sf::Sprite exitButton(exitTexture);
    exitButton.setPosition({390, 420});
    exitButton.setScale({0.3f, 0.3f});

    
    bool isPaused = false;
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
                //auto mousePos = sf::Vector2f(sf::Mouse::getPosition(window));
                auto mousePos = window.mapPixelToCoords(sf::Mouse::getPosition(window));

                if (playButton.getGlobalBounds().contains(mousePos))
                {
                    playButton.setScale({0.8f, 0.8f});
                    sound.play(); // Play sound
                    sf::sleep(sf::milliseconds(200));

                    window.close(); // Exit from main window
                    std::system("main.exe");

                }
                else if (optionButton.getGlobalBounds().contains(mousePos))
                {
                    optionButton.setScale({0.8f, 0.8f});
                    sound.play(); // Play sound
                    sf::sleep(sf::milliseconds(200));

                    window.close(); // Exit from main window
                    std::system("slider.exe");
                    
                }
                else if (exitButton.getGlobalBounds().contains(mousePos))
                {
                    exitButton.setScale({0.95f, 0.95f});
                    sound.play(); // Play sound
                    sf::sleep(sf::milliseconds(200)); // Small delay to let sound play
                    window.close(); // Exit directly
                }
            }
            
        }

        window.clear(sf::Color::White);

        window.draw(menuBackground);
        window.draw(menuImage);
        window.draw(nameSprite);
        window.draw(playButton);
        window.draw(optionButton);
        window.draw(exitButton);
        window.display();
    }

    return 0;
}