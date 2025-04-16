#include "common.hpp"
#include <string>

const char* turnAsChar(const Turn& t)
{
    switch (t) {
        case Blue:
            return "B";
        case Red:
            return "R";
        default:
            return "o";
    }
}

std::string turnAsLabel(const Turn& t)
{
    switch (t) {
        case Blue:
            return "Blue plays";
        case Red:
            return "Red plays";
        default:
            return "Game finished";
    }
} 