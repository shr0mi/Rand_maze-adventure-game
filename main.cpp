#include "enemy.hpp"
#include "player.h"
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <iostream>
#include <memory>
#include <vector>

int main()
{
    sf::RenderWindow window(sf::VideoMode({800, 600}), "Game: Player and Enemies");
    window.setFramerateLimit(60);

    sf::Texture texture;
    if (!texture.loadFromFile("colored-transparent.png"))
    {
        std::cerr << "Failed to load texture.\n";
        return -1;
    }

    // --- Create Player ---
    Player player(texture, 5, 25); // pick sprite row, col for player

    // --- Crosshair ---
    Crosshair crosshair(texture, 5, 26);

    // --- Enemies ---
    std::vector<std::unique_ptr<BaseEnemy>> enemies;
    enemies.push_back(std::make_unique<ShooterEnemy>(texture, 200.f, 100.f));
    enemies.push_back(std::make_unique<ShooterEnemy>(texture, 600.f, 100.f));
    enemies.push_back(std::make_unique<ExploderEnemy>(texture, 600.f, 300.f));
    enemies.push_back(std::make_unique<ExploderEnemy>(texture, 700.f, 100.f));
    enemies.push_back(std::make_unique<TurretEnemy>(texture, 400.f, 300.f));


    // --- Keys & Chest ---
    std::vector<Key> keys = {
        Key(texture, {150.f, 400.f}, 2, 5),
        Key(texture, {300.f, 500.f}, 2, 5),
        Key(texture, {600.f, 350.f}, 2, 5)};
    Chest chest(texture, {700.f, 500.f});

    // --- Bullets ---
    // std::vector<Bullet> bullets;
    std::vector<Bullet> playerBullets;
    std::vector<EnemyBullet> enemyBullets;

    sf::Clock deltaClock;

    while (window.isOpen())
    {
        float dt = deltaClock.restart().asSeconds();

        while (const std::optional<sf::Event> event = window.pollEvent())
        {
            if (event->is<sf::Event::Closed>())
                window.close();
        }

        // --- Update ---
        player.update(dt);
        crosshair.update(window);

        for (auto &enemy : enemies)
            enemy->update(dt, player.getPosition(), enemyBullets);

        // MISSING:
        for (auto &eBullet : enemyBullets)
            eBullet.update(dt);

        handleShooting(playerBullets, texture, player.getPosition(), window);
        updateBullets(playerBullets, dt, window);

        handleKeyChestInteraction(keys, chest, player.getPosition());

        // --- Draw ---
        window.clear(sf::Color::Black);
        player.draw(window);
        crosshair.draw(window);

        for (auto &key : keys)
            key.draw(window);

        chest.draw(window);

        for (auto &enemy : enemies)
            enemy->draw(window);

        // for (auto &bullet : bullets)
        //     bullet.draw(window);
        for (auto &bullet : playerBullets)
            bullet.draw(window);

        for (auto &eBullet : enemyBullets)
            window.draw(eBullet.shape); // enemy bullets are just CircleShape

        window.display();
    }

    return 0;
}
