#pragma once

#include <SFML/Graphics.hpp>
#include <vector>
#include <cmath>

// === Constants ===
constexpr float PLAYER_SPEED = 70.0f;  // Speed of the player
constexpr float ENEMY_SPEED = 100.0f;  // Speed of enemies
constexpr float SHOOT_RANGE = 100.0f;  // Max shooting distance for enemies
constexpr float Ss_speed = 200.f;      // Possibly projectile or special enemy speed
constexpr float EXPLODE_RANGE = 25.0f; // Distance at which ExploderEnemy explodes
constexpr float BULLET_SPEED = 150.0f; // Speed of enemy bullets
constexpr float CHASE_RANGE = 250.0f;  // ExploderEnemy starts chasing player within this range

// === Helper function to normalize vectors ===
inline sf::Vector2f normalize(sf::Vector2f v)
{
    float len = std::sqrt(v.x * v.x + v.y * v.y);
    return (len != 0.f) ? v / len : sf::Vector2f(0.f, 0.f); // Avoid divide by zero
}

class EnemyBullet
{
public:
    sf::CircleShape shape;
    sf::Vector2f velocity;
    bool isAlive = true; // Flag to determine if bullet is still active

    EnemyBullet(sf::Vector2f pos, sf::Vector2f dir); // Constructor

    void update(float dt, const std::vector<std::vector<int>> &collisionMap); // Move and collision check

    sf::Vector2f getPosition() const; // Return current bullet position
};

class BaseEnemy
{
public:
    virtual void update(float dt, const sf::Vector2f &playerPos, std::vector<EnemyBullet> &bullets) = 0; // Update enemy logic
    virtual void draw(sf::RenderWindow &window) = 0;
    virtual bool isAlive() const = 0;            // Check if enemy is still alive
    virtual void takeDamage(int amount) = 0;     // Apply damage to enemy
    virtual sf::FloatRect getBounds() const = 0; // Get enemy bounding box for collision

    virtual ~BaseEnemy() = default; // Virtual destructor
};

// === Enemy that shoots toward the player if in range ===
class ShooterEnemy : public BaseEnemy
{
    bool recentlyDamaged = false; // Flag for visual damage feedback
    int health = 3;

public:
    ShooterEnemy(sf::Texture &texture, float x, float y); // Constructor

    void update(float dt, const sf::Vector2f &playerPos, std::vector<EnemyBullet> &bullets) override;
    void draw(sf::RenderWindow &window) override;
    bool isAlive() const override;
    void takeDamage(int amount) override;
    sf::FloatRect getBounds() const override;

private:
    sf::Sprite sprite;
    sf::Clock shootClock;  // Timer for shooting
    sf::Clock damageClock; // Timer for damage color effect
};

class ExploderEnemy : public BaseEnemy
{
    bool active = true;           // Is this enemy active
    bool chasing = false;         // Is it currently chasing the player
    bool recentlyDamaged = false; // Flash red when hit
    int health = 3;

public:
    ExploderEnemy(sf::Texture &texture, float x, float y); // Constructor

    void update(float dt, const sf::Vector2f &playerPos, std::vector<EnemyBullet> &bullets) override;
    void update(float dt, const sf::Vector2f &playerPos, std::vector<EnemyBullet> &bullets,
                const std::vector<std::vector<int>> &collisionMap); // Overload with collision
    void draw(sf::RenderWindow &window) override;
    bool isAlive() const override;
    void takeDamage(int amount) override;
    sf::FloatRect getBounds() const override;

private:
    sf::Sprite sprite;
    sf::Vector2f spawnPos; // Where the enemy was spawned (for range limit)
    sf::Clock shootClock;  // Used for animation timing
    sf::Clock damageClock; // Color effect timer
};

class TurretEnemy : public BaseEnemy
{
    const float shootInterval = 2.0f; // Time between shooting cycles
    const int bulletCount = 12;       // Number of bullets in radial pattern
    bool recentlyDamaged = false;     // Visual damage feedback
    int health = 3;

public:
    TurretEnemy(sf::Texture &texture, float x, float y); // Constructor

    void update(float dt, const sf::Vector2f &playerPos, std::vector<EnemyBullet> &bullets) override;
    void draw(sf::RenderWindow &window) override;
    bool isAlive() const override;
    void takeDamage(int amount) override;
    sf::FloatRect getBounds() const override;

private:
    sf::Sprite sprite;     // Visual
    sf::Clock shootClock;  // Controls when to shoot
    sf::Clock damageClock; // Red flash effect after hit
};