#pragma once
#include <SFML/Graphics.hpp>
#include <vector>
#include <iostream>
#include "Tile.h"
#include <limits>

/**
 * Represents a grid-based map made of tiles, used for robot pathfinding and rendering
 */
class Map{
    private: 
        int tilesSize = 50; /// Size of each tile (square)
        std::vector<Tile> tiles; /// Vector of all tiles
        int rows; /// Number of rows in the grid
        int cols; /// Number of columns in the grid

        /**
         * Graph representation of the map as an adjacency list. 
         * Each node is a tile index; edges connect traversable neighbors. 
         */
        std::vector<std::vector<std::pair<int,float>>> graph; 

    public: 
        /**
         * Constructs a Map with the specified windows dimension
         * @param windowsWidth Width of the SFML window
         * @param windowHeight Height of the SFML window.
         */
        Map(int const windowsWidth, int const windowHeight);

        /// Default constructor for cloning
        Map() : tilesSize(50), rows(0), cols(0) {}

        /**
         * Creates a copy of the map structure without obstacles. 
         * @return A new Map object with same grid but only empty tiles. 
         */
        Map cloneStructureWithoutObstacles() const; 

        /**
         *  Returns a reference to the vector of tiles.
         */
        std::vector<Tile>& getTiles(); 

        /**
         * Builds the graph structure representing walkable tile connections.
         */
        void buildGraph();

        /**
        *  Returns a const reference to the adjacency list.
        */
        const std::vector<std::vector<std::pair<int,float>>>& getGraph() const;

        /**
         * Prints the graph to standard output (for debugging)
         */
        void printGraph() const; 
        
        /**
         * Draws all tiles to the given SFML window
         */
        void draw(sf::RenderWindow& window) const;
        
        /**
         * Computes the shortest path between two tiles using Dijkstra's algorith
         * @param start Index of the start tile 
         * @param goal Index of the goal tile 
         * @return Vector of tile indices representing the path
         */
        std::vector<int> dijkstra(int start, int goal);
        
        
        int getTileSize(){return tilesSize;}
        int getCols(){return cols;}
        int getRows(){return rows;}
        
        /**
         * Sets the border color of a specific tile.
         * @param c Color to set.
         * @param id Tile index.
         */        
        void setBorderColorTile(sf::Color c, int id);
        
        /**
        * Highlights a path with a distinct color.
        * @param p Vector of tile indices representing the path.
        */
        void setColorPath(std::vector<int> p);
        
        /**
        * Resets the path color to default white (except for obstacles/start/goal).
        * @param p Vector of tile indices that should be reset.
        */
        void defaultColorTile(std::vector<int> p);
};