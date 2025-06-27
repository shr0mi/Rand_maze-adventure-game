#include <SFML/Graphics.hpp>
#include <bits/stdc++.h>
#include "mapgen.h"
#include "player.h"
#include "enemy.hpp"
#include "bsp_algorithm.h"
#include "pause.hpp" // pause menu
#include "options.hpp"
#include "audio.hpp"

PauseMenu pauseMenu;
AudioManager audioManager;

std::vector<std::unique_ptr<BaseEnemy>> enemies;
sf::Texture texture;

std::vector<std::vector<int>> generateCollisionMap(std::vector<std::vector<int>> &level)
{
    std::vector<std::vector<int>> collisionMap(level.size(), std::vector<int>(level[0].size(), 0));
    for (int i = 0; i < level.size(); i++)
    {
        for (int j = 0; j < level[i].size(); j++)
        {
            if ((50 <= level[i][j] && level[i][j] <= 55) || level[i][j] == 12 || level[i][j] == 638)
            {
                collisionMap[i][j] = 1;
            }
            else
            {
                collisionMap[i][j] = 0;
            }

            if (level[i][j] == -1)
            {
                enemies.push_back(std::make_unique<ShooterEnemy>(texture, j * 16.f, i * 16.f));
            }
            else if (level[i][j] == -2)
            {
                enemies.push_back(std::make_unique<TurretEnemy>(texture, j * 16.f, i * 16.f));
            }
        }
    }

    return collisionMap;
}

int main()
{
    std::cerr << "Terminal working" << std::endl;
    sf::RenderWindow window(sf::VideoMode({1000, 1000}), "TileMap works!");

    if (!texture.loadFromFile("colored-transparent.png"))
        return -1;

    BSP_algorithm bsp;
    std::vector<std::vector<int>> level = bsp.generate_bsp_map(200, 200);
    std::vector<std::vector<int>> collisionMap = generateCollisionMap(level);

    Player player(texture, 3, 24, collisionMap);
    Crosshair crosshair(texture, 14, 21);
    std::vector<Bullet> bullets;

    sf::Clock clock;

    TileMap map;
    if (!map.load("colored-transparent.png", sf::Vector2u(832, 373), level, 16, 16, 1))
    {
        std::cerr << "Failed to load tilemap!" << std::endl;
        return -1;
    }

    std::vector<Bullet> playerBullets;
    std::vector<EnemyBullet> enemyBullets;

    // Pause flag
    bool isPaused = false;

    // Load font for PAUSED message
    sf::Font font;
    if (!font.openFromFile("arial.ttf"))
    {
        std::cerr << "Could not load font!" << std::endl;
    }

    while (window.isOpen())
    {
        while (std::optional event = window.pollEvent())
        {
            if (event->is<sf::Event::Closed>())
            {
                window.close();
            }

            if (event->is<sf::Event::KeyPressed>())
            {
                if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Escape))
                {
                    isPaused = !isPaused;
                }
            }

            // Handle PauseMenu UI
            if (isPaused)
            {
                pauseMenu.handleEvent(*event, window, audioManager);

                // Check resume/menu button clicks
                if (event->is<sf::Event::MouseButtonPressed>())
                {
                    sf::Vector2f mousePos = window.mapPixelToCoords(sf::Mouse::getPosition(window));
                    if (pauseMenu.isResumeClicked(mousePos))
                    {
                        isPaused = false; // Unpause
                    }
                    else if (pauseMenu.isMenuClicked(mousePos))
                    {
                        window.close();
                        std::system("main_menu.exe");
                    }
                }
            }
        }

        float dt = clock.restart().asSeconds();

        if (!isPaused)
        {
            for (auto &enemy : enemies)
                enemy->update(dt, player.getPosition(), enemyBullets);

            for (auto &eBullet : enemyBullets)
                eBullet.update(dt);

            handleShooting(playerBullets, texture, player.getPosition(), window);
            updateBullets(playerBullets, dt, window);
            player.update(dt);
        }

        crosshair.update(window);

        window.clear();
        window.setView(player.getView());
        window.draw(map);
        player.draw(window);
        crosshair.draw(window);

        for (auto &bullet : playerBullets)
            bullet.draw(window);

        for (auto &enemy : enemies)
            enemy->draw(window);

        for (auto &eBullet : enemyBullets)
            window.draw(eBullet.shape);

        if (isPaused)
        {
            window.setView(window.getDefaultView());
            pauseMenu.draw(window);
        }

        window.display();
    }
}
