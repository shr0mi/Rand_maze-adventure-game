#include "player.h"
#include <cmath>
#include <algorithm>

constexpr float BULLET_COOLDOWN = 0.2f; // 5 bullets per second

Player::Player(sf::Texture& tex, int row, int col)
    : sprite(tex, sf::IntRect({col * 17, row * 17}, {16, 16})) {
    sprite.setPosition({400.f, 300.f});
    sprite.setScale({5.f,5.f});
}

void Player::update(float dt) {
    sf::Vector2f move(0.f, 0.f);
    float speed = 400.f; // units per second — adjust as needed

    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::W)) move.y -= 1.f;
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::S)) move.y += 1.f;
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::A)) move.x -= 1.f;
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::D)) move.x += 1.f;

    // Normalize movement
    if (move.x != 0.f || move.y != 0.f) {
        float len = std::sqrt(move.x * move.x + move.y * move.y);
        move /= len;
        move *= speed * dt; // scale movement by speed and time
        sprite.move(move);  // apply movement
    }
}

void Player::draw(sf::RenderWindow& window) {
    window.draw(sprite);
}

sf::Vector2f Player::getPosition() const {
    return sprite.getPosition();
}

sf::Vector2f Player::getPosition() {
    return sprite.getPosition();

}


Crosshair::Crosshair(sf::Texture& tex, int row, int col)
    : sprite(tex, sf::IntRect({col * 17, row * 17}, {16, 16})) {
    sprite.setPosition({300.f, 200.f});
    sprite.setScale({3.f,3.f});
}

void Crosshair::draw(sf::RenderWindow& window) {
    window.draw(sprite);
}
void Crosshair::update(const sf::RenderWindow& window) {
    sf::Vector2i mousePos = sf::Mouse::getPosition(window);
    sprite.setPosition(static_cast<sf::Vector2f>(mousePos));
}

Bullet::Bullet(sf::Texture& tex,const sf::Vector2f& startPos, const sf::Vector2f& targetPos,int row,int col)
 : sprite(tex, sf::IntRect({col * 17, row * 17}, {16, 16})){
    sprite.setPosition(startPos);
    sprite.setScale({5.f,5.f});
    sf::Vector2f direction = targetPos - startPos;
    float length = std::sqrt(direction.x * direction.x + direction.y * direction.y);
    if (length != 0)
        direction /= length;

    float speed = 600.f;    
    velocity = direction * speed;

}

void Bullet::update(float deltaTime) {
    sprite.move(velocity * deltaTime);
}

void Bullet::draw(sf::RenderWindow& window) const {
    window.draw(sprite);
}

bool Bullet::isOffScreen(const sf::RenderWindow& window) const {
    sf::Vector2f pos = sprite.getPosition();
    sf::Vector2u size = window.getSize();
    return pos.x < 0 || pos.y < 0 || pos.x > size.x || pos.y > size.y;
}

void handleShooting(std::vector<Bullet>& bullets, sf::Texture& tex, const sf::Vector2f& playerPos, const sf::RenderWindow& window) {
    static float cooldownTimer = 0.f;
    static sf::Clock shootClock;

    float dt = shootClock.restart().asSeconds();
    cooldownTimer -= dt;

    if (sf::Mouse::isButtonPressed(sf::Mouse::Button::Left) && cooldownTimer <= 0.f) {
        sf::Vector2i mousePixel = sf::Mouse::getPosition(window);
        sf::Vector2f mouseWorld = window.mapPixelToCoords(mousePixel);

        sf::Vector2f playercenter = playerPos + sf::Vector2f(8.f, 8.f);

        bullets.emplace_back(tex, playercenter, mouseWorld, 4,22); // adjust sprite row/col
        cooldownTimer = BULLET_COOLDOWN;
    }
}

void updateBullets(std::vector<Bullet>& bullets, float dt, const sf::RenderWindow& window) {
    for (auto& bullet : bullets) {
        bullet.update(dt);
    }

    bullets.erase(std::remove_if(bullets.begin(), bullets.end(),
        [&](const Bullet& b) { return b.isOffScreen(window); }),
        bullets.end());
}

Key::Key(sf::Texture& tex, sf::Vector2f position, int col, int row)
    : sprite(tex, sf::IntRect({col * 17, row * 17}, {16, 16})) {
    sprite.setPosition(position);
    sprite.setScale({3.f,3.f});
}


void Key::draw(sf::RenderWindow& window) const {
    if (!collected)
        window.draw(sprite);
}

void Key::checkCollision(const sf::Vector2f& playerPos, float radius) {
    if (collected) return;

    sf::Vector2f keyPos = sprite.getPosition();
    float dist = std::hypot(playerPos.x - keyPos.x, playerPos.y - keyPos.y);

    if (dist < radius)
        collected = true;
}

bool Key::isCollected() const {
    return collected;
}


Chest::Chest(sf::Texture& texture, sf::Vector2f position)
 : sprite(texture, sf::IntRect({8 * 17, 6 * 17}, {16, 16})) {
    sprite.setPosition(position);
    sprite.setScale({6.f,6.f});
}

void Chest::draw(sf::RenderWindow& window) const {
        window.draw(sprite);
}

void Chest::tryOpen(const sf::Vector2f& playerPos, float radius, bool allKeysCollected) {
    if (opened || !allKeysCollected) return;

    sf::Vector2f chestPos = sprite.getPosition();
    float dist = std::hypot(playerPos.x - chestPos.x, playerPos.y - chestPos.y);

     if (!opened && dist < radius && allKeysCollected) {
        opened = true;
        // 🔁 Change to "opened" tile
        sprite.setTextureRect(sf::IntRect({9 * 17, 6 * 17}, {16, 16})); // Update to opened chest tile
    }
}

bool Chest::isOpened() const {
    return opened;
}

void handleKeyChestInteraction(std::vector<Key>& keys, Chest& chest, const sf::Vector2f& playerPos) {
    for (auto& key : keys)
        key.checkCollision(playerPos, 55.0f);

    bool allCollected = std::all_of(keys.begin(), keys.end(),
                                    [](const Key& k){ return k.isCollected(); });

    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Space))
        chest.tryOpen(playerPos, 55.0f, allCollected);
}





