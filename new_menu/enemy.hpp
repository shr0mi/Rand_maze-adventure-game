#pragma once
#include <SFML/Graphics.hpp>
#include <vector>
#include <cmath>

constexpr float PLAYER_SPEED = 100.0f;
constexpr float ENEMY_SPEED = 100.0f;
constexpr float SHOOT_RANGE = 100.0f;
constexpr float Ss_speed = 200.f;
constexpr float EXPLODE_RANGE = 25.0f;
constexpr float BULLET_SPEED = 150.0f;
constexpr float CHASE_RANGE = 250.0f; // Enemy chases player within 200 pixels

// Normalize helper
inline sf::Vector2f normalize(sf::Vector2f v)
{
    float len = std::sqrt(v.x * v.x + v.y * v.y);
    return (len != 0.f) ? v / len : sf::Vector2f(0.f, 0.f);
}

// Bullet struct
struct EnemyBullet
{
    sf::CircleShape shape;
    sf::Vector2f velocity;
    bool isAlive = true;

    EnemyBullet(sf::Vector2f pos, sf::Vector2f dir)
    {
        shape.setRadius(2);
        shape.setFillColor(sf::Color::Red);
        shape.setPosition(pos);
        velocity = normalize(dir) * BULLET_SPEED;
    }

    /*void update(float dt)
    {
        shape.move(velocity * dt);
    }*/

    void update(float dt, const std::vector<std::vector<int>> &collisionMap)
    {
        shape.move(velocity * dt);
        sf::Vector2f pos = shape.getPosition();
        int col = static_cast<int>(pos.x) / 16;
        int row = static_cast<int>(pos.y) / 16;

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

    sf::Vector2f getPosition() const
    {
        return shape.getPosition();
    }
};

// Base enemy class (optional if shared behavior increases)
// class BaseEnemy
// {
// public:
//     virtual void update(float dt, const sf::Vector2f &playerPos, std::vector<EnemyBullet> &bullets) = 0;
//     virtual void draw(sf::RenderWindow &window) = 0;
//     virtual ~BaseEnemy() = default;
// };

// enemy.hpp
class BaseEnemy
{
public:
    virtual void update(float dt, const sf::Vector2f &playerPos, std::vector<EnemyBullet> &bullets) = 0;
    virtual void draw(sf::RenderWindow &window) = 0;

    // NEW:
    virtual bool isAlive() const = 0;
    virtual void takeDamage(int amount) = 0;
    virtual sf::FloatRect getBounds() const = 0;

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
    int health = 3; // or any value

    sf::Sprite sprite;
    sf::Clock shootClock;
      sf::Clock damageClock;
    bool recentlyDamaged = false;
};

// ExploderEnemy class
// class ExploderEnemy : public BaseEnemy
// {
// public:
//     ExploderEnemy(sf::Texture &texture, float x, float y);

//     void update(float dt, const sf::Vector2f &playerPos, std::vector<EnemyBullet> &bullets) override;
//     void draw(sf::RenderWindow &window) override;

// private:
//     sf::Sprite sprite;
//     bool active = true;
//     sf::Clock shootClock; // ✅ Add this line
// };
// class ExploderEnemy : public BaseEnemy
// {
// public:
//     ExploderEnemy(sf::Texture &texture, float x, float y);
//     void update(float dt, const sf::Vector2f &playerPos, std::vector<EnemyBullet> &bullets) override;
//     void draw(sf::RenderWindow &window) override;

// private:
//     sf::Sprite sprite;
//     bool active = true;
//     bool chasing = false;
//     sf::Clock shootClock;
// };
// class ExploderEnemy : public BaseEnemy
// {
// public:
//     ExploderEnemy(sf::Texture &texture, float x, float y);

//     //void update(float dt, const sf::Vector2f &playerPos, std::vector<EnemyBullet> &bullets) override;
//     void update(float dt, const sf::Vector2f &playerPos, std::vector<EnemyBullet> &bullets, const std::vector<std::vector<int>>& collisionMap);
//     void draw(sf::RenderWindow &window) override;

// private:
//     sf::Sprite sprite;
//     sf::Vector2f spawnPos;
//     bool active = true;
//     bool chasing = false;
//     sf::Clock shootClock;
// };
class ExploderEnemy : public BaseEnemy
{
public:
    ExploderEnemy(sf::Texture &texture, float x, float y);

    void update(float dt, const sf::Vector2f &playerPos, std::vector<EnemyBullet> &bullets) override;
    void update(float dt, const sf::Vector2f &playerPos, std::vector<EnemyBullet> &bullets, const std::vector<std::vector<int>> &collisionMap);
    void draw(sf::RenderWindow &window) override;
    bool isAlive() const override;
void takeDamage(int amount) override;
sf::FloatRect getBounds() const override;


private:
    int health =3;
    sf::Sprite sprite;
    sf::Vector2f spawnPos;
    bool active = true;
    bool chasing = false;
    sf::Clock shootClock;
      sf::Clock damageClock;
    bool recentlyDamaged = false;
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
    sf::Clock shootClock;
    const float shootInterval = 2.0f;
    const int bulletCount = 12; // how many bullets per burst
      sf::Clock damageClock;
    bool recentlyDamaged = false;
};