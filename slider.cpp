#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include "option.hpp"

int main()
{
    sf::RenderWindow window(sf::VideoMode({800, 600}), "SFML Volume Slider");
    window.setFramerateLimit(60);

    SliderVolume slider(200, 300, 400);

    // sf::SoundBuffer buffer;
    // if (!buffer.loadFromFile("click-a.ogg")) return -1;

    // sf::Sound sound(buffer);
    // sound.setLooping(true);
    // sound.play();

    sf::Texture musicTexture;
    if (!musicTexture.loadFromFile("musicbtn.png"))
    {
        return -1; // Error if images not found
    }
    // Define Play Button
    // Create sprite
    sf::Sprite musicButton(musicTexture);
    musicButton.setPosition({390, 200});
    musicButton.setScale({0.3f, 0.3f}); // Scale down the button for better fit

    while (window.isOpen())
    {
        while (const std::optional event = window.pollEvent())
        {
            if (event->is<sf::Event::Closed>())
                window.close();
            slider.handleEvent(*event, window);
        }

        // sound.setVolume(slider.getVolume());

        window.clear(sf::Color::Black);
        slider.draw(window);
        window.draw(musicButton);
        window.display();
    }

    return 0;
}
