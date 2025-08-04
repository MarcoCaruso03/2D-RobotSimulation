#ifndef DIRECTION_H
#define DIRECTION_H

/**
 * Enumeration representing possible directions for movement or orientation.
 */
enum class Direction {
    Up, /// Move or face upward
    Down, /// Move or face downward
    Left, /// Move or face left
    Right, /// Move or face right
    None /// No direction or stationary
};

/**
 * Converts a Direction enum value to its corresponding string representation.
 * 
 * @param d The direction to convert.
 * @return const char* String name of the direction.
 */
inline const char* directionToString(Direction d) {
    switch(d) {
        case Direction::Up: return "Up";
        case Direction::Down: return "Down";
        case Direction::Left: return "Left";
        case Direction::Right: return "Right";
        case Direction::None: return "None";
        default: return "Unknown";
    }
}

#endif
