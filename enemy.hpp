#pragma once
#include <SFML/Graphics.hpp>
#include <vector>
#include <cmath>

constexpr float PLAYER_SPEED = 100.0f;
constexpr float ENEMY_SPEED = 60.0f;
constexpr float SHOOT_RANGE = 400.0f;
constexpr float EXPLODE_RANGE = 50.0f;
constexpr float BULLET_SPEED = 250.0f;

// Normalize helper
inline sf::Vector2f normalize(sf::Vector2f v) {
    float len = std::sqrt(v.x * v.x + v.y * v.y);
    return (len != 0.f) ? v / len : sf::Vector2f(0.f, 0.f);
}

// Bullet struct
struct EnemyBullet {
    sf::CircleShape shape;
    sf::Vector2f velocity;

    EnemyBullet(sf::Vector2f pos, sf::Vector2f dir) {
        shape.setRadius(5);
        shape.setFillColor(sf::Color::Red);
        shape.setPosition(pos);
        velocity = normalize(dir) * BULLET_SPEED;
    }

    void update(float dt) {
        shape.move(velocity * dt);
    }
};

// Base enemy class (optional if shared behavior increases)
class BaseEnemy {
public:
    virtual void update(float dt, const sf::Vector2f& playerPos, std::vector<EnemyBullet>& bullets) = 0;
    virtual void draw(sf::RenderWindow& window) = 0;
    virtual ~BaseEnemy() = default;
};

// ShooterEnemy class
class ShooterEnemy : public BaseEnemy {
public:
    ShooterEnemy(sf::Texture& texture, float x, float y);

    void update(float dt, const sf::Vector2f& playerPos, std::vector<EnemyBullet>& bullets) override;
    void draw(sf::RenderWindow& window) override;

private:
    sf::Sprite sprite;
    sf::Clock shootClock;
};

// ExploderEnemy class
class ExploderEnemy : public BaseEnemy {
public:
    ExploderEnemy(sf::Texture& texture, float x, float y);

    void update(float dt, const sf::Vector2f& playerPos, std::vector<EnemyBullet>& bullets) override;
    void draw(sf::RenderWindow& window) override;

private:
    sf::Sprite sprite;
    bool active = true;
};
