#include "enemy.hpp"
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <vector>
#include <iostream>
#include <cmath>

const float BULLET_SPEED = 250.0f;
const float ENEMY_SPEED = 60.0f;
const float SHOOT_RANGE = 300.0f;
const float EXPLODE_RANGE = 50.0f;

const int TILE_SIZE = 16;
const int MAP_WIDTH = 10;
const int MAP_HEIGHT = 10;

enum TileType
{
    EMPTY,
    NORMAL,
    EXPLODING,
    SPIKE
};

struct Tile
{
    TileType type;
    sf::Sprite sprite;
    float standingTime = 0.0f;
    bool exploded = false;

    Tile(TileType t, const sf::Texture &texture, sf::Vector2f pos)
        : type(t), sprite(texture)
    {
        sf::IntRect rect;
        switch (t)
        {
        case NORMAL:
            rect = sf::IntRect({0, 0}, {TILE_SIZE, TILE_SIZE});
            break;
        case EXPLODING:
            rect = sf::IntRect({TILE_SIZE, 0}, {TILE_SIZE, TILE_SIZE});
            break;
        case SPIKE:
            rect = sf::IntRect({2 * TILE_SIZE, 0}, {TILE_SIZE, TILE_SIZE});
            break;
        default:
            rect = sf::IntRect({0, 0}, {TILE_SIZE, TILE_SIZE});
            break;
        }

        sprite.setTextureRect(rect);
        sprite.setPosition(pos);
    }
};
sf::Vector2f normalize(sf::Vector2f v)
{
    float len = std::sqrt(v.x * v.x + v.y * v.y);
    return len != 0 ? v / len : sf::Vector2f(0, 0);
}
struct Bullet
{
    sf::CircleShape shape;
    sf::Vector2f velocity;

    Bullet(sf::Vector2f pos, sf::Vector2f dir)
    {
        shape.setRadius(5);
        shape.setFillColor(sf::Color::Red);
        shape.setPosition(pos);
        velocity = normalize(dir) * BULLET_SPEED;
    }

    void update(float dt)
    {
        shape.move(velocity * dt);
    }
};



void runGame()
{
    sf::RenderWindow window(sf::VideoMode({640, 480}), "Shooter + Spike + Exploder");
    window.setFramerateLimit(60);

    // Load textures
    sf::Texture tilesheet;
    if (!tilesheet.loadFromFile("colored-transparent.png"))
    {
        std::cerr << "Failed to load tilesheet!\n";
        return;
    }

    // Load sound
    sf::SoundBuffer explosionBuffer;
    if (!explosionBuffer.loadFromFile("impactGlass_heavy_001.ogg"))
    {
        std::cerr << "Failed to load explosion sound!\n";
        return;
    }
    sf::Sound explosionSound(explosionBuffer);

    // Static Player
    sf::Sprite player(tilesheet);
    player.setTextureRect(sf::IntRect({(26 - 1) * 17, (5 - 1) * 17}, {16, 16}));
    player.setScale({2, 2});
    player.setPosition({100, 100});

    // Shooter
    sf::Sprite shooter(tilesheet);
    shooter.setTextureRect(sf::IntRect({(19 - 1) * 17, (9 - 1) * 17}, {16, 16}));
    shooter.setScale({2, 2});
    shooter.setPosition({300, 100});

    // Exploder
    sf::Sprite exploder(tilesheet);
    exploder.setTextureRect(sf::IntRect({(19 - 1) * 17, (8 - 1) * 17}, {16, 16}));
    exploder.setScale({2, 2});
    exploder.setPosition({500, 100});

    // Bullets
    std::vector<Bullet> bullets;
    sf::Clock shootClock;
    sf::Clock deltaClock;

    // Tilemap
    std::vector<std::vector<Tile>> tilemap;
    for (int y = 0; y < MAP_HEIGHT; ++y)
    {
        std::vector<Tile> row;
        for (int x = 0; x < MAP_WIDTH; ++x)
        {
            TileType type = NORMAL;
            if ((x == 4 && y == 4) || (x == 1 && y == 1) || (x == 2 && y == 5) || (x == 7 && y == 3))
                type = EXPLODING;
            if (x == 6 && y == 6)
                type = SPIKE;

            row.emplace_back(type, tilesheet, sf::Vector2f(x * TILE_SIZE, y * TILE_SIZE));
        }
        tilemap.push_back(row);
    }

    while (window.isOpen())
    {
        float dt = deltaClock.restart().asSeconds();

        while (const std::optional<sf::Event> event = window.pollEvent())
        {
            if (event->is<sf::Event::Closed>())
                window.close();
        }

        // SHOOTER ENEMY
        sf::Vector2f toPlayer = player.getPosition() - shooter.getPosition();
        if (std::sqrt(toPlayer.x * toPlayer.x + toPlayer.y * toPlayer.y) < SHOOT_RANGE)
        {
            if (shootClock.getElapsedTime().asSeconds() > 0.5f)
            {
                bullets.emplace_back(shooter.getPosition(), toPlayer);
                shootClock.restart();
            }
        }

        // EXPLODER ENEMY
        sf::Vector2f toExplode = player.getPosition() - exploder.getPosition();
        if (std::sqrt(toExplode.x * toExplode.x + toExplode.y * toExplode.y) < EXPLODE_RANGE)
        {
            exploder.setColor(sf::Color::Transparent);
        }
        else
        {
            exploder.move(normalize(toExplode) * ENEMY_SPEED * dt);
        }

        // BULLET UPDATE
        for (auto &b : bullets)
            b.update(dt);

        // TILE INTERACTIONS
        int tileX = static_cast<int>(player.getPosition().x) / TILE_SIZE;
        int tileY = static_cast<int>(player.getPosition().y) / TILE_SIZE;

        if (tileX >= 0 && tileX < MAP_WIDTH && tileY >= 0 && tileY < MAP_HEIGHT)
        {
            Tile &current = tilemap[tileY][tileX];

            if (current.type == EXPLODING)
            {
                current.standingTime += dt;
                if (current.standingTime >= 3.0f && !current.exploded)
                {
                    current.exploded = true;
                    current.sprite.setTextureRect(sf::IntRect({(49 - 1) * 17, (14 - 1) * 17}, {16, 16}));
                    explosionSound.play();
                }
            }
            else
            {
                current.standingTime = 0.0f;
            }

            if (current.type == SPIKE)
            {
                std::cout << "Player stepped on spike!\n";
            }
        }

        // RENDER
        window.clear();
        for (auto &row : tilemap)
            for (auto &tile : row)
                window.draw(tile.sprite);

        window.draw(player);
        if (exploder.getColor() != sf::Color::Transparent)
            window.draw(exploder);
        window.draw(shooter);
        for (auto &b : bullets)
            window.draw(b.shape);

        window.display();
    }
}
