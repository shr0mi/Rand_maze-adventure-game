#include <SFML/Graphics.hpp>
#include "player.h"
#include "enemy.hpp"
#include "mapgen.h"
#include "bsp_algorithm.h"

std::vector<std::vector<int>> generateCollisionMap(std::vector<std::vector<int>>& level)
{
    std::vector<std::vector<int>> collisionMap(level.size(), std::vector<int>(level[0].size(), 0));
    for (int i = 0; i < level.size(); i++)
    {
        for (int j = 0; j < level[i].size(); j++)
        {
            if( (50 <= level[i][j] && level[i][j] <= 55) || level[i][j] == 12 || level[i][j] == 638){
                collisionMap[i][j] = 1;
            }else{
                collisionMap[i][j] = 0;
            }
        }
    }

    return collisionMap;
}


int main() {
std::cerr << "Terminal working" << std::endl;
    sf::RenderWindow window(sf::VideoMode({1000, 1000}), "TileMap works!");

    // Level Vector
    // Trees -> 50, 51, 52, 53, 54, 55
    //Walls -> 19, 20, 21, 68, *69, 70, 117, 118, 119
    //Floor -> 2, 3, 4, 5, 
    /*w
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
        {50, 51, 50, 51, 52, 50, 54, 55, 50, 51},
    };
    */
   
    window.setMouseCursorVisible(false);
    
    BSP_algorithm bsp;
    std::vector<std::vector<int>> level = bsp.generate_bsp_map(200, 200);

    std::vector<std::vector<int>> collisionMap = generateCollisionMap(level);




    //Player
    sf::Texture texture;
    if (!texture.loadFromFile("C:/Users/User/Desktop/Game Project/kenney_1-bit-pack/Tilesheet/colored-transparent.png"))
        return -1;

    Player player(texture, 3, 24,collisionMap);
    Crosshair crosshair(texture , 14, 21);
    std::vector<Bullet> bullets;

    sf::Clock clock;

     TileMap map;
    if(!map.load("C:/Users/User/Desktop/Game Project/kenney_1-bit-pack/Tilesheet/colored-transparent.png", sf::Vector2u(832, 373), level, 16, 16, 1))
    {
        std::cerr << "Failed to load tilemap!" << std::endl;
        return -1;
    }

    std::vector<EnemyBullet> enmbull;

     std::vector<std::unique_ptr<BaseEnemy>> enemies;
    enemies.push_back(std::make_unique<ShooterEnemy>(texture, 200.f, 100.f));
    enemies.push_back(std::make_unique<ShooterEnemy>(texture, 600.f, 100.f));
    enemies.push_back(std::make_unique<ExploderEnemy>(texture, 600.f, 300.f));
    enemies.push_back(std::make_unique<ExploderEnemy>(texture, 700.f, 100.f));


    while (window.isOpen()) {
        while (const std::optional<sf::Event> event = window.pollEvent()) {
            if (event->is<sf::Event::Closed>())
                window.close();
        }




        float dt = clock.restart().asSeconds();

        handleShooting(bullets, texture, player.getPosition(), window);
        updateBullets(bullets, dt, window);
        
       // handleKeyChestInteraction(keys, chest,player.getPosition());

        for (auto &enemy : enemies)
            enemy->update(dt, player.getPosition(), enmbull);

         for (auto &eBullet : enmbull)
            eBullet.update(dt);

        player.update(dt);
        crosshair.update(window);
        window.clear();
       window.clear();
  window.setView(player.getView());
for (const auto& bullet : bullets)
    bullet.draw(window);
     for (auto &eBullet : enmbull)
            window.draw(eBullet.shape);
       for (auto &enemy : enemies)
            enemy->draw(window);       
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
