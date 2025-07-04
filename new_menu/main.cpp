#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <bits/stdc++.h>
#include "menu.hpp"
#include "options.hpp"
#include "credits.hpp"
#include "view_manager.hpp"
#include "audio.hpp"

#include "mapgen.h"
#include "player.h"
#include "enemy.hpp"
#include "bsp_algorithm.h"
#include "timer.h"
#include "pause.hpp"

std::vector<std::shared_ptr<BaseEnemy>> enemies;
sf::Texture texture;
std::vector<Key> keys;
Chest chest;
Boss boss;
sf::Vector2f pos;

AudioManager audioManager;

std::vector<std::vector<int>> generateCollisionMap(std::vector<std::vector<int>> &level)
{
    enemies.clear();
    keys.clear();
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
                enemies.push_back(std::make_shared<ShooterEnemy>(texture, j * 16.f, i * 16.f));
            }
            else if (level[i][j] == -2)
            {
                enemies.push_back(std::make_shared<TurretEnemy>(texture, j * 16.f, i * 16.f));
            }
            else if (level[i][j] == -3)
            {
                enemies.push_back(std::make_shared<ExploderEnemy>(texture, j * 16.f, i * 16.f));
            }
            else if (level[i][j] == -4)
            {
                pos.x = j * 16.f;
                pos.y = i * 16.f;
                cout << pos.x << " " << pos.y << endl;
            }
            else if (level[i][j] == -5)
            {
                Key k({j * 16.f, i * 16.f});
                keys.push_back(k);
            }
            else if (level[i][j] == -6)
            {
                chest.setpos({j * 16.f, i * 16.f});
                boss.setpos({j * 16.f, i * 16.f});
            }
        }
    }

    return collisionMap;
}

void runGame(sf::RenderWindow &window)
{
    GameTimer gameTimer;
    PauseMenu pauseMenu;
    pauseMenu.syncWithAudio(audioManager);

    sf::Clock clock;
    bool isPaused = false;

    BSP_algorithm bsp;
    std::vector<std::vector<int>> level = bsp.generate_bsp_map(150, 150);
    std::vector<std::vector<int>> collisionMap = generateCollisionMap(level);

    if (!texture.loadFromFile("colored-transparent.png"))
        return;

    Player player(texture, 3, 24, collisionMap);
    player.setPos(pos);
    Crosshair crosshair(texture, 14, 21);
    std::vector<Bullet> playerBullets;
    std::vector<EnemyBullet> enemyBullets;

    TileMap map;
    if (!map.load("colored-transparent.png", sf::Vector2u(832, 373), level, 16, 16, 1))
    {
        std::cerr << "Failed to load tilemap!" << std::endl;
        return;
    }

    while (window.isOpen())
    {
        while (std::optional event = window.pollEvent())
        {
            if (event->is<sf::Event::Closed>())
                window.close();

            // Press Esc to Pause
            if (event->is<sf::Event::KeyPressed>())
            {
                if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Escape))
                {
                    isPaused = !isPaused;
                }
            }

            if (isPaused)
            {
                pauseMenu.handleEvent(*event, window, audioManager);

                if (event->is<sf::Event::MouseButtonPressed>())
                {
                    sf::Vector2f mousePos = window.mapPixelToCoords(sf::Mouse::getPosition(window));
                    if (pauseMenu.isResumeClicked(mousePos))
                        isPaused = false;
                    else if (pauseMenu.isMenuClicked(mousePos))
                        // window.close();
                        return; // Back to main menu
                }
            }
        }

        // Player
        float dt = clock.restart().asSeconds();

        // Pause
        gameTimer.update(dt, isPaused);

        if (!isPaused)
        {
            player.cheatlook(window, dt);

            for (auto &enemy : enemies)
            {
                // Dynamic cast to access ExploderEnemy update method with collision
                if (auto *exploder = dynamic_cast<ExploderEnemy *>(enemy.get()))
                    exploder->update(dt, player.getPosition(), enemyBullets, collisionMap);
                else
                    enemy->update(dt, player.getPosition(), enemyBullets);
            }

            // MISSING:
            for (auto it = enemyBullets.begin(); it != enemyBullets.end();)
            {
                it->update(dt, collisionMap);
                if (!it->isAlive)
                    it = enemyBullets.erase(it); // remove bullet
                else
                    ++it;
            }

            // Enemy Damage
            for (auto it = playerBullets.begin(); it != playerBullets.end();)
            {
                bool hit = false;
                for (auto &enemy : enemies)
                {
                    if (enemy->isAlive() && it->getBounds().findIntersection(enemy->getBounds()).has_value())
                    {
                        enemy->takeDamage(1); // Adjust damage if needed
                        hit = true;
                        break;
                    }
                }

                if (hit)
                    it = playerBullets.erase(it); // remove bullet
                else
                    ++it;
            }

            enemies.erase(
                std::remove_if(enemies.begin(), enemies.end(),
                               [](const std::shared_ptr<BaseEnemy> &e)
                               {
                                   return !e->isAlive();
                               }),
                enemies.end());

            handleShooting(playerBullets, texture, player.getPosition(), window);
            updateBullets(playerBullets, dt, window);

            player.update(dt, enemyBullets, window, enemies, pos, boss.getBounds(), chest.isOpened());
            boss.update(dt, window, playerBullets, enemyBullets);
            handleKeyChestInteraction(keys, chest, player.getPosition());
            crosshair.update(window);
        }

        window.clear();
        window.setView(player.getCurrentView());
        window.draw(map);
        player.draw(window);
        crosshair.draw(window);
        gameTimer.draw(window); // Draw Timer

        for (auto &bullet : playerBullets)
            bullet.draw(window);

        for (auto &enemy : enemies)
            enemy->draw(window);

        for (auto &eBullet : enemyBullets)
            window.draw(eBullet.shape); // enemy bullets are just CircleShape

        for (auto &ky : keys)
            ky.draw(window);

        chest.draw(window);
        boss.draw(window);

        // Draw Pause Menu
        if (isPaused)
        {
            window.setView(window.getDefaultView());
            pauseMenu.draw(window, audioManager);
        }

        window.display();
    }
}

int main()
{
    sf::RenderWindow window(sf::VideoMode({960, 540}), "Main Menu", sf::Style::None);
    window.setFramerateLimit(60);

    ViewManager viewManager;
    MenuScreen menu;
    OptionsScreen options(audioManager);
    CreditsScreen credits;

    Scene scene = Scene::Menu;

    while (window.isOpen())
    {
        while (std::optional<sf::Event> event = window.pollEvent())
        {
            if (event->is<sf::Event::Closed>())
                window.close();

            if (scene == Scene::Options)
                options.handleEvent(*event, window, audioManager);

            if (event->is<sf::Event::MouseButtonPressed>())
            {
                sf::Vector2f pos = window.mapPixelToCoords(sf::Mouse::getPosition(window));

                if (scene == Scene::Menu)
                {
                    if (menu.playClick(pos))
                    {
                        // Switch to game
                        window.create(sf::VideoMode({1000, 1000}), "Main Game", sf::Style::None);
                        window.setView(sf::View(sf::FloatRect({0, 0}, {1000, 1000}))); // Fix view
                        runGame(window);                                               // run game loop
                        // After return
                        scene = Scene::Menu;
                        window.create(sf::VideoMode({960, 540}), "Main Menu", sf::Style::None);
                    }
                    else if (menu.optionClick(pos))
                    {
                        scene = Scene::Options;
                        viewManager.setView(Scene::Options);
                        options.syncWithAudio(audioManager);
                    }
                    else if (menu.exitClick(pos))
                    {
                        window.close();
                    }
                }
                else if (scene == Scene::Options)
                {
                    options.syncWithAudio(audioManager);

                    if (options.backClicked(pos))
                    {
                        scene = Scene::Menu;
                        viewManager.setView(Scene::Menu);
                    }
                    else if (options.creditsClicked(pos))
                    {
                        scene = Scene::Credits;
                        viewManager.setView(Scene::Credits);
                    }
                }
                else if (scene == Scene::Credits && credits.backClicked(pos))
                {
                    scene = Scene::Options;
                    viewManager.setView(Scene::Options);
                }
            }
        }

        window.setView(viewManager.getView());
        window.clear();

        if (scene == Scene::Menu)
            menu.draw(window);
        else if (scene == Scene::Options)
            options.draw(window);
        else if (scene == Scene::Credits)
            credits.draw(window);

        window.display();
    }

    return 0;
}
