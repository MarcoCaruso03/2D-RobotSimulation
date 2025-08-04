#include "Robot.h"
#include <cmath>

/**
 * Robot constructor 
 * 
 * Initializes sensors, shape, position, and robot's internal map.
 */
Robot::Robot(Map* m, int initX, int initY, int initR, sf::RenderWindow* w)
    : map(m)
{
    //// Initialize sensors for each direction
    sensors.emplace_back(Direction::Up, this, m);
    sensors.emplace_back(Direction::Down, this, m);
    sensors.emplace_back(Direction::Left, this, m);
    sensors.emplace_back(Direction::Right, this, m);

    //// Default detection status: false
    detected = std::vector<bool>(4, false);

    x = initX; 
    y = initY;
    r = initR;

    //// Create an internal map copy (no obstacles)
    robotMap = map->cloneStructureWithoutObstacles();

    shape = sf::CircleShape(r);
    shape.setFillColor(sf::Color::Red);
    shape.setPosition(sf::Vector2f(x, y));

    int tileSize = map->getTileSize();
    int robot_col = x / tileSize; 
    int robot_row = y / tileSize; 
    currentTile = robot_row * map->getCols() + robot_col; 
    std::cout << "CurrentRobotTile: " << currentTile << "\n";

    win = w;
}

void Robot::setX(int new_x){
    sf::Vector2f pos = shape.getPosition();
    shape.setPosition(new_x, pos.y);
}

void Robot::setY(int new_y){
    sf::Vector2f pos = shape.getPosition();
    shape.setPosition(pos.x, new_y);
}

/**
 * Main logic loop for robot movement, sensing, and path following.
 */
void Robot::update(sf::RenderWindow& window){
    if(robotPlaced){
        if(canRunAlgo){
            //// Reset sensor detection flags
            detected = std::vector<bool>(4, false);
            
            //// True if it is the first time or when we find an obstacle
            if(needToComputePath){
                std::cout<<"Recomputing path...\n";

                map->defaultColorTile(pathToFollow); //// Reset color of previous path
                robotMap.buildGraph(); //// Build graph based on updated internal map
                pathToFollow.clear();
                pathToFollow=robotMap.dijkstra(currentTile, endTile);
                needToComputePath = false;
                
                //// For debugging: print of the path
                for(auto p : pathToFollow)
                    std::cout<<p<<"->"; 
                
                currentStep = 0;
                map->setColorPath(pathToFollow); //// Visually mark new path
                if(pathToFollow.size()==1){
                    std::cout<<"\nNo valid path to follow.\n";
                    canRunAlgo = false; 
                    return; 
                }
            }

            if (currentStep >= pathToFollow.size()) {
                window.draw(shape);
                canRunAlgo = false; 
                std::cout<<"\nEnd tile reached\n";
                return;  
            }

            int nextTile = pathToFollow[currentStep];
            int tileSize = map->getTileSize();
            int col = nextTile % map->getCols();
            int row = nextTile / map->getCols();


            float radius = shape.getRadius();
            sf::Vector2f targetPosition = {
                col * tileSize + tileSize / 2.0f - radius,
                row * tileSize + tileSize / 2.0f - radius
            };

            sf::Vector2f currentPosition = shape.getPosition();
            float dx = targetPosition.x - currentPosition.x;
            float dy = targetPosition.y - currentPosition.y;
            float distance = std::sqrt(dx * dx + dy * dy);

            if (distance < 1.0f) {
                //// Reached tile
                currentTile = nextTile;
                currentStep++;
            } else {
                //// Simulate obstacle detection
                for(int i=0; i<sensors.size(); i++){
                    sensors[i].active();
                    if(sensors[i].getdetectionChecked()){
                        detected[i]=true;
                        sensors[i].disable();
                    }
                }
                ////take the nextDirection and check the corrispoding sensors
                Direction d = getNewDirection(nextTile);
                
                //// Obstacle detection per direction
                if(directionToString(d)=="Up" && detected[0]==true){
                    nextMoveIsValide = false;
                    robotMap.getTiles()[nextTile].setType(TileType::Obstacle);
                    std::cout << "Obstacle detected above.\n";
                    needToComputePath = true; 
                }
                if(directionToString(d)=="Down" && detected[1]==true){
                    nextMoveIsValide = false;
                    robotMap.getTiles()[nextTile].setType(TileType::Obstacle);
                    std::cout << "Obstacle detected below.\n";
                    needToComputePath = true; 
                }
                if(directionToString(d)=="Left" && detected[2]==true){
                    nextMoveIsValide = false;
                    robotMap.getTiles()[nextTile].setType(TileType::Obstacle);
                    std::cout << "Obstacle detected on the left.\n";
                    needToComputePath = true; 
                }
                if(directionToString(d)=="Right" && detected[3]==true){
                    nextMoveIsValide = false;
                    robotMap.getTiles()[nextTile].setType(TileType::Obstacle);
                    std::cout << "Obstacle detected on the right.\n";
                    needToComputePath = true; 
                }
                if(needToComputePath)
                    return;
                
                //// No obstacle: proceed in the detected direction
                if (d == Direction::Right) moveXpos();
                else if (d == Direction::Left) moveXneg();
                else if (d == Direction::Up) moveYneg();
                else if (d == Direction::Down) moveYpos();
            }

        }
        window.draw(shape);
    }
}

//// Movement helpers
void Robot::moveXpos(){
    shape.move(speed, 0);
}
void Robot::moveXneg(){
    shape.move(-speed, 0);
}
void Robot::moveYpos(){
    shape.move(0, speed);
}
void Robot::moveYneg(){
    shape.move(0, -speed);
}

/**
 * Computes direction from current tile to a target tile.
 */
Direction Robot::getNewDirection(int tile){
    int currentRow = currentTile / map->getCols();
    int currentCol = currentTile % map->getCols();

    int targetRow = tile / map->getCols();
    int targetCol = tile % map->getCols();

    if (targetRow < currentRow) return Direction::Up;
    if (targetRow > currentRow) return Direction::Down;
    if (targetCol < currentCol) return Direction::Left;
    if (targetCol > currentCol) return Direction::Right;
    return Direction::None;

}
/**
 * Places robot at a specified tile position (column, row).
 */
void Robot::placeRobot(int c, int r){
    std::cout<<"Robot placed\n";
    x = c*map->getTileSize()+map->getTileSize()/2;
    x-=25;
    y = r * map->getTileSize()+map->getTileSize()/2;
    y-=25;
    robotPlaced = true; 
    setX(x);
    setY(y);
}

/**
 * Updates internal path and highlights it visually on the map.
 */
void Robot::setPath(const std::vector<int>& newPath){
    pathToFollow = newPath;
    for(int i=0; i<pathToFollow.size(); i++){
        map->setBorderColorTile(sf::Color::Cyan, pathToFollow[i]);
    }
}

