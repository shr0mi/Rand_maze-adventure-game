#include <iostream>
#include "player.h"
#include "enemy.hpp"
#include <cmath>
#include <algorithm>

Player::Player(sf::Texture &tex, int row, int col, std::vector<std::vector<int>> collisionMap, GameInfo &gameInfo)
    : sprite(tex, sf::IntRect({col * 17, row * 17}, {17, 17}))
{
    sprite.setScale({1.f, 1.f});
    gi = &gameInfo;
    gameInfo.collMap = collisionMap;
    gameInfo.pltex = tex;
}

void Player::setPos(sf::Vector2f pos)
{
    sprite.setPosition(pos);
}

void Player::update(float dt, std::vector<EnemyBullet> &enemyBullets, sf::RenderWindow &window, std::vector<std::shared_ptr<BaseEnemy>> enemies, sf::Vector2f pos, sf::FloatRect bossbounds, bool open)
{
    if (debugMode || sf::Keyboard::isKeyPressed(sf::Keyboard::Key::LShift))
        return;

    static bool isRespawning = false;
    static float respawnTimer = 0.f;
    static int countdownValue = 3;
    static sf::IntRect playerNormalRect = sprite.getTextureRect();
    static int flickerFrames = 0;

    // === If respawning, handle countdown ===
    if (isRespawning)
    {
        gi->disableshoot = true;
        respawnTimer -= dt;

        if (respawnTimer <= 0.f)
        {
            countdownValue--;

            if (countdownValue > 0)
            {
                respawnTimer = 1.f;
                int x = 0;
                if (countdownValue == 2)
                    x = 629;
                else if (countdownValue == 1)
                    x = 612;
                int y = 289;

                sprite.setTextureRect(sf::IntRect({x, 289}, {16, 16}));
                sprite.setScale({3.f, 3.f});
            }
            else
            {
                // Respawn done
                gi->disableshoot = false;
                isRespawning = false;
                flickerFrames = 0;
                health = 4;
                sprite.setPosition(pos);
                sprite.setTextureRect(playerNormalRect); // restore normal frame
                sprite.setScale({1.f, 1.f});
                std::cout << "Player respawned\n";
            }
        }

        return; // Skip everything else while respawning
    }

    // === Invincibility timer ===
    if (invincibleTimer > 0.f)
        invincibleTimer -= dt;

    // === Bullet collision ===
    sf::FloatRect playerBounds = sprite.getGlobalBounds();

    if (bossbounds.findIntersection(playerBounds).has_value() && gi->active)
    {
        if (invincibleTimer <= 0.f)
        {
            invincibleTimer = INVINCIBLE_TIME;
            health--;
            std::cout << "Player hit by Boss! Health: " << health << "\n";
            flickerFrames = 200;
        }
    }

    for (size_t i = 0; i < enemyBullets.size(); ++i)
    {
        if (enemyBullets[i].shape.getGlobalBounds().findIntersection(playerBounds).has_value())
        {
            if (invincibleTimer <= 0.f)
            {
                invincibleTimer = INVINCIBLE_TIME;
                health--;
                std::cout << "Player hit! Health: " << health << "\n";

                // Start flicker: 10 frames of invisibility
                flickerFrames = 200;

                enemyBullets.erase(enemyBullets.begin() + i);
                break;
            }
        }
    }

    for (const auto &enemy : enemies)
    {
        // Check if this enemy is an ExploderEnemy
        ExploderEnemy *exploder = dynamic_cast<ExploderEnemy *>(enemy.get());
        if (!exploder)
            continue;

        // Get distance to player
        sf::FloatRect ebounds = exploder->getBounds(); // assumes getPosition() exists

        sf::Vector2f playerPosCenter(
            playerBounds.position.x + playerBounds.size.x / 2.f,
            playerBounds.position.y + playerBounds.size.y / 2.f);

        sf::Vector2f enemyPosCenter(
            ebounds.position.x + ebounds.size.x / 2.f,
            ebounds.position.y + ebounds.size.y / 2.f);

        sf::Vector2f toPlayer = playerPosCenter - enemyPosCenter;
        float distance = std::sqrt(toPlayer.x * toPlayer.x + toPlayer.y * toPlayer.y);

        if (distance < 26.f)
        {
            if (invincibleTimer <= 0.f)
            {
                invincibleTimer = INVINCIBLE_TIME;
                health -= 2;
                std::cout << "Player hit! Health: " << health << "\n";
                flickerFrames = 200;
                break; // Only damage once
            }
        }
    }

    if (flickerFrames > 0)
    {
        flickerFrames--;
        // Make sprite invisible
        sprite.setColor(sf::Color(255, 0, 0, 255)); // Fully transparent
    }
    else
    {
        // Restore full opacity
        sprite.setColor(sf::Color(255, 255, 255, 255));
    }

    // === Start countdown on death ===
    if (health <= 0 && !isRespawning)
    {
        isRespawning = true;
        countdownValue = 3;
        respawnTimer = 1.f;

        // Show "3" — assume it's at (64, 0) row, tile size 32×32
        sprite.setTextureRect(sf::IntRect({646, 289}, {16, 16})); // "3" is at index 2
        sprite.setScale({3.f, 3.f});
        sprite.setColor(sf::Color(255, 255, 255, 255));
        std::cout << "Player dying... countdown begins\n";
    }

    sf::Vector2f move(0.f, 0.f);
    float speed = 300.f; // units per second — adjust as needed

    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::W))
    {
        sf::Vector2f newPos = sprite.getPosition();
        newPos.y += 8.f;
        newPos.x += 8.f;
        int x = (int)(newPos.x / 16), y = (int)((newPos.y - 8.5f) / 16);

        if (gi->collMap[y][x] == 0)
        {
            move.y -= 1.f;
        }
        std::cerr << sprite.getPosition().x + 8.f << " " << sprite.getPosition().y + 8.f << std::endl;
        std::cerr << x << " " << y << std::endl;
    }
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::S))
    {
        sf::Vector2f newPos = sprite.getPosition();
        newPos.y += 8.f;
        newPos.x += 8.f;
        int x = (int)(newPos.x / 16), y = (int)((newPos.y + 8.5f) / 16);

        if (gi->collMap[y][x] == 0)
        {
            move.y += 1.f;
        }

        std::cerr << sprite.getPosition().x + 8.f << " " << sprite.getPosition().y + 8.f << std::endl;
        std::cerr << x << " " << y << std::endl;
    }
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::A))
    {
        sf::Vector2f newPos = sprite.getPosition();
        newPos.y += 8.f;
        newPos.x += 8.f;
        int x = (int)((newPos.x - 8.5f) / 16), y = (int)(newPos.y / 16);

        if (gi->collMap[y][x] == 0)
        {
            move.x -= 1.f;
        }

        std::cerr << x << " " << y << std::endl;
    }
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::D))
    {
        sf::Vector2f newPos = sprite.getPosition();
        newPos.y += 8.f;
        newPos.x += 8.f;
        int x = (int)((newPos.x + 8.5f) / 16), y = (int)(newPos.y / 16);

        if (gi->collMap[y][x] == 0)
        {
            move.x += 1.f;
        }

        std::cerr << x << " " << y << std::endl;
    }

    // Normalize movement
    if (move.x != 0.f || move.y != 0.f)
    {
        float len = std::sqrt(move.x * move.x + move.y * move.y);
        move /= len;
        move *= speed * dt; // scale movement by speed and time
        sprite.move(move);  // apply movement
    }
    cameraCenter += (sprite.getPosition() - cameraCenter) * cameraLag * dt; // interpolate camera
}

void Player::draw(sf::RenderWindow &window)
{
    window.draw(sprite);
}

int Player::gethealth() { return health; }

sf::View Player::getCurrentView() const
{
    sf::View view;
    if (debugMode)
    {
        view.setSize(cheatViewSize);
        view.setCenter(cheatCameraCenter);
    }
    else
    {
        view.setSize(normalViewSize);
        view.setCenter(cameraCenter);
    }

    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::LShift) && !debugMode)
    {
        view.zoom(2.f);
    }
    return view;
}

void Player::cheatlook(sf::RenderWindow &, float dt)
{
    bool keyOneNow = sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Num1);
    bool keyZeroNow = sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Num0);
    bool bothNowPressed = keyOneNow && keyZeroNow;
    bool bothPrevPressed = keyOnePrev && keyZeroPrev;

    if (bothNowPressed && !bothPrevPressed)
    {
        debugMode = !debugMode;
        if (debugMode)
            cheatCameraCenter = cameraCenter; // start cheat camera at player pos
        std::cout << (debugMode ? "CHEAT MODE ENABLED\n" : "CHEAT MODE DISABLED\n");
    }

    keyOnePrev = keyOneNow;
    keyZeroPrev = keyZeroNow;

    if (debugMode)
    {
        float speed = 1600.f * dt;
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Left))
            cheatCameraCenter.x -= speed;
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Right))
            cheatCameraCenter.x += speed;
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Up))
            cheatCameraCenter.y -= speed;
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Down))
            cheatCameraCenter.y += speed;
    }
}

sf::Vector2f Player::getPosition() const
{
    return sprite.getPosition();
}

sf::Vector2f Player::getPosition()
{
    return sprite.getPosition();
}

Crosshair::Crosshair(sf::Texture &tex, int row, int col)
    : sprite(tex, sf::IntRect({col * 17, row * 17}, {16, 16}))
{
    sprite.setPosition({300.f, 200.f});
    sprite.setScale({2.f, 2.f});
}

void Crosshair::draw(sf::RenderWindow &window)
{
    window.draw(sprite);
}
void Crosshair::update(const sf::RenderWindow &window)
{
    sf::Vector2f mousePos = window.mapPixelToCoords(sf::Mouse::getPosition(window));
    mousePos.x -= 16.f;
    mousePos.y -= 16.f;
    sprite.setPosition(static_cast<sf::Vector2f>(mousePos));
}

Bullet::Bullet(sf::Texture &tex, const sf::Vector2f &startPos, const sf::Vector2f &targetPos, int row, int col)
    : sprite(tex, sf::IntRect({col * 17, row * 17}, {16, 16}))
{
    sprite.setPosition(startPos);
    sprite.setScale({1.f, 1.f});
    sf::Vector2f direction = targetPos - startPos;
    float length = std::sqrt(direction.x * direction.x + direction.y * direction.y);
    if (length != 0)
        direction /= length;

    float speed = 600.f;
    velocity = direction * speed;
}

void Bullet::update(float deltaTime)
{
    sprite.move(velocity * deltaTime);
}

void Bullet::draw(sf::RenderWindow &window) const
{
    window.draw(sprite);
}

bool Bullet::isOffScreen(const sf::RenderWindow &window) const
{
    sf::Vector2f pos = sprite.getPosition();
    sf::Vector2u size = window.getSize();
    return pos.x < 0 || pos.y < 0 || pos.x > size.x || pos.y > size.y;
}

void handleShooting(std::vector<Bullet> &bullets, sf::Texture &tex, const sf::Vector2f &playerPos, const sf::RenderWindow &window, GameInfo &gi)
{
    if (gi.disableshoot)
        return;

    static bool wasMousePressed = false;

    bool isMousePressed = sf::Mouse::isButtonPressed(sf::Mouse::Button::Left);

    if (isMousePressed && !wasMousePressed)
    {
        // Just clicked this frame (mouse down event)

        sf::Vector2i mousePixel = sf::Mouse::getPosition(window);
        sf::Vector2f mouseWorld = window.mapPixelToCoords(mousePixel);

        sf::Vector2f playercenter = playerPos;
        sf::Vector2f mousecenter = mouseWorld - sf::Vector2f(8.f, 8.f);

        bullets.emplace_back(tex, playercenter, mousecenter, 4, 22); // adjust sprite row/col
    }

    // Update mouse state for next frame
    wasMousePressed = isMousePressed;
}

/*void updateBullets(std::vector<Bullet>& bullets, float dt, const sf::RenderWindow& window) {
    for (auto& bullet : bullets) {
        bullet.update(dt);
    }
}*/

void updateBullets(std::vector<Bullet> &bullets, float dt, const sf::RenderWindow &window, GameInfo &gi)
{
    for (size_t i = 0; i < bullets.size();)
    {
        bullets[i].update(dt);

        // Get bullet position from sprite
        sf::Vector2f pos = bullets[i].sprite.getPosition();
        int col = static_cast<int>(pos.x) / 16;
        int row = static_cast<int>(pos.y) / 16;

        // Check collision with map
        if (row >= 0 && row < (int)gi.collMap.size() &&
            col >= 0 && col < (int)gi.collMap[0].size())
        {
            if (gi.collMap[row][col] == 1)
            {
                bullets[i].isAlive = false;
            }
        }
        else
        {
            bullets[i].isAlive = false; // Out of bounds
        }

        // Remove dead bullet
        if (!bullets[i].isAlive)
        {
            bullets.erase(bullets.begin() + i);
        }
        else
        {
            ++i;
        }
    }
}

Key::Key(sf::Vector2f position, sf::Texture &tex)
    : sprite(tex, sf::IntRect({32 * 17, 11 * 17}, {16, 16}))
{
    sprite.setPosition(position);
    sprite.setScale({1.f, 1.f});
}

void Key::draw(sf::RenderWindow &window) const
{
    if (!collected)
        window.draw(sprite);
}

void Key::checkCollision(const sf::Vector2f &playerPos, float radius)
{
    if (collected)
        return;

    sf::Vector2f keyPos = sprite.getPosition();
    float dist = std::hypot(playerPos.x - keyPos.x, playerPos.y - keyPos.y);

    if (dist < radius)
        collected = true;
}

bool Key::isCollected() const
{
    return collected;
}

Chest::Chest(GameInfo &gameinfo)
    : sprite(gameinfo.pltex, sf::IntRect({10 * 17, 6 * 17}, {16, 16}))
{
    sprite.setScale({1.f, 1.f});
    gi = &gameinfo;
}

void Chest::setpos(sf::Vector2f position)
{
    sprite.setPosition(position);
}

void Chest::draw(sf::RenderWindow &window) const
{
    window.draw(sprite);
}

void Chest::tryOpen(const sf::Vector2f &playerPos, float radius, bool allKeysCollected)
{
    if (opened || !allKeysCollected)
        return;
    if (!gi->bossdeath)
        return;
    sf::Vector2f chestPos = sprite.getPosition();
    float dist = std::hypot(playerPos.x - chestPos.x, playerPos.y - chestPos.y);

    if (!opened && dist < radius && allKeysCollected)
    {
        opened = true;
        sprite.setTextureRect(sf::IntRect({11 * 17, 6 * 17}, {16, 16}));
        sprite.setColor(sf::Color::Yellow); // Update to opened chest tile
    }
}

bool Chest::isOpened() const
{
    return opened;
}

bool allKeysCollected(const std::vector<Key> &keys)
{
    return std::all_of(keys.begin(), keys.end(),
                       [](const Key &k)
                       { return k.isCollected(); });
}

void handleKeyChestInteraction(std::vector<Key> &keys, Chest &chest, const sf::Vector2f &playerPos, GameInfo &gameinfo)
{
    for (auto &key : keys)
        key.checkCollision(playerPos, 10.0f);

    if (allKeysCollected(keys))
    {
        gameinfo.active = true;
    }

    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Space))
        chest.tryOpen(playerPos, 55.0f, gameinfo.active);
}

Boss::Boss(GameInfo &gameinfo)
    : sprite(gameinfo.pltex, sf::IntRect({35 * 17, 14 * 17}, {16, 16}))
{
    gi = &gameinfo;
    sprite.setScale({1.5f, 1.5f}); // Adjust as needed
    velocity = {100, 100};         // start moving diagonally
}

sf::FloatRect Boss::getBounds() const
{
    return sprite.getGlobalBounds();
}

void Boss::setpos(sf::Vector2f pos)
{
    sprite.setPosition(pos);
}
void Boss::draw(sf::RenderWindow &window) const
{
    if (gi->active)
        window.draw(sprite);
}

void Boss::update(float dt, sf::RenderWindow &window, std::vector<Bullet> &playerBullets, std::vector<EnemyBullet> &enemyBullets)
{
    if (!gi->active)
        return;

    if (defeated)
    {
        deathTimer += dt;
        if (deathTimer > 0.8f)
        {
            gi->active = false;
        }
        return;
    }

    handlePlayerBulletCollision(playerBullets);

    if (health <= 0)
    {
        defeated = true;
        gi->bossdeath = true;
        sprite.setTextureRect(sf::IntRect({38 * 17, 14 * 17}, {16, 16})); // death sprite
        return;
    }

    if (health <= 20 && !angry)
    {
        angry = true;
        sprite.setTextureRect(sf::IntRect({36 * 17, 14 * 17}, {16, 16})); // angry sprite
        velocity *= 2.f;                                                  // speed boost
    }

    // Handle hit flash timer
    // Handle hit flash transparency
    if (hitFlashTimer > 0.f)
    {
        hitFlashTimer -= dt;

        // Make sprite transparent while flashing
        sprite.setColor(sf::Color(255, 255, 255, 100)); // 100 = semi-transparent
    }
    else
    {
        // Restore full opacity
        sprite.setColor(sf::Color(255, 255, 255, 255));
    }

    sf::FloatRect nextBounds = sprite.getGlobalBounds();
    nextBounds.position += velocity * dt;

    handleWallBounce(dt, enemyBullets);

    sprite.move(velocity * dt);
}

void Boss::handleWallBounce(float dt, std::vector<EnemyBullet> &enemyBullets)
{
    // accumulate shoot timer
    timeSinceLastShot += dt;

    sf::FloatRect current = sprite.getGlobalBounds();
    sf::FloatRect nextBounds = current;
    nextBounds.position += velocity * dt;

    bool bounced = false;

    if (checkWallCollision({{nextBounds.position.x, current.position.y}, nextBounds.size}))
    {
        velocity.x = -velocity.x;
        bounced = true;
    }

    if (checkWallCollision({{current.position.x, nextBounds.position.y}, nextBounds.size}))
    {
        velocity.y = -velocity.y;
        bounced = true;
    }

    if (bounced && timeSinceLastShot >= shootCooldown)
    {
        timeSinceLastShot = 0.f; // reset timer

        sf::Vector2f center = sprite.getPosition() + sprite.getGlobalBounds().size / 2.f;
        float bulletSpeed = angry ? 300.f : 150.f;

        enemyBullets.emplace_back(center, sf::Vector2f(0.f, -1.f * bulletSpeed));
        enemyBullets.emplace_back(center, sf::Vector2f(0.f, 1.f * bulletSpeed));
        enemyBullets.emplace_back(center, sf::Vector2f(-1.f * bulletSpeed, 0.f));
        enemyBullets.emplace_back(center, sf::Vector2f(1.f * bulletSpeed, 0.f));
    }
}

bool Boss::checkWallCollision(const sf::FloatRect &bounds)
{
    if (gi->collMap.empty() || gi->collMap[0].empty())
        return false;

    const int tileSize = 16;

    int left = std::max(0, static_cast<int>(bounds.position.x) / tileSize);
    int right = std::min((int)gi->collMap[0].size() - 1, static_cast<int>((bounds.position.x + bounds.size.x - 1) / tileSize));
    int top = std::max(0, static_cast<int>(bounds.position.y) / tileSize);
    int bottom = std::min((int)gi->collMap.size() - 1, static_cast<int>((bounds.position.y + bounds.size.y - 1) / tileSize));

    for (int y = top; y <= bottom; ++y)
        for (int x = left; x <= right; ++x)
            if (gi->collMap[y][x] == 1)
                return true;

    return false;
}

void Boss::handlePlayerBulletCollision(std::vector<Bullet> &playerBullets)
{
    sf::FloatRect bossBounds = sprite.getGlobalBounds();

    for (auto it = playerBullets.begin(); it != playerBullets.end();)
    {
        // Check collision using findIntersection to match your existing style
        if (it->getBounds().findIntersection(bossBounds).has_value())
        {
            health--;
            hitFlashTimer = hitFlashDuration; // start hit flash effect
            isVisible = false;
            it = playerBullets.erase(it); // remove bullet on hit
        }
        else
        {
            ++it;
        }
    }
}
