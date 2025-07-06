#include "enemy.hpp"
#include <SFML/Graphics/RectangleShape.hpp>
#include <sstream> // for std::stringstream
#include "audio.hpp"

// Constructor: Initializes bullet shape, color, position, and velocity
EnemyBullet::EnemyBullet(sf::Vector2f pos, sf::Vector2f dir)
{
    shape.setRadius(2);
    shape.setFillColor(sf::Color::Red);
    shape.setPosition(pos);
    velocity = normalize(dir) * BULLET_SPEED;
}

// Update: Moves the bullet and checks collision with map
void EnemyBullet::update(float dt, const std::vector<std::vector<int>> &collisionMap)
{
    shape.move(velocity * dt); // Move bullet

    sf::Vector2f pos = shape.getPosition();
    int col = static_cast<int>(pos.x) / 16;
    int row = static_cast<int>(pos.y) / 16;

    // Check if bullet is outside bounds or hits a wall
    if (row >= 0 && row < static_cast<int>(collisionMap.size()) &&
        col >= 0 && col < static_cast<int>(collisionMap[0].size()))
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

// Return current position of bullet
sf::Vector2f EnemyBullet::getPosition() const
{
    return shape.getPosition();
}

// Constructor: Set sprite and position
ShooterEnemy::ShooterEnemy(sf::Texture &tex, float x, float y)
    : sprite(tex, sf::IntRect({(19 - 1) * 17, (9 - 1) * 17}, {16, 16}))
{
    sprite.setScale({1.f, 1.f});
    sprite.setPosition({x, y});

    // Health bar background
    healthBarBack.setSize({16.f, 3.f});
    healthBarBack.setFillColor(sf::Color(100, 100, 100));
    healthBarBack.setPosition({x, y - 5});

    // Health bar front (dynamic)
    healthBarFront.setSize({16.f, 3.f});
    healthBarFront.setFillColor(sf::Color::Green);
    healthBarFront.setPosition({x, y - 5});
}

// Update: Shoots if player is in range, manages damage flash
void ShooterEnemy::update(float dt, const sf::Vector2f &playerPos, std::vector<EnemyBullet> &bullets)
{
    sf::Vector2f spritemiddle = sprite.getPosition() + sf::Vector2f(8.f, 8.f);
    sf::Vector2f toPlayer = playerPos - spritemiddle;

    // Shoot if player is within range and cooldown passed
    if (std::sqrt(toPlayer.x * toPlayer.x + toPlayer.y * toPlayer.y) < SHOOT_RANGE)
    {
        if (shootClock.getElapsedTime().asSeconds() > 0.5f)
        {
            bullets.emplace_back(spritemiddle, toPlayer);
            shootClock.restart();
        }
    }

    // Reset color after hit flash
    if (recentlyDamaged && damageClock.getElapsedTime().asSeconds() > 0.2f)
    {
        sprite.setColor(sf::Color::White);
        recentlyDamaged = false;
    }

    sf::Vector2f pos = sprite.getPosition();
    healthBarBack.setPosition({pos.x, pos.y - 5});
    healthBarFront.setPosition({pos.x, pos.y - 5});
    healthBarFront.setSize({16.f * static_cast<float>(health) / 3.f, 3.f}); // assumes max health is 3
}

// Draw enemy
void ShooterEnemy::draw(sf::RenderWindow &window)
{
    window.draw(sprite);
    if (health < 3 && health > 0) // Show only when damaged
    {
        window.draw(healthBarBack);
        window.draw(healthBarFront);
    }
}

// Check if enemy is alive
bool ShooterEnemy::isAlive() const
{
    return health > 0;
}

// Take damage and flash red
void ShooterEnemy::takeDamage(int amount)
{
    health -= amount;
    sprite.setColor(sf::Color::Red);
    damageClock.restart();
    recentlyDamaged = true;
}

// Get bounding box for collision
sf::FloatRect ShooterEnemy::getBounds() const
{
    return sprite.getGlobalBounds();
}

// Constructor: Setup sprite and store spawn position
ExploderEnemy::ExploderEnemy(sf::Texture &tex, float x, float y)
    : sprite(tex, sf::IntRect({(19 - 1) * 17, (8 - 1) * 17}, {16, 16})),
      spawnPos(x, y)
{
    sprite.setScale({1.f, 1.f});
    sprite.setPosition({x, y});

    // Health bar background
    healthBarBack.setSize({16.f, 3.f});
    healthBarBack.setFillColor(sf::Color(100, 100, 100));
    healthBarBack.setPosition({x, y - 5});

    // Health bar front (dynamic)
    healthBarFront.setSize({16.f, 3.f});
    healthBarFront.setFillColor(sf::Color::Green);
    healthBarFront.setPosition({x, y - 5});
}

// Update with collision map: Moves toward player, explodes if close, animates, handles collisions
void ExploderEnemy::update(float dt, const sf::Vector2f &playerPos, std::vector<EnemyBullet> &, const std::vector<std::vector<int>> &collisionMap)
{
    if (!active)
        return;

    sf::Vector2f currentPos = sprite.getPosition();
    sf::Vector2f toPlayer = playerPos - currentPos;
    sf::Vector2f toPlayerFromSpawn = playerPos - spawnPos;

    float distToSpawn = std::sqrt(toPlayerFromSpawn.x * toPlayerFromSpawn.x + toPlayerFromSpawn.y * toPlayerFromSpawn.y);
    float distToPlayer = std::sqrt(toPlayer.x * toPlayer.x + toPlayer.y * toPlayer.y);

    // Explode if player is close
    if (distToPlayer < EXPLODE_RANGE)
    {
        sfx.playSound("explode");
        active = false;
        sprite.setColor(sf::Color::Transparent);
        return;
    }

    if (distToSpawn < CHASE_RANGE)
    {
        chasing = true;

        // Animate explosion frames
        int baseCol = 19;
        int frameCount = 6;
        float frameTime = 0.1f;
        int frame = static_cast<int>(shootClock.getElapsedTime().asSeconds() / frameTime) % frameCount;
        int currentCol = baseCol + frame;
        sprite.setTextureRect(sf::IntRect({(currentCol - 1) * 17, (8 - 1) * 17}, {16, 16}));

        // Move toward player and avoid walls
        sf::Vector2f dir = normalize(toPlayer);
        sf::Vector2f nextPos = currentPos + dir * ENEMY_SPEED * dt;
        sf::Vector2f checkPos = nextPos + sf::Vector2f(8.f, 8.f);

        int col = static_cast<int>(checkPos.x) / 16;
        int row = static_cast<int>(checkPos.y) / 16;

        if (row >= 0 && row < (int)collisionMap.size() &&
            col >= 0 && col < (int)collisionMap[0].size() &&
            collisionMap[row][col] == 0)
        {
            sprite.setPosition(nextPos);
        }
    }
    else
    {
        chasing = false;
        sprite.setTextureRect(sf::IntRect({(19 - 1) * 17, (8 - 1) * 17}, {16, 16}));
    }

    // Reset color after being hit
    if (recentlyDamaged && damageClock.getElapsedTime().asSeconds() > 0.2f)
    {
        sprite.setColor(sf::Color::White);
        recentlyDamaged = false;
    }

    sf::Vector2f pos = sprite.getPosition();
    healthBarBack.setPosition({pos.x, pos.y - 5});
    healthBarFront.setPosition({pos.x, pos.y - 5});
    healthBarFront.setSize({16.f * static_cast<float>(health) / 3.f, 3.f}); // assumes max health is 3
}

// Draw exploder only if active
void ExploderEnemy::draw(sf::RenderWindow &window)
{
    if (active)
        window.draw(sprite);
    if (health < 3 && health > 0) // Show only when damaged
    {
        window.draw(healthBarBack);
        window.draw(healthBarFront);
    }
}

// Update without collision map: Default to empty map
void ExploderEnemy::update(float dt, const sf::Vector2f &playerPos, std::vector<EnemyBullet> &bullets)
{
    static std::vector<std::vector<int>> emptyMap;
    update(dt, playerPos, bullets, emptyMap);
}

// Check if enemy is alive and active
bool ExploderEnemy::isAlive() const
{
    return active && health > 0;
}

// Take damage and handle death
void ExploderEnemy::takeDamage(int amount)
{
    health -= amount;
    sprite.setColor(sf::Color::Red);
    damageClock.restart();
    recentlyDamaged = true;

    if (health <= 0)
    {
        active = false;
        sprite.setColor(sf::Color::Transparent); // Hide on death
    }
}

// Get bounds for collision
sf::FloatRect ExploderEnemy::getBounds() const
{
    return sprite.getGlobalBounds();
}

// Constructor: Initialize turret position and sprite
TurretEnemy::TurretEnemy(sf::Texture &tex, float x, float y)
    : sprite(tex, sf::IntRect({(19 - 1) * 17, (10 - 1) * 17}, {16, 16}))
{
    sprite.setScale({1.f, 1.f});
    sprite.setPosition({x, y});

    // Health bar background
    healthBarBack.setSize({16.f, 3.f});
    healthBarBack.setFillColor(sf::Color(100, 100, 100));
    healthBarBack.setPosition({x, y - 5});

    // Health bar front (dynamic)
    healthBarFront.setSize({16.f, 3.f});
    healthBarFront.setFillColor(sf::Color::Green);
    healthBarFront.setPosition({x, y - 5});
}

// Update: Shoots bullets in circular pattern every few seconds
void TurretEnemy::update(float dt, const sf::Vector2f &, std::vector<EnemyBullet> &bullets)
{
    if (shootClock.getElapsedTime().asSeconds() >= shootInterval)
    {
        sf::Vector2f center = sprite.getPosition() + sf::Vector2f(8.f, 8.f);

        // Shoot in all directions
        for (int i = 0; i < bulletCount; ++i)
        {
            float angle = i * (2 * 3.14159265f / bulletCount);
            sf::Vector2f dir(std::cos(angle), std::sin(angle));
            bullets.emplace_back(center, dir);
        }

        shootClock.restart();
    }

    // Reset red flash after damage
    if (recentlyDamaged && damageClock.getElapsedTime().asSeconds() > 0.2f)
    {
        sprite.setColor(sf::Color::White);
        recentlyDamaged = false;
    }

    sf::Vector2f pos = sprite.getPosition();
    healthBarBack.setPosition({pos.x, pos.y - 5});
    healthBarFront.setPosition({pos.x, pos.y - 5});
    healthBarFront.setSize({16.f * static_cast<float>(health) / 3.f, 3.f}); // assumes max health is 3
}

// Draw turret
void TurretEnemy::draw(sf::RenderWindow &window)
{
    window.draw(sprite);
    if (health < 3 && health > 0) // Show only when damaged
    {
        window.draw(healthBarBack);
        window.draw(healthBarFront);
    }
}

// Check if turret is still alive
bool TurretEnemy::isAlive() const
{
    return health > 0;
}

// Take damage and flash red
void TurretEnemy::takeDamage(int amount)
{
    health -= amount;
    sprite.setColor(sf::Color::Red);
    damageClock.restart();
    recentlyDamaged = true;
}

// Get bounding box for collision
sf::FloatRect TurretEnemy::getBounds() const
{
    return sprite.getGlobalBounds();
}
