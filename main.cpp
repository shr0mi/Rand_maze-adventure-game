#include <SFML/Graphics.hpp>
#include "player.h"

int main() {
    sf::RenderWindow window(sf::VideoMode({800, 600}), "Player Move");

    sf::Texture texture;
    if (!texture.loadFromFile("C:\\Users\\User\\Desktop\\Game Project\\kenney_1-bit-pack\\Tilesheet\\colored-transparent.png"))
        return -1;

    Player player(texture, 3, 24);

    sf::Clock clock;

    while (window.isOpen()) {
        while (const std::optional<sf::Event> event = window.pollEvent()) {
            if (event->is<sf::Event::Closed>())
                window.close();
        }

        float dt = clock.restart().asSeconds();
        player.update(dt);

        window.clear();
        player.draw(window);
        window.display();
    }
    return 0;
}
