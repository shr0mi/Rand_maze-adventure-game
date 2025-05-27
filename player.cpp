#include "player.h"
#include <cmath>

Player::Player(sf::Texture& tex, int row, int col)
    : sprite(tex, sf::IntRect({col * 17, row * 17}, {16, 16})) {
    sprite.setPosition({400.f, 300.f});
    sprite.setScale({5.f,5.f});
}

void Player::update(float dt) {
    sf::Vector2f move(0.f, 0.f);
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::W)) move.y -= 10;
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::S)) move.y += 10;
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::A)) move.x -= 10;
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::D)) move.x += 10;

    if (move.x || move.y) {
        float len = std::sqrt(move.x * move.x + move.y * move.y);
        move /= len;
        sprite.move(move * speed * dt);
    }
}

void Player::draw(sf::RenderWindow& window) {
    window.draw(sprite);
}
