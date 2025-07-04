#pragma once
#include <SFML/Graphics.hpp>
#include <vector>
#include <cmath>
// Constants for various speeds and ranges
constexpr float PLAYER_SPEED = 100.0f;
constexpr float ENEMY_SPEED = 100.0f;
constexpr float SHOOT_RANGE = 100.0f;
constexpr float Ss_speed = 200.f;
constexpr float EXPLODE_RANGE = 25.0f;
constexpr float BULLET_SPEED = 150.0f;
constexpr float CHASE_RANGE = 250.0f; // Enemy chases player within 200 pixels

// normalize a vector (convert to unit vector)
inline sf::Vector2f normalize(sf::Vector2f v)
{
    float len = std::sqrt(v.x * v.x + v.y * v.y);
    return (len != 0.f) ? v / len : sf::Vector2f(0.f, 0.f);
}

// Enemy Bullet struct
struct EnemyBullet
{
    sf::CircleShape shape;
    sf::Vector2f velocity;
    bool isAlive = true; // Determines if bullet is still active

    // Constructor: initialize bullet with position and direction
    EnemyBullet(sf::Vector2f pos, sf::Vector2f dir)
    {
        shape.setRadius(2);
        shape.setFillColor(sf::Color::Red);
        shape.setPosition(pos);
        velocity = normalize(dir) * BULLET_SPEED;
    }

    // Updates bullet position and checks collision with map
    void update(float dt, const std::vector<std::vector<int>> &collisionMap)
    {
        shape.move(velocity * dt);
        sf::Vector2f pos = shape.getPosition();
        int col = static_cast<int>(pos.x) / 16;
        int row = static_cast<int>(pos.y) / 16;

        // If bullet hits wall (value 1) or out of bounds, it's destroyed
        if (row >= 0 && row < (int)collisionMap.size() &&
            col >= 0 && col < (int)collisionMap[0].size())
        {
            if (collisionMap[row][col] == 1)
            {
                isAlive = false;
            }
        }
        else
        {
            isAlive = false;
        }
    }

    // Returns current bullet position
    sf::Vector2f getPosition() const
    {
        return shape.getPosition();
    }
};

// Class for all enemy types
class BaseEnemy
{
public:
    // Pure virtual methods to be implemented by derived enemy classes
    virtual void update(float dt, const sf::Vector2f &playerPos, std::vector<EnemyBullet> &bullets) = 0;
    virtual void draw(sf::RenderWindow &window) = 0;
    virtual bool isAlive() const = 0;
    virtual void takeDamage(int amount) = 0;
    virtual sf::FloatRect getBounds() const = 0;

    // Virtual destructor for proper cleanup of derived classes
    virtual ~BaseEnemy() = default;
};

// ShooterEnemy class
class ShooterEnemy : public BaseEnemy
{
public:
    ShooterEnemy(sf::Texture &texture, float x, float y);

    void update(float dt, const sf::Vector2f &playerPos, std::vector<EnemyBullet> &bullets) override;
    void draw(sf::RenderWindow &window) override;
    bool isAlive() const override;
    void takeDamage(int amount) override;
    sf::FloatRect getBounds() const override;

private:
    int health = 3;

    sf::Sprite sprite;
    sf::Clock shootClock;         // Timer for shooting intervals
    sf::Clock damageClock;        // Timer for managing recent damage flash
    bool recentlyDamaged = false; // Flag to indicate recent hit
};

// Exploder enemy that chases and explodes near the player
class ExploderEnemy : public BaseEnemy
{
public:
    ExploderEnemy(sf::Texture &texture, float x, float y);

    // Overloaded update methods, one with collision map
    void update(float dt, const sf::Vector2f &playerPos, std::vector<EnemyBullet> &bullets) override;
    void update(float dt, const sf::Vector2f &playerPos, std::vector<EnemyBullet> &bullets, const std::vector<std::vector<int>> &collisionMap);
    void draw(sf::RenderWindow &window) override;
    bool isAlive() const override;
    void takeDamage(int amount) override;
    sf::FloatRect getBounds() const override;

private:
    int health = 3;
    sf::Sprite sprite;
    sf::Vector2f spawnPos;
    bool active = true;           // Is the enemy currently active
    bool chasing = false;         // Is the enemy currently chasing the player
    sf::Clock shootClock;         // Optional shooting or self-destruct timer
    sf::Clock damageClock;        // Timer for handling visual damage effect
    bool recentlyDamaged = false; // Whether the enemy was recently damaged
};

// TurretEnemy class - shoots in a circular pattern every 2 seconds
class TurretEnemy : public BaseEnemy
{
public:
    TurretEnemy(sf::Texture &texture, float x, float y);

    void update(float dt, const sf::Vector2f &playerPos, std::vector<EnemyBullet> &bullets) override;
    void draw(sf::RenderWindow &window) override;
    bool isAlive() const override;
    void takeDamage(int amount) override;
    sf::FloatRect getBounds() const override;

private:
    int health = 3;
    sf::Sprite sprite;
    sf::Clock shootClock;             // Timer to control burst intervals
    const float shootInterval = 2.0f; // Interval between bursts (in seconds)
    const int bulletCount = 12;       // how many bullets per burst
    sf::Clock damageClock;            // Flash timer for when damaged
    bool recentlyDamaged = false;     // Flag for recent damage visual
};