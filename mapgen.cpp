#include <SFML/Graphics.hpp>
#include <bits/stdc++.h>
#include "mapgen.h"

bool TileMap::load(const std::string &tileset, sf::Vector2u tileSize, const std::vector<std::vector<int>> &tiles, unsigned int width, unsigned int height, unsigned int gap)
{
    // Check if Tileset exists
    if (!m_tileset.loadFromFile(tileset))
        return false;

    // Resizing vertex array to fit level size
    m_vertices.setPrimitiveType(sf::PrimitiveType::Triangles);
    m_vertices.resize(tiles.size() * tiles[0].size() * 6); // 6 vertecies per tile
    int cur_Vertex = 0;

    // Populate vertex array with two triangles per tile
    for (unsigned int i = 0; i < tiles.size(); i++)
    {
        for (unsigned int j = 0; j < tiles[i].size(); j++)
        {
            // Tilenumber
            int tileNum = tiles[i][j];

            // Finding Position in Tileset Texture
            // Note that here the tileset has gap **between** all tiles.
            int tu = ((tileNum - 1) % ((tileSize.x + gap) / (width + gap))) * (width + gap);
            int tv = ((tileNum - 1) / ((tileSize.x + gap) / (width + gap))) * (height + gap);

            // Get 2 triangles for each tile
            sf::Vertex *tri1 = &m_vertices[cur_Vertex];
            sf::Vertex *tri2 = &m_vertices[cur_Vertex + 3];

            cur_Vertex += 6;

            // Setting positions
            unsigned int x = j * width;
            unsigned int y = i * height;
            unsigned int scale = 1;
            x *= scale;
            y *= scale;
            // x+=gap; y+=gap;
            tri1[0].position = sf::Vector2f(x, y);
            tri1[1].position = sf::Vector2f(x + width * scale, y);
            tri1[2].position = sf::Vector2f(x + width * scale, y + height * scale);

            tri2[0].position = sf::Vector2f(x, y);
            tri2[1].position = sf::Vector2f(x, y + height * scale);
            tri2[2].position = sf::Vector2f(x + width * scale, y + height * scale);

            // Setting texture coordinates
            tri1[0].texCoords = sf::Vector2f(tu, tv);
            tri1[1].texCoords = sf::Vector2f(tu + width, tv);
            tri1[2].texCoords = sf::Vector2f(tu + width, tv + height);

            tri2[0].texCoords = sf::Vector2f(tu, tv);
            tri2[1].texCoords = sf::Vector2f(tu, tv + height);
            tri2[2].texCoords = sf::Vector2f(tu + width, tv + height);
        }
    }

    return true;
}

void TileMap::draw(sf::RenderTarget &target, sf::RenderStates states) const
{
    // apply the transform
    states.transform *= getTransform();

    // apply the tileset texture
    states.texture = &m_tileset;

    // draw the vertex array
    target.draw(m_vertices, states);
}
