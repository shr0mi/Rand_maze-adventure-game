#include <SFML/Graphics.hpp>
#include<bits/stdc++.h>
#include "mapgen.h"
#include "player.h"
#include "enemy.hpp"
#include "bsp_algorithm.h"

std::vector<std::unique_ptr<BaseEnemy>> enemies;
sf::Texture texture;

std::vector<std::vector<int>> generateCollisionMap(std::vector<std::vector<int>>& level)
{
    std::vector<std::vector<int>> collisionMap(level.size(), std::vector<int>(level[0].size(), 0));
    for (int i = 0; i < level.size(); i++)
    {
        for (int j = 0; j < level[i].size(); j++)
        {
            if((50 <= level[i][j] && level[i][j] <= 55) || level[i][j] == 12 || level[i][j] == 638){
                collisionMap[i][j] = 1;
            }else{
                collisionMap[i][j] = 0;
            }

            if(level[i][j] == -1){
                enemies.push_back(std::make_unique<ExploderEnemy>(texture, j * 16.f, i * 16.f));
            }else if(level[i][j] == -2){
                enemies.push_back(std::make_unique<TurretEnemy>(texture, j * 16.f, i * 16.f));
            }
            //std::cout << collisionMap[i][j] << " ";
        }
        //std::cout << std::endl;
    }

    return collisionMap;
}

int main()
{
    std::cerr << "Terminal working" << std::endl;
    sf::RenderWindow window(sf::VideoMode({1000, 1000}), "TileMap works!");
    window.setMouseCursorVisible(false);
    //Player
    if (!texture.loadFromFile("colored-transparent.png"))
        return -1;
    // Level Vector
    // Trees -> 50, 51, 52, 53, 54, 55
    //Walls -> 19, 20, 21, 68, *69, 70, 117, 118, 119
    //Floor -> 2, 3, 4, 5, 
    
    BSP_algorithm bsp;
    std::vector<std::vector<int>> level = bsp.generate_bsp_map(200, 200);

    // Generate collision map
    std::vector<std::vector<int>> collisionMap = generateCollisionMap(level);


    

    Player player(texture, 3, 24, collisionMap);
    Crosshair crosshair(texture , 14, 21);
    std::vector<Bullet> bullets;

    sf::Clock clock;

    //Render Tilemap
    TileMap map;
    if(!map.load("colored-transparent.png", sf::Vector2u(832, 373), level, 16, 16, 1))
    {
        std::cerr << "Failed to load tilemap!" << std::endl;
        return -1;
    }

    //Bullet handling
    std::vector<Bullet> playerBullets;
    std::vector<EnemyBullet> enemyBullets;

    // --- Enemies ---
    
    //enemies.push_back(std::make_unique<ShooterEnemy>(texture, 200.f, 100.f));
    //enemies.push_back(std::make_unique<ShooterEnemy>(texture, 600.f, 100.f));
    //enemies.push_back(std::make_unique<ExploderEnemy>(texture, 600.f, 300.f));
    //enemies.push_back(std::make_unique<ExploderEnemy>(texture, 700.f, 100.f));
    //enemies.push_back(std::make_unique<TurretEnemy>(texture, 400.f, 300.f));

    while (window.isOpen())
    {
        
        while (std::optional event = window.pollEvent())
        {
            if (event->is<sf::Event::Closed>())
                window.close();
        }

        //Player
        float dt = clock.restart().asSeconds();
        player.cheatlook(window,dt);

        for (auto &enemy : enemies)
            enemy->update(dt, player.getPosition(), enemyBullets);

        // MISSING:
        for (auto it = enemyBullets.begin(); it != enemyBullets.end(); ) {
    it->update(dt, collisionMap);
    if (!it->isAlive)
        it = enemyBullets.erase(it); // remove bullet
    else
        ++it;
}


        handleShooting(playerBullets, texture, player.getPosition(), window);
        updateBullets(playerBullets, dt, window);
        
        player.update(dt,enemyBullets);
        crosshair.update(window);
        window.clear();
        window.setView(player.getCurrentView());


        window.draw(map);
        player.draw(window);
        crosshair.draw(window);

        for (auto &bullet : playerBullets)
            bullet.draw(window);

        for (auto &enemy : enemies)
            enemy->draw(window);

        for (auto &eBullet : enemyBullets)
            window.draw(eBullet.shape);   

        window.display();    

    }
}