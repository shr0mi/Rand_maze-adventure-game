//To avoid multiple inclusions
#ifndef TILEMAP_H
#define TILEMAP_H

#include <SFML/Graphics.hpp>
#include<bits/stdc++.h>

class TileMap : public sf::Drawable, public sf::Transformable{
    public:
        bool load(const std::string& tileset, sf::Vector2u tileSize, const std::vector<std::vector<int>>& tiles, unsigned int width, unsigned int height, unsigned int gap);

    private:
        virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const override;
        sf::VertexArray m_vertices;
        sf::Texture m_tileset;

};

#endif