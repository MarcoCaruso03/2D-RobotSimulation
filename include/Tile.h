#pragma once
#include <SFML/Graphics.hpp>
/**
 * Enum representing different types of tiles in the map
 */

enum class TileType {
    Empty, /// Walkalble tile
    Obstacle, /// Non-walkable tile
    Start, /// Robot starting position
    Goal /// Destination tile 
};

/**
 * Class representing a single tile in the 2D grip map. 
 * Each tile is rendered as a quare and has a specific type
 */
class Tile {
    private:
        sf::RectangleShape shape; /// The drawable square shape
        TileType type; /// The tile's functional role
        int Size; /// Side length in pixels
    public: 
        /**
         * Constructs a tile at position (x,y) with size 's' and optional type
         * @param x X position in the window
         * @param y Y position in the window
         * @param s Size of the square side 
         * @param type Initial tile type (default is Empty)
         */
        Tile(float x, float y,  int s, TileType type = TileType::Empty);
        
        /**
         * Sets the type of the tile and updates its color accordingly
         */
        void setType(TileType newType);
        
        /**
         * Returns the current type of the tile
         */
        TileType getType() const;
        
        /**
         * Renders the tile on the provided window
         */
        void draw(sf::RenderWindow& window) const;

        /**
         * Return a copy of the internal shape
         */
        sf::RectangleShape getShape();
        
        /**
         * Sets the top-left position ofthe tile
         */
        void setPosition(float x, float y){shape.setPosition(x,y);}
        
        /**
         * Sets the fill color of the tile
         */
        void setFillColor(const sf::Color& color){shape.setFillColor(color);}
        
        /**
         * Sets the border color of the tile
         */
        void setBorderColor(const sf::Color& color){shape.setOutlineColor(color);}
        
        /**
         * Returns the global boundind box of the tile
         */
        sf::FloatRect getGlobalBounds() const { return shape.getGlobalBounds();}
};