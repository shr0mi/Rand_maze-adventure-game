#ifndef PLAYER_H
#define PLAYER_H

#include <SFML/Graphics.hpp>
#include <vector>
#include <array>
#include <memory>


class Player {
public:
    Player(sf::Texture& tex, int row, int col, std::vector<std::vector<int>> collisionMap);

    void update(float dt);
    void draw(sf::RenderWindow& window);
    sf::Vector2f getPosition() const;
    sf::Vector2f getPosition();
    sf::View getView() const;
    sf::Vector2f cameraCenter;


private:
    sf::Sprite sprite;
    float speed = 300.f;

     bool dashing = false;
    float dashSpeed = 800.f;
    float dashDuration = 0.1f;
    float dashTimeLeft = 0.f;
    sf::Vector2f dashDirection;
    float dashCooldown = 100.0f;  
    float dashCooldownTimer = 0.f;
     float cameraLag = 4.0f;
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







