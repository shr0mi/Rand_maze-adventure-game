#include "enemy.hpp"
//: sprite(tex, sf::IntRect({col * 17, row * 17}, {16, 16}))
ShooterEnemy::ShooterEnemy(sf::Texture &tex, float x, float y)
    : sprite(tex, sf::IntRect({(19 - 1) * 17, (9 - 1) * 17}, {16, 16}))
{

    sprite.setScale({1.f, 1.f});
    sprite.setPosition({x, y});
}

void ShooterEnemy::update(float dt, const sf::Vector2f &playerPos, std::vector<EnemyBullet> &bullets)
{
    sf::Vector2f spritemiddle = sprite.getPosition() + sf::Vector2f(8.f,8.f);
    sf::Vector2f toPlayer = playerPos - spritemiddle;
    
    if (std::sqrt(toPlayer.x * toPlayer.x + toPlayer.y * toPlayer.y) < SHOOT_RANGE)
    {
        if (shootClock.getElapsedTime().asSeconds() > 0.5f)
        {
            bullets.emplace_back(spritemiddle, toPlayer);
            shootClock.restart();
        }
    }
}

void ShooterEnemy::draw(sf::RenderWindow &window)
{
    window.draw(sprite);
}

ExploderEnemy::ExploderEnemy(sf::Texture &tex, float x, float y)
    : sprite(tex, sf::IntRect({(19 - 1) * 17, (8 - 1) * 17}, {16, 16}))
{

    sprite.setScale({1.f, 1.f});
    sprite.setPosition({x, y});
}

// void ExploderEnemy::update(float dt, const sf::Vector2f& playerPos, std::vector<EnemyBullet>&) {
//     if (!active) return;

//     sf::Vector2f toPlayer = playerPos - sprite.getPosition();
//     if (std::sqrt(toPlayer.x * toPlayer.x + toPlayer.y * toPlayer.y) < EXPLODE_RANGE) {
//         active = false;
//         sprite.setColor(sf::Color::Transparent);
//     } else {
//         sprite.move(normalize(toPlayer) * ENEMY_SPEED * dt);
//     }
// }
void ExploderEnemy::update(float dt, const sf::Vector2f &playerPos, std::vector<EnemyBullet> &)
{
    if (!active)
        return;

    // Animate sprite frame (columns 19 to 24)
    int baseCol = 19;
    int frameCount = 6;     // Columns 19, 20, 21, 22, 23, 24
    float frameTime = 0.1f; // Duration of each frame in seconds
    int frame = static_cast<int>(shootClock.getElapsedTime().asSeconds() / frameTime) % frameCount;
    int currentCol = baseCol + frame;
    sprite.setTextureRect(sf::IntRect({(currentCol - 1) * 17, (8 - 1) * 17}, {16, 16}));

    sf::Vector2f toPlayer = playerPos - sprite.getPosition();
    if (std::sqrt(toPlayer.x * toPlayer.x + toPlayer.y * toPlayer.y) < EXPLODE_RANGE)
    {
        active = false;
        sprite.setColor(sf::Color::Transparent);
    }
    else
    {
        sprite.move(normalize(toPlayer) * ENEMY_SPEED * dt);
    }
}

void ExploderEnemy::draw(sf::RenderWindow &window)
{
    if (active)
        window.draw(sprite);
}

TurretEnemy::TurretEnemy(sf::Texture &tex, float x, float y)
    : sprite(tex, sf::IntRect({(19 - 1) * 17, (10 - 1) * 17}, {16, 16}))
{

    sprite.setScale({1.f, 1.f});
    sprite.setPosition({x, y});
}

void TurretEnemy::update(float dt, const sf::Vector2f &, std::vector<EnemyBullet> &bullets)
{
    if (shootClock.getElapsedTime().asSeconds() >= shootInterval)
    {
        sf::Vector2f center = sprite.getPosition()+sf::Vector2f(8.f,8.f);

        for (int i = 0; i < bulletCount; ++i)
        {
            float angle = i * (2 * 3.14159265f / bulletCount);
            sf::Vector2f dir(std::cos(angle), std::sin(angle));
            bullets.emplace_back(center, dir);
        }

        shootClock.restart();
    }
}

void TurretEnemy::draw(sf::RenderWindow &window)
{
    window.draw(sprite);
}