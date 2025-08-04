#include "Map.h"
#include <queue>
/**
 * Main constructor. Initializes the grid based on the window dimensions.
 * Each tile is created as an "Empty" (white) tile.
 * @param windowsWidth Width of the window in pixels.
 * @param windowHeight Height of the window in pixels
 */
Map::Map(int const windowsWidth, int const windowHeight){
    cols = windowsWidth / tilesSize;
    rows = windowHeight / tilesSize;

    /// Create the tile grid (flattened into a 1D vector)
    for(int r = 0; r < rows; r++){
        for(int c = 0; c < cols; c++){
            /// Generete a rect in the right position 
            Tile t(tilesSize-1, tilesSize-1, tilesSize); ///-1 to visually separate tiles
            t.setFillColor(sf::Color::White);
            t.setPosition(c*tilesSize, r*tilesSize);
            tiles.push_back(t);
        }
    }
}

/**
 * Clones the map structure without obstacles. All tiles are set to Empty.
 * Useful for algorithmic purposes (e.g., simulated pathfinding) without modifying the real map.
 * 
 * @return A new Map object with the same layout but no obstacles.
 */
Map Map::cloneStructureWithoutObstacles() const {
    Map clone;
    clone.tilesSize = this->tilesSize;
    clone.rows = this->rows;
    clone.cols = this->cols;

    for (int r = 0; r < rows; r++) {
        for (int c = 0; c < cols; c++) {
            Tile original = tiles[r * cols + c];
            Tile t(tilesSize - 1, tilesSize - 1, tilesSize);
            t.setPosition(c * tilesSize, r * tilesSize);
            t.setType(TileType::Empty); /// Remove any obstacle
            clone.tiles.push_back(t);
        }
    }

    return clone;
}



std::vector<Tile>& Map::getTiles()  {
    return tiles;
}

/**
 * Builds the adjacency list graph for pathfinding. 
 * Only non-obstacles tiles are connected to their valid neighbors. 
 */
void Map::buildGraph(){
    int totalNodes = rows*cols; 
    graph.clear();
    graph.resize(totalNodes); 

    for(int r=0; r<rows; r++){
        for(int c=0; c<cols; c++){
            int nodeID= r*cols + c; 

            /// Skip if the current tile is an obstacle
            if(tiles[nodeID].getType() == TileType::Obstacle)
                continue;

            /// Check adjacent nodes (up, down, left, right)
            if (r > 0){
                int upNodeID = (r-1)*cols+c; 
                if(tiles[upNodeID].getType() != TileType::Obstacle)
                    graph[nodeID].emplace_back(upNodeID, 1.0f);
            }
            if (r < rows - 1){
                int downNodeID = (r + 1) * cols + c;
                if(tiles[downNodeID].getType() != TileType::Obstacle)
                    graph[nodeID].emplace_back(downNodeID, 1.0f);
            }
            if(c>0){
                int leftNodeID = r * cols + (c - 1);
                if(tiles[leftNodeID].getType() != TileType::Obstacle)
                    graph[nodeID].emplace_back(leftNodeID, 1.0f);
            }
            if (c < cols - 1) {
                int rightNodeID = r * cols + (c + 1);
                if(tiles[rightNodeID].getType() != TileType::Obstacle)
                    graph[nodeID].emplace_back(rightNodeID, 1.0f);
            }
        }
    }   
}

const std::vector<std::vector<std::pair<int,float>>>& Map::getGraph() const{
    return graph; 
}

/**
 * Prints the adjacency list of the graph to stdout.
 */
void Map::printGraph() const {
    for (int i = 0; i < graph.size(); ++i) {
        std::cout << "Nodo " << i << " -> ";
        for (const auto& [neighbor, weight] : graph[i]) {
            std::cout << "(" << neighbor << ", peso=" << weight << ") ";
        }
        std::cout << "\n";
    }
}

/**
 * Draws all the tiles on the provided SFML window.
 * @param window The SFML window to render to
 */
void Map::draw(sf::RenderWindow& window) const {
    for (const auto& tile : tiles) {
        tile.draw(window);  /// Il metodo draw di Tile
    }
}


/**
 * Dijkstra's algorithm for shortest path between two nodes.
 * @param start ID of the start tile.
 * @param goal ID of the goal tile.
 * @return A vector of tile indices representing the shortest path.
 */
std::vector<int> Map::dijkstra(int start, int goal){
    int totalNodes = graph.size();
    /// Vector of distances 
    std::vector<float> distance (totalNodes, std::numeric_limits<float>::infinity());

    /// Vector of previous
    std::vector<int> previous(totalNodes, -1);

    distance[start]=0.0f;

    /// Priority queue 
    std::priority_queue<std::pair<float, int>,
                    std::vector<std::pair<float, int>>,
                    std::greater<std::pair<float, int>>> pq;
    pq.emplace(0.0f, start);

    /// Algorithm: 
    while(!pq.empty()){
        /// Get the element with the shortest distance: 
        auto [dist, u] = pq.top(); 
        pq.pop();

        /// Get the neighbor of u and check the distances
        for(auto [v, weight] : graph[u]){
            if(distance[u] + weight < distance[v]){
                distance[v] = distance[u]+weight; 
                previous[v] = u; 
                pq.emplace(distance[v], v);
            }
        }
    }

    /// Reconstruct path from goal to start
    std::vector<int> path; 
    for(int at = goal; at!=-1; at = previous[at]){
        path.push_back(at);
    }
    std::reverse(path.begin(), path.end());

    return path; 
}

/**
 *  Changes the border color of a specific tile.
 * @param c The new outline color.
 * @param id The tile index.
 */
void Map::setBorderColorTile(sf::Color c, int id){
    tiles[id].setBorderColor(c);
}

/**
 * Resets the path tiles (excluding start/goal/obstacles) to default white color.
 * 
 * @param p The path as a list of tile indices.
 */
void Map::defaultColorTile(std::vector<int> p){
    for(int i=0; i<p.size(); i++){
        if(i==0 || i==p.size()-1 || tiles[p[i]].getType()==TileType::Obstacle)
            continue;
        tiles[p[i]].setFillColor(sf::Color::White);
    }
}

/**
 * Visually highlights the path on the map.
 * 
 * @param p The path as a list of tile indices.
 */
void Map::setColorPath(std::vector<int> p){
    for(int i=0; i<p.size(); i++){
        if(i==0 || i==p.size()-1 || tiles[p[i]].getType()==TileType::Obstacle)
            continue;
        tiles[p[i]].setFillColor(sf::Color(255, 0, 0, 150));  /// semi-transparent red

    }
}
