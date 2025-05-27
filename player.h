#ifndef PLAYER_H
#define PLAYER_H

#include <SFML/Graphics.hpp>

class Player {
public:
    Player(sf::Texture& tex, int row, int col);

    void update(float dt);
    void draw(sf::RenderWindow& window);

private:
    sf::Sprite sprite;
    float speed = 150.f;
};

#endif
