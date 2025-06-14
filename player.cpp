#include <iostream>
#include "player.h"
#include "enemy.hpp"
#include <cmath>
#include <algorithm>

constexpr float BULLET_COOLDOWN = 0.2f; // 5 bullets per second

std::vector<std::vector<int>> collMap;

Player::Player(sf::Texture& tex, int row, int col, std::vector<std::vector<int>> collisionMap)
    : sprite(tex, sf::IntRect({col * 17, row * 17}, {17, 17})) {
    sprite.setPosition({340.f, 340.f});
    sprite.setScale({1.f,1.f});
    collMap = collisionMap;
}

void Player::update(float dt,std::vector<EnemyBullet>& enemyBullets) {
    if(debugMode) return;




if (invincibleTimer > 0.f)
    invincibleTimer -= dt;

// 2. Collision detection with enemy bullets
sf::FloatRect playerBounds = sprite.getGlobalBounds();

for (auto& bullet : enemyBullets) {
    if (bullet.shape.getGlobalBounds().findIntersection(playerBounds).has_value()) {
        if (invincibleTimer <= 0.f) {
            //health--;
            invincibleTimer = INVINCIBLE_TIME;

            std::cout << "Player hit! Health: " << health << "\n";
        }
        break; // Only one hit per frame
    }
}

// 3. Respawn if health reaches zero
if (health <= 0) {
    health = 4; // Reset health
    sprite.setPosition({340.f, 340.f}); // Respawn position
    std::cout << "Player respawned\n";
}



    sf::Vector2f move(0.f, 0.f);
    float speed = 200.f; // units per second — adjust as needed
    
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::W)){ 
        sf::Vector2f newPos = sprite.getPosition();
        newPos.y += 8.f; newPos.x += 8.f;
        int x = (int)(newPos.x/16), y = (int)((newPos.y - 8.5f)/16);

        if(collMap[y][x] == 0){
            move.y -= 1.f;
        }
        std::cerr << sprite.getPosition().x + 8.f << " " << sprite.getPosition().y + 8.f << std::endl;
        std::cerr << x << " " << y << std::endl;
        
    }
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::S)){
        sf::Vector2f newPos = sprite.getPosition();
        newPos.y += 8.f; newPos.x += 8.f;
        int x = (int)(newPos.x/16), y = (int)((newPos.y + 8.5f)/16);

        if(collMap[y][x]==0){
            move.y += 1.f;
        }

        std::cerr << sprite.getPosition().x + 8.f << " " << sprite.getPosition().y + 8.f << std::endl;
        std::cerr << x << " " << y << std::endl;
        
    }
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::A)){ 
        sf::Vector2f newPos = sprite.getPosition();
        newPos.y += 8.f; newPos.x += 8.f;
        int x = (int)((newPos.x - 8.5f)/16), y = (int)(newPos.y/16);
        

        if(collMap[y][x]==0){
            move.x -= 1.f;
        }

        std::cerr << x << " " << y << std::endl;
    }
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::D)){
        sf::Vector2f newPos = sprite.getPosition();
        newPos.y += 8.f; newPos.x += 8.f;
        int x = (int)((newPos.x + 8.5f)/16), y = (int)(newPos.y/16);

        if(collMap[y][x]==0){
            move.x += 1.f;
        }

        std::cerr << x << " " << y << std::endl;
    }

    // Normalize movement
    if (move.x != 0.f || move.y != 0.f) {
        float len = std::sqrt(move.x * move.x + move.y * move.y);
        move /= len;
        move *= speed * dt; // scale movement by speed and time
        sprite.move(move);  // apply movement
    }
     cameraCenter += (sprite.getPosition() - cameraCenter) * cameraLag * dt; // interpolate camera
}

void Player::draw(sf::RenderWindow& window) {
    window.draw(sprite);
}

int Player::gethealth() {return health;}

sf::View Player::getCurrentView() const {
    sf::View view;
    if (debugMode) {
        view.setSize(cheatViewSize);
        view.setCenter(cheatCameraCenter);
    } else {
        view.setSize(normalViewSize);
        view.setCenter(cameraCenter);
    }
    return view;
}

void Player::cheatlook(sf::RenderWindow&, float dt) {
    bool keyOneNow = sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Num1);
    bool keyZeroNow = sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Num0);
    bool bothNowPressed = keyOneNow && keyZeroNow;
    bool bothPrevPressed = keyOnePrev && keyZeroPrev;

    if (bothNowPressed && !bothPrevPressed) {
        debugMode = !debugMode;
        if (debugMode)
            cheatCameraCenter = cameraCenter; // start cheat camera at player pos
        std::cout << (debugMode ? "CHEAT MODE ENABLED\n" : "CHEAT MODE DISABLED\n");
    }

    keyOnePrev = keyOneNow;
    keyZeroPrev = keyZeroNow;

    if (debugMode) {
        float speed = 1600.f * dt;
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Left))  cheatCameraCenter.x -= speed;
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Right)) cheatCameraCenter.x += speed;
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Up))    cheatCameraCenter.y -= speed;
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Down))  cheatCameraCenter.y += speed;
    }
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
    sprite.setScale({2.f,2.f});
}

void Crosshair::draw(sf::RenderWindow& window) {
    window.draw(sprite);
}
void Crosshair::update(const sf::RenderWindow& window) {
    sf::Vector2f mousePos = window.mapPixelToCoords(sf::Mouse::getPosition(window));
    mousePos.x -= 16.f; mousePos.y -= 16.f;
    sprite.setPosition(static_cast<sf::Vector2f>(mousePos));
}

Bullet::Bullet(sf::Texture& tex,const sf::Vector2f& startPos, const sf::Vector2f& targetPos,int row,int col)
 : sprite(tex, sf::IntRect({col * 17, row * 17}, {16, 16})){
    sprite.setPosition(startPos);
    sprite.setScale({1.f,1.f});
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

        sf::Vector2f playercenter = playerPos;

        sf::Vector2f mousecenter = mouseWorld - sf::Vector2f(8.f,8.f);

        bullets.emplace_back(tex, playercenter, mousecenter, 4,22); // adjust sprite row/col
        cooldownTimer = BULLET_COOLDOWN;
    }
}

void updateBullets(std::vector<Bullet>& bullets, float dt, const sf::RenderWindow& window) {
    for (auto& bullet : bullets) {
        bullet.update(dt);
    }
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



