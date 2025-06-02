#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include "option.hpp"

int main() {
    sf::RenderWindow window(sf::VideoMode({800, 600}), "SFML Volume Slider");
    window.setFramerateLimit(60);

    SliderVolume slider(200, 300, 400);

    // sf::SoundBuffer buffer;
    // if (!buffer.loadFromFile("click-a.ogg")) return -1;

    // sf::Sound sound(buffer);
    // sound.setLooping(true);
    // sound.play();

    while (window.isOpen())
    {
        while (const std::optional event = window.pollEvent())
        {
            if (event->is<sf::Event::Closed>())
                window.close();
            slider.handleEvent(*event, window);

        }

        //sound.setVolume(slider.getVolume());

        window.clear(sf::Color::Black);
        slider.draw(window);
        window.display();
    }

    return 0;
}
