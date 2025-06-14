#ifndef PLAYER_H
#define PLAYER_H

#include <SFML/Graphics.hpp>
#include <vector>
#include <array>
#include <memory>
#include "enemy.hpp"


class Player {
public:
    Player(sf::Texture& tex, int row, int col, std::vector<std::vector<int>> collisionMap);

     void update(float dt, std::vector<EnemyBullet>& enemyBullets);
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
    float speed = 1000.f;
     float cameraLag = 4.0f;
     bool debugMode = false;
     bool keyOnePrev = false;
     bool keyZeroPrev = false;
    std::string cheatBuffer;
    sf::Vector2f cheatCameraCenter;          // center for cheat mode camera
    const sf::Vector2f normalViewSize{500.f, 500.f};  // your normal view size
    const sf::Vector2f cheatViewSize{normalViewSize.x * 5.5f, normalViewSize.y * 5.5f}; // wider cheat view

    int health = 4;
    float invincibleTimer = 0.f;
    const float INVINCIBLE_TIME = 1.0f; 

    void checkBulletCollisions(std::vector<EnemyBullet>& enemyBullets);

};


class Crosshair {
public:
    Crosshair(sf::Texture& tex,int row,int col);

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
    Bullet(sf::Texture& tex,const sf::Vector2f& startPos, const sf::Vector2f& targetPos,int row,int col);

    void update(float deltaTime);
    void draw(sf::RenderWindow& window) const;
    bool isOffScreen(const sf::RenderWindow& window) const;
    sf::FloatRect getBounds() const { return sprite.getGlobalBounds(); }



private:
    sf::Sprite sprite;
    sf::Vector2f velocity;
};

void handleShooting(std::vector<Bullet>& bullets, sf::Texture& tex, const sf::Vector2f& playerPos, const sf::RenderWindow& window);
void updateBullets(std::vector<Bullet>& bullets, float dt, const sf::RenderWindow& window);



class Key {
public:
    Key(sf::Texture& texture, sf::Vector2f position,int col,int row);
    void draw(sf::RenderWindow& window) const;
    void checkCollision(const sf::Vector2f& playerPos, float radius);
    bool isCollected() const;

private:
    sf::Sprite sprite;
    bool collected = false;
};

class Chest {
public:
    Chest(sf::Texture& texture, sf::Vector2f position);
    void draw(sf::RenderWindow& window) const;
    void tryOpen(const sf::Vector2f& playerPos, float radius, bool allKeysCollected);
    bool isOpened() const;

private:
    sf::Sprite sprite;
    bool opened = false;
};

void handleKeyChestInteraction(std::vector<Key>& keys, Chest& chest, const sf::Vector2f& playerPos);





#endif






