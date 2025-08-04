#include "Sensor.h"
#include "Sensor.h"
#include "Robot.h"
#include "Map.h"


Sensor::Sensor(Direction Direction, Robot * r, Map * m){
    detectionChecked = false; 
    robot = r; 
    d = Direction;
    map = m;
}

/**
 * Checks the tile in the specificied direction from the robot's current tile.
 * If the tile contains an obstacle, sets detectionChecked to true
 */
void Sensor::active(){

    int currentTile = robot->getCurrentTile();
    int tileToBeChecked = -1;
    int row = currentTile / map->getCols();;
    int col = currentTile % map->getCols();;
    
    /// Determine which tile to check based on direction and boudary conditions
    if(d==Direction::Right && col<map->getCols()-1)
        tileToBeChecked = currentTile+1;
    else if(d==Direction::Left && col>0)
        tileToBeChecked = currentTile-1;
    else if(d==Direction::Down && row<map->getRows()-1)
        tileToBeChecked = currentTile + map->getCols();
    else if(d==Direction::Up && row > 0)
        tileToBeChecked = currentTile - map->getCols();
    else 
        tileToBeChecked = -1;
    
    /// If a valid tile was found, check its type
    if (tileToBeChecked >= 0) {
        std::vector<Tile> tiles = map->getTiles();
        detectionChecked = (tiles[tileToBeChecked].getType() == TileType::Obstacle);
    } else {
        detectionChecked = false;
    }
}

/**
 * Resets the detection state to false
 */
void Sensor::disable(){
    detectionChecked = false; 
    return; 
}