#pragma once
#include <SFML/Graphics.hpp>
#include "Map.h"
#include "Sensor.h"
#include "Direction.h"

/**
 * Represents a robot that can navigate a map using sensors and Dijkstra's algorithm.
 * 
 * The robot maintains a local copy of the map without obstacles (robotMap) and updates it based on sensor feedback.
 * It follows a path, recalculating if an obstacle is detected during motion.
 */
class Robot{
    private: 
        int r=25; /// Radius of the robot shape
        int x, y; /// Current coordinates
        sf::CircleShape shape; ///SFML circle representing the robot
        float speed=0.5f;
        
        Map * map; /// Pointer to the real map (with obstacles)

        /// Map replica used by the robot (same structure but initially no obstacles)
        Map robotMap; 

        int currentTile = -1;
        std::vector<int> pathToFollow; 
        int currentStep = 0;
        int startTile = -1; 
        int endTile = -1;
        bool pathComputed = false; 
        bool robotPlaced = false; 
        sf::RenderWindow * win; 
        bool canRunAlgo = false;

        std::vector<Sensor> sensors; /// Sensors for obstacle detection (Up, Down, Left, Right)
        std::vector<bool> detected; /// Obstacle detection flags from sensors
        bool nextMoveIsValide = true;
        bool needToComputePath = true; 

    public: 
        Robot(Map * m, int x_init, int y_init, int initR, sf::RenderWindow * w);
        
        void setX(int new_x);
        void setY(int new_y);
        
        void setCanRunAlgo(bool b){canRunAlgo = b;}
        void setStartTile(int i){startTile = i; currentTile =i;}
        void setEndTile(int i){endTile= i;}
        
        /**
         * Main update function to move and control the robot's behavior on the map.
         */
        void update(sf::RenderWindow& window);

        /// Movement in four directions
        void moveXpos();
        void moveXneg();
        void moveYpos();
        void moveYneg();

        void setPathComputed(bool b){pathComputed = b;}
        

        /**
         * Determines the movement direction from the current tile to a target tile.
         */
        Direction getNewDirection(int tile);
        
        /**
         * Places the robot on the map at tile (col, row).
         */
        void placeRobot(int c, int r);

        /**
         * Sets the path for the robot to follow and marks it on the map.
         */
        void setPath(const std::vector<int>& newPath);

        int  getCurrentTile(){return currentTile;}

        /// Phase3
        Map * getRobotMap(){return &robotMap;}

};
