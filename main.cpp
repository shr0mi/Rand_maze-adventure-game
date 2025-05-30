#include <SFML/Graphics.hpp>
#include "player.h"
#include "enemy.hpp"
#include "mapgen.h"

int main() {
    sf::RenderWindow window(sf::VideoMode({500, 500}), "Player Move");

    window.setMouseCursorVisible(false);
    sf::Texture texture;
    if (!texture.loadFromFile("C:/Users/User/Desktop/Game Project/kenney_1-bit-pack/Tilesheet/colored-transparent.png")) //path change kore nis
        return -1;


    // Level Vector
    // Trees -> 50, 51, 52, 53, 54, 55
    //Walls -> 19, 20, 21, 68, *69, 70, 117, 118, 119
    //Floor -> 2, 3, 4, 5, 
    std::vector<std::vector<int>> level={
        {50, 51, 50, 51, 52, 50, 54, 55, 50, 51},
        {51, 19, 20, 20, 20, 20, 20, 20, 21, 54},
        {51, 68, 2, 3, 4, 5, 2, 2, 70, 54},
        {51, 68, 2, 3, 4, 5, 2, 2, 70, 54},
        {51, 68, 2, 3, 4, 5, 2, 2, 70, 54},
        {51, 68, 2, 3, 4, 5, 2, 2, 70, 54},
        {51, 68, 2, 3, 4, 5, 2, 2, 70, 54},
        {51, 68, 2, 3, 4, 5, 2, 2, 70, 54},
        {51, 117, 118, 118, 118, 118, 118, 118, 119, 54},
        {50, 51, 50, 51, 52, 50, 54, 55, 50, 51}
        
    };

    //Render Tilemap
    TileMap map;
    if(!map.load("C:/Users/User/Desktop/Game Project/kenney_1-bit-pack/Tilesheet/colored-transparent.png", sf::Vector2u(832, 373), level, 16, 16, 1))
    {
        std::cerr << "Failed to load tilemap!" << std::endl;
        return -1;
    }    

    Player player(texture, 2, 24);

    Crosshair crosshair(texture , 14, 21);


    /*std::vector<Key> keys;
    keys.emplace_back(texture, sf::Vector2f(200.f, 100.f),32,11);
    keys.emplace_back(texture,  sf::Vector2f(300.f, 100.f),33,11);
    keys.emplace_back(texture,  sf::Vector2f(400.f, 100.f),34,11);
    // Create chest
    Chest chest(texture, sf::Vector2f(400.f, 300.f));*/

    std::vector<Bullet> bullets;
    std::vector<EnemyBullet> enmbull;

    sf::Clock clock;

    while (window.isOpen()) {
        while (const std::optional<sf::Event> event = window.pollEvent()) {
            if (event->is<sf::Event::Closed>())
                window.close();
        }




        float dt = clock.restart().asSeconds();

        handleShooting(bullets, texture, player.getPosition(), window);
        updateBullets(bullets, dt, window);
        
       // handleKeyChestInteraction(keys, chest,player.getPosition());

   



        player.update(dt);
        crosshair.update(window);
        window.clear();
       window.clear();
  window.setView(player.getView());
for (const auto& bullet : bullets)
    bullet.draw(window);
/*for (const auto& key : keys)
    key.draw(window);  
chest.draw(window);*/
window.draw(map);
player.draw(window);
crosshair.draw(window);
window.display();

    }
    return 0;
}
