#include <SFML/Graphics.hpp>
#include<bits/stdc++.h>
#include "mapgen.h"
#include "player.h"

int main()
{
    std::cerr << "Terminal working" << std::endl;
    sf::RenderWindow window(sf::VideoMode({500, 500}), "TileMap works!");

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
        {50, 51, 50, 51, 52, 50, 54, 55, 50, 51},
    };

    //Player
    sf::Texture texture;
    if (!texture.loadFromFile("colored-transparent.png"))
        return -1;

    Player player(texture, 3, 24);

    sf::Clock clock;

    //Render Tilemap
    TileMap map;
    if(!map.load("colored-transparent.png", sf::Vector2u(832, 373), level, 16, 16, 1))
    {
        std::cerr << "Failed to load tilemap!" << std::endl;
        return -1;
    }

    while (window.isOpen())
    {
        while (const std::optional event = window.pollEvent())
        {
            if (event->is<sf::Event::Closed>())
                window.close();
        }

        //Player
        float dt = clock.restart().asSeconds();
        player.update(dt);
        
        window.clear();
        window.draw(map);
        player.draw(window);
        window.display();
    }
}