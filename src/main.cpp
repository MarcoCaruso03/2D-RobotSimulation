#include <SFML/Graphics.hpp>
#include <cmath>
#include "Map.h"
#include "Robot.h"
#include "Utils.h"
/**
 * Entry point of the simulation. Initializes windows, map, and robot, and handles user interaction.
 * 
 * Users can define a start point, goal point, and obstacles on a grid map using mouse clicks.
 * Two windows are displayed:
 *  - One for the actual map
 *  - One for the robot's internal map
 * 
 *  The robot navigates from start to goal using a pathfinding algorithm (e.g., Dijkstra).
 */
int main()
{
    std::cout<<"Hello\n";

    /// ----------------------- Window setup -----------------------
    int windowsWidth = 800; 
    int windowsHeigt = 600;
    sf::RenderWindow window1(sf::VideoMode(windowsWidth, windowsHeigt), "True Map");
    sf::RenderWindow window2(sf::VideoMode(windowsWidth, windowsHeigt), "Robot Map");
    sf::Color colorBackGround = sf::Color::Black;

    /// ------------------------ Font loading ------------------------
    sf::Font font;
    if (!font.loadFromFile("/usr/share/fonts/truetype/dejavu/DejaVuSans.ttf")){
        /// gestisci errore caricamento font
        return -1;
    }

    /// --------------------- Application state ----------------------
    bool generateMap = false;
    int clickStage = 0; /// 0 = start, 1 = goal, 2+ = obstacles
    int startTile = 0;
    int goalTile = -1;


    /// ------------------------ Map & Robot -------------------------
    Map map(windowsWidth, windowsHeigt);
    auto& tiles = map.getTiles();

    Robot robot(&map, 0,0,25, &window1);
    Map * rMap = robot.getRobotMap();

    int xR, yR, colR, rowR;
    bool robotPlaced = false; 
    bool canMove=false; 
    bool canRunAlgo = false; 

    
    /// -------------------- Main render loop ------------------------
    while (window1.isOpen() || window2.isOpen())
    {
        sf::Event event;
        /// =========== Handle events in window1 ===========
        while (window1.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
                window1.close();
            sf::Vector2i mousePos = sf::Mouse::getPosition(window1);

            /// -------- Left mouse click: select tiles --------
            if (event.type == sf::Event::MouseButtonPressed && event.mouseButton.button == sf::Mouse::Left){
                int tileSize = map.getTileSize();
                int col = mousePos.x / tileSize;
                int row = mousePos.y / tileSize;
                if (col >= 0 && col < map.getCols() && row >= 0 && row < map.getRows()) {
                    int index = row*map.getCols()+col;
                    /// Starting point 
                    if(clickStage == 0){
                        tiles[index].setFillColor(sf::Color::Magenta);
                        std::cout << "Start set on tile: " << index << "\n";
                        startTile = index;
                        rowR = startTile / map.getCols();
                        colR = startTile % map.getCols();
                        robot.placeRobot(colR, rowR);
                        robot.setStartTile(startTile);
                    }
                    /// Goal point
                    else if(clickStage == 1){
                        tiles[index].setFillColor(sf::Color::Green);
                        std::cout << "Goal set on tile: " << index << "\n";
                        goalTile = index;
                        robot.setEndTile(goalTile);
                    }
                    /// Obstacle
                    else{
                        if(tiles[index].getType() != TileType::Obstacle){
                            tiles[index].setType(TileType::Obstacle);
                            tiles[index].setFillColor(sf::Color::Black);
                            std::cout << "Tile " << index << " set as obstacle\n";
                        }
                    }
                    clickStage++;
                }
            }

            /// -------- Right click: remove obstacle --------
            if(sf::Mouse::isButtonPressed(sf::Mouse::Right)){
                int tileSize = map.getTileSize();
                int col = mousePos.x / tileSize;
                int row = mousePos.y / tileSize;
                if(col >= 0 && col<map.getCols() && row >= 0 && row < map.getRows()){
                    int index = row*map.getCols()+col; 
                    if(tiles[index].getType() == TileType::Obstacle){
                        tiles[index].setType(TileType::Empty);
                        tiles[index].setFillColor(sf::Color::White);
                        std::cout << "Tile " << index << " is not an obstacle anymore\n";
                    }
                }
            }
            /// -------- Key press: 'E' triggers robot pathfinding --------
            if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::E && clickStage>=1)
                robot.setCanRunAlgo(true);

        }

        /// =========== Handle events in window2 ===========
        while(window2.pollEvent(event)){
            if(event.type==sf::Event::Closed)
                window2.close();
        }

        /// =========== Rendering ===========

        window1.clear(colorBackGround);
        window2.clear(colorBackGround);


        /// Draw full map in window1      
        map.draw(window1);

        /// Update and draw robot
        robot.update(window1);
       
        /// ----- Debugging: draw tile indices -----
        for (int i = 0; i < tiles.size(); ++i) {
            sf::Text text;
            text.setFont(font);
            text.setString(std::to_string(i));
            text.setCharacterSize(12);
            text.setFillColor(sf::Color::Red);

            sf::FloatRect tileBounds = tiles[i].getGlobalBounds();
            sf::FloatRect textBounds = text.getLocalBounds();

            float x = tileBounds.left + (tileBounds.width - textBounds.width) / 2.f;
            float y = tileBounds.top + (tileBounds.height - textBounds.height) / 2.f;

            text.setPosition(x, y);
            window1.draw(text);  
        }

        /// ----- Draw robot's internal map (rMap) -----
        robot.getRobotMap()->draw(window2);

        const std::vector<Tile>& robotTiles = robot.getRobotMap()->getTiles();

        for (int i = 0; i < robotTiles.size(); ++i) {
            sf::Text text;
            text.setFont(font);
            text.setString(std::to_string(i));
            text.setCharacterSize(12);
            text.setFillColor(sf::Color::Red);

            sf::FloatRect tileBounds = robotTiles[i].getGlobalBounds();
            sf::FloatRect textBounds = text.getLocalBounds();

            float x = tileBounds.left + (tileBounds.width - textBounds.width) / 2.f;
            float y = tileBounds.top + (tileBounds.height - textBounds.height) / 2.f;

            text.setPosition(x, y);
            window2.draw(text);
        }


        window2.display();
        window1.display();
    }

    return 0;
}
