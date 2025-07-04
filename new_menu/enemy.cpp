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
    // if (recentlyDamaged && damageClock.getElapsedTime().asSeconds() > 0.2f) {
    // sprite.setColor(sf::Color::White); // <-- Changed color reset
    // recentlyDamaged = false;
    // Changed: revert color to white after 0.2s if damaged
if (recentlyDamaged && damageClock.getElapsedTime().asSeconds() > 0.2f) {
    sprite.setColor(sf::Color::White); // <-- Changed color reset
    recentlyDamaged = false;
}



}

void ShooterEnemy::draw(sf::RenderWindow &window)
{
    window.draw(sprite);
}
bool ShooterEnemy::isAlive() const {
    return health > 0;
}

void ShooterEnemy::takeDamage(int amount) {
    health -= amount;
    sprite.setColor(sf::Color::Red); // <-- Changed color on hit
    damageClock.restart();           // <-- Changed for color timing
    recentlyDamaged = true;          // <-- Changed for color tracking
}

sf::FloatRect ShooterEnemy::getBounds() const {
    return sprite.getGlobalBounds();
}


ExploderEnemy::ExploderEnemy(sf::Texture &tex, float x, float y)
    : sprite(tex, sf::IntRect({(19 - 1) * 17, (8 - 1) * 17}, {16, 16})),
      spawnPos(x, y)
{
    sprite.setScale({1.f, 1.f});
    sprite.setPosition({x, y});
}



void ExploderEnemy::update(float dt, const sf::Vector2f &playerPos, std::vector<EnemyBullet> &, const std::vector<std::vector<int>>& collisionMap)
{
    if (!active) return;

    sf::Vector2f currentPos = sprite.getPosition();
    sf::Vector2f toPlayer = playerPos - currentPos;
    sf::Vector2f toPlayerFromSpawn = playerPos - spawnPos;

    float distToSpawn = std::sqrt(toPlayerFromSpawn.x * toPlayerFromSpawn.x + toPlayerFromSpawn.y * toPlayerFromSpawn.y);
    float distToPlayer = std::sqrt(toPlayer.x * toPlayer.x + toPlayer.y * toPlayer.y);

    if (distToPlayer < EXPLODE_RANGE)
    {
        active = false;
        sprite.setColor(sf::Color::Transparent);
        return;
    }

    if (distToSpawn < CHASE_RANGE)
    {
        chasing = true;

        // Animate frames
        int baseCol = 19;
        int frameCount = 6;
        float frameTime = 0.1f;
        int frame = static_cast<int>(shootClock.getElapsedTime().asSeconds() / frameTime) % frameCount;
        int currentCol = baseCol + frame;
        sprite.setTextureRect(sf::IntRect({(currentCol - 1) * 17, (8 - 1) * 17}, {16, 16}));

        sf::Vector2f dir = normalize(toPlayer);
        sf::Vector2f nextPos = currentPos + dir * ENEMY_SPEED * dt;

        // Check future position center for collision
        sf::Vector2f checkPos = nextPos + sf::Vector2f(8.f, 8.f); // sprite center
        int col = static_cast<int>(checkPos.x) / 16;
        int row = static_cast<int>(checkPos.y) / 16;

        if (row >= 0 && row < (int)collisionMap.size() &&
            col >= 0 && col < (int)collisionMap[0].size() &&
            collisionMap[row][col] == 0)
        {
            sprite.setPosition(nextPos); // move only if not colliding
        }
        // else: blocked, stay in place
    }
    else
    {
        chasing = false;
        sprite.setTextureRect(sf::IntRect({(19 - 1) * 17, (8 - 1) * 17}, {16, 16}));
    }
    // === Revert color after damage ===
    if (recentlyDamaged && damageClock.getElapsedTime().asSeconds() > 0.2f) {
        sprite.setColor(sf::Color::White); // <-- Changed color reset
        recentlyDamaged = false;
    }
}

void ExploderEnemy::draw(sf::RenderWindow &window)
{
    if (active)
        window.draw(sprite);
}
void ExploderEnemy::update(float dt, const sf::Vector2f &playerPos, std::vector<EnemyBullet> &bullets)
{
    // Dummy empty collision map — used only to satisfy the abstract function
    static std::vector<std::vector<int>> emptyMap;
    update(dt, playerPos, bullets, emptyMap);
}
bool ExploderEnemy::isAlive() const {
    return active && health > 0;
}

void ExploderEnemy::takeDamage(int amount) {
    health -= amount;
     sprite.setColor(sf::Color::Red); // <-- Changed color on hit
    damageClock.restart();           // <-- Changed for color timing
    recentlyDamaged = true;          // <-- Changed for color tracking

    if (health <= 0) {
        active = false;
        sprite.setColor(sf::Color::Transparent); // hide
    }
}

sf::FloatRect ExploderEnemy::getBounds() const {
    return sprite.getGlobalBounds();
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
    // === Revert color after damage ===
    if (recentlyDamaged && damageClock.getElapsedTime().asSeconds() > 0.2f) {
        sprite.setColor(sf::Color::White); // <-- Changed color reset
        recentlyDamaged = false;
    }
}

void TurretEnemy::draw(sf::RenderWindow &window)
{
    window.draw(sprite);
}
bool TurretEnemy::isAlive() const {
    return health > 0;
}

void TurretEnemy::takeDamage(int amount) {
    health -= amount;
    sprite.setColor(sf::Color::Red); // <-- Changed color on hit
    damageClock.restart();           // <-- Changed for color timing
    recentlyDamaged = true;          // <-- Changed for color tracking
}

sf::FloatRect TurretEnemy::getBounds() const {
    return sprite.getGlobalBounds();
}