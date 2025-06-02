#include "enemy.hpp"
//: sprite(tex, sf::IntRect({col * 17, row * 17}, {16, 16})) 
ShooterEnemy::ShooterEnemy(sf::Texture& tex, float x, float y)
 : sprite(tex, sf::IntRect({(19 - 1) * 17, (9 - 1) * 17}, {16, 16})){
    
    sprite.setScale({2.f, 2.f});
    sprite.setPosition({x, y});
}

void ShooterEnemy::update(float dt, const sf::Vector2f& playerPos, std::vector<EnemyBullet>& bullets) {
    sf::Vector2f toPlayer = playerPos - sprite.getPosition();
    if (std::sqrt(toPlayer.x * toPlayer.x + toPlayer.y * toPlayer.y) < SHOOT_RANGE) {
        if (shootClock.getElapsedTime().asSeconds() > 0.5f) {
            bullets.emplace_back(sprite.getPosition(), toPlayer);
            shootClock.restart();
        }
    }
}

void ShooterEnemy::draw(sf::RenderWindow& window) {
    window.draw(sprite);
}

ExploderEnemy::ExploderEnemy(sf::Texture& tex, float x, float y) 
:sprite(tex, sf::IntRect({(19 - 1) * 17, (8 - 1) * 17}, {16, 16})){
    
    sprite.setScale({2.f, 2.f});
    sprite.setPosition({x, y});
}

void ExploderEnemy::update(float dt, const sf::Vector2f& playerPos, std::vector<EnemyBullet>&) {
    if (!active) return;

    sf::Vector2f toPlayer = playerPos - sprite.getPosition();
    if (std::sqrt(toPlayer.x * toPlayer.x + toPlayer.y * toPlayer.y) < EXPLODE_RANGE) {
        active = false;
        sprite.setColor(sf::Color::Transparent);
    } else {
        sprite.move(normalize(toPlayer) * ENEMY_SPEED * dt);
    }
}

void ExploderEnemy::draw(sf::RenderWindow& window) {
    if (active)
        window.draw(sprite);
}

TurretEnemy::TurretEnemy(sf::Texture& tex, float x, float y)
: sprite(tex, sf::IntRect({(19 - 1) * 17, (10 - 1) * 17}, {16, 16})) {

    sprite.setScale({2.f, 2.f});
    sprite.setPosition({x, y});
}

void TurretEnemy::update(float dt, const sf::Vector2f&, std::vector<EnemyBullet>& bullets) {
    if (shootClock.getElapsedTime().asSeconds() >= shootInterval) {
        sf::Vector2f center = sprite.getPosition();

        for (int i = 0; i < bulletCount; ++i) {
            float angle = i * (2 * M_PI / bulletCount);
            sf::Vector2f dir(std::cos(angle), std::sin(angle));
            bullets.emplace_back(center, dir);
        }

        shootClock.restart();
    }
}

void TurretEnemy::draw(sf::RenderWindow& window) {
    window.draw(sprite);
}
