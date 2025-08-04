#pragma once
#include <SFML/Graphics.hpp>
#include "Direction.h"

/// Forward declarations to avoid circular includes
class Robot; 
class Map;   

/**
 * Sensor class used by a robot to detect nearby obstacles. 
 * Each sensor is bound to a direction and can check for obstacles in that direction
 */
class Sensor{
    private: 
        bool detectionChecked;  /// True if an obstacles is detected in the direction
        Direction d; /// Direction this sensors is facing
        Robot * robot; /// Pointer to the robot using this sensor 
        Map * map; /// Pointer to the map
    public: 
        /**
         * Constructs a Sensor. 
         * @param direction Direction the sensor will monitor
         * @param r Pointer to the owning Robot
         * @param m Pointer to the Map where the Robot operates
         */
        Sensor(Direction direction, Robot * r, Map * m);

        /**
         * Activates the sensor and checks for obstacles. 
         */
        void active(); 

        /**
         * Resets the sensor's detection flag
         */
        void disable();

        /**
         * Returns the current detection state. 
         * @return True if an obstacle had been detected
         */
        bool getdetectionChecked(){return detectionChecked;}
};