#ifndef PLAYER_H
#define PLAYER_H

#include <SFML/Graphics.hpp>
#include <vector>
#include <array>
#include <memory>
#include "enemy.hpp"


class Player {
public:
    Player(sf::Texture& tex,std::vector<std::vector<int>> collisionMap);
    void setPos(sf::Vector2f pos);

    void update(float dt, std::vector<EnemyBullet>& enemyBullets,sf::RenderWindow& window,std::vector<std::shared_ptr<BaseEnemy>> enemies,sf::Vector2f pos,sf::FloatRect bossbounds,bool open);
    void draw(sf::RenderWindow& window);
    sf::Vector2f getPosition() const;
    sf::Vector2f getPosition();
    //sf::View getView() const;
    sf::Vector2f cameraCenter;
    sf::View getCurrentView() const;  // returns cheat view if debugMode else normal view
    void cheatlook(sf::RenderWindow& window, float dt);
    bool isDebugModeEnabled() const { return debugMode; }
    int gethealth();
    



private:
    sf::Sprite sprite;
     float cameraLag = 4.0f;
     bool debugMode = false;
     bool keyOnePrev = false;
     bool keyZeroPrev = false;
    std::string cheatBuffer;
    sf::Vector2f cheatCameraCenter;          // center for cheat mode camera
    const sf::Vector2f normalViewSize{500.f, 500.f};  // your normal view size
    const sf::Vector2f cheatViewSize{normalViewSize.x * 3.5f, normalViewSize.y * 3.5f}; // wider cheat view

    int health = 20;
    float invincibleTimer = 0.2f;
    const float INVINCIBLE_TIME = 2.0f; 

    void checkBulletCollisions(std::vector<EnemyBullet>& enemyBullets);
};


class Crosshair {
public:
    Crosshair();

    void draw(sf::RenderWindow& window);
    void update(const sf::RenderWindow& window);

    void setPosition(float x, float y);
    void setPosition(const sf::Vector2f& position);

private:
    sf::Texture texture;
    sf::Sprite sprite;
};


class Bullet {
public:
    Bullet(const sf::Vector2f& startPos, const sf::Vector2f& targetPos);

    void update(float deltaTime);
    void draw(sf::RenderWindow& window) const;
    bool isOffScreen(const sf::RenderWindow& window) const;
    sf::FloatRect getBounds() const { return sprite.getGlobalBounds(); }
    sf::Sprite sprite;
    bool isAlive = true;



private:
   
    sf::Vector2f velocity;
};

void handleShooting(std::vector<Bullet>& bullets, sf::Texture& tex, const sf::Vector2f& playerPos, const sf::RenderWindow& window);
void updateBullets(std::vector<Bullet>& bullets, float dt, const sf::RenderWindow& window);



class Key {
public:
    Key(sf::Vector2f position);
    void draw(sf::RenderWindow& window) const;
    void checkCollision(const sf::Vector2f& playerPos, float radius);
    bool isCollected() const;

private:
    sf::Sprite sprite;
    bool collected = false;
};

class Chest {
public:
    Chest();
    void setpos(sf::Vector2f position);
    void draw(sf::RenderWindow& window) const;
    void tryOpen(const sf::Vector2f& playerPos, float radius, bool allKeysCollected);
    bool allKeysCollected(const std::vector<Key>& keys);
    bool isOpened() const;

private:
    sf::Sprite sprite;
    bool opened = false;
};

void handleKeyChestInteraction(std::vector<Key>& keys, Chest& chest, const sf::Vector2f& playerPos);

class Boss {
    public:

    Boss();
    void setpos(sf::Vector2f position);
    void draw(sf::RenderWindow& window) const;
    void update(float dt,sf::RenderWindow& window,std::vector<Bullet>& playerBullets, std::vector<EnemyBullet>& enemyBullets);
    sf::FloatRect getBounds() const;

    private:
    sf::Sprite sprite;
    sf::Vector2f velocity;
    int health = 40;
    bool angry = false;
    bool defeated = false;
    float shootCooldown = 0.5f;
    float timeSinceLastShot = 0.f; 
    float deathTimer = 0.f;
    float normalspeed = 100.f;
    float angryspeed = 200.f;
    float hitFlashTimer = 0.f;    // counts time since last hit
    const float hitFlashDuration = 0.1f; // flash duration in seconds
    bool isVisible = true;        // whether to draw the sprite

    void handlePlayerBulletCollision(std::vector<Bullet>& playerBullets);
    void handleWallBounce(float dt,std::vector<EnemyBullet>& enemyBullets);
    bool checkWallCollision(const sf::FloatRect& bounds);

};

#endif






