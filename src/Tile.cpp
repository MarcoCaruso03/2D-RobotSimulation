#include "Tile.h"

/**
 * Construts the tile shape and inizializes its color and outline
 */
Tile::Tile(float x, float y, int s, TileType t) : type(t) {
    shape.setPosition(x, y);
    shape.setFillColor(sf::Color::White);
    shape.setOutlineColor(sf::Color::Black);   
    shape.setSize(sf::Vector2f(s,s));
    shape.setOutlineThickness(1.f);             
}

/**
 * Sets the tile type and updates its visual representation.
 */
void Tile::setType(TileType newType) {
    type = newType;
    switch (type) {
        case TileType::Empty:    shape.setFillColor(sf::Color::White); break;
        case TileType::Obstacle: shape.setFillColor(sf::Color::Black); break;
        case TileType::Start:    shape.setFillColor(sf::Color::Green); break;
        case TileType::Goal:     shape.setFillColor(sf::Color::Blue); break;
    }
}

/**
 * Returns the current tile type.
 */
TileType Tile::getType() const {
    return type;
}

/**
 * Draws the tile to the given SFML render window.
 */
void Tile::draw(sf::RenderWindow& window) const {
    window.draw(shape);
}

/**
 * Returns the internal RectangleShape (copy).
 */
sf::RectangleShape Tile::getShape(){
    return shape;
}
