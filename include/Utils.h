#include "Tile.h"
/**
 * Converts a TileType enum value to its string representation.
 *
 * @param type The TileType to convert.
 * @return std::string Human-readable string describing the tile type.
 */
std::string tileTypeToString(TileType type) {
    switch(type) {
        case TileType::Empty: return "Empty";
        case TileType::Obstacle: return "Obstacle";
        case TileType::Start: return "Start";
        case TileType::Goal: return "Goal";
        default: return "Unknown";
    }
}
