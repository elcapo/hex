#ifndef COMMON_H
#define COMMON_H

#include <utility>
#include <string>

/**
 * The `Turn` enum contains the possible colors for each player.
 */
enum Turn { Undecided = 0, Blue = 1, Red = 2 };

/**
 * The `Position` type is used to store a reference to a cell.
 */
typedef std::pair<int, int> Position;

/**
 * The `HumanPlayers` struct indicates what players are human.
 */
struct HumanPlayers
{
    bool blue;
    bool red;
};

/**
 * Get a given turn as a single character.
 *
 * @return 'B' for Turn::Blue
 *         'R' for Turn::Red
 *         '·' otherwise
 */
const char* turnAsChar(const Turn& t);

/**
 * Get a given turn as a printable label.
 *
 * @return 'Blue' for Turn::Blue
 *         'Red' for Turn::Red
 *         'Undecided' for Turn::Undecided
 */
std::string turnAsLabel(const Turn& t);

#endif // COMMON_H 