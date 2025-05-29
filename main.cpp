#include <SFML/Graphics.hpp>
#include "player.h"

int main() {
    sf::RenderWindow window(sf::VideoMode({800, 600}), "Player Move");

    window.setMouseCursorVisible(false);
    sf::Texture texture;
    if (!texture.loadFromFile("C:/Users/User/Desktop/Game Project/kenney_1-bit-pack/Tilesheet/colored-transparent.png")) //path change kore nis
        return -1;

    Player player(texture, 2, 24);

    Crosshair crosshair(texture , 14, 21);

    std::vector<Key> keys;
    keys.emplace_back(texture, sf::Vector2f(200.f, 100.f),32,11);
    keys.emplace_back(texture,  sf::Vector2f(300.f, 100.f),33,11);
    keys.emplace_back(texture,  sf::Vector2f(400.f, 100.f),34,11);
    // Create chest
    Chest chest(texture, sf::Vector2f(400.f, 300.f));

    std::vector<Bullet> bullets;

    sf::Clock clock;

    while (window.isOpen()) {
        while (const std::optional<sf::Event> event = window.pollEvent()) {
            if (event->is<sf::Event::Closed>())
                window.close();
        }





        float dt = clock.restart().asSeconds();

        handleShooting(bullets, texture, player.getPosition(), window);
        updateBullets(bullets, dt, window);
        
        handleKeyChestInteraction(keys, chest,player.getPosition());




        player.update(dt);
        crosshair.update(window);
        window.clear();
       window.clear();
player.draw(window);
crosshair.draw(window);
for (const auto& bullet : bullets)
    bullet.draw(window);
for (const auto& key : keys)
    key.draw(window);
chest.draw(window);
window.display();

    }
    return 0;
}
