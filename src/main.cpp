#include <cstddef>
#include <cstdlib>
#include <iostream>
#include <ncurses.h>
#include <cstring>
#include "window.hpp"
#include "dijkstra.hpp"
#include "graph.hpp"
#include "board.hpp"
#include "strategy.hpp"

HumanPlayers readArguments(int argc, char *argv[])
{
    HumanPlayers humanPlayers = {false, false};

    for (int i = 0; i < argc; i++) {
        if (strcmp(argv[i], "--blue") == 0)
            humanPlayers.blue = true;

        if (strcmp(argv[i], "--red") == 0)
            humanPlayers.red = true;
    }

    if (! humanPlayers.blue && ! humanPlayers.red)
        humanPlayers.blue = true;

    return humanPlayers;
}

int main(int argc, char *argv[])
{
    HumanPlayers humanPlayers = readArguments(argc, argv);

    Board board(BOARD_SIZE, humanPlayers);
    
    if (!humanPlayers.blue) {
        board.setStrategy(Turn::Blue, std::make_unique<AIStrategy>(Turn::Blue));
    }

    if (!humanPlayers.red) {
        board.setStrategy(Turn::Red, std::make_unique<AIStrategy>(Turn::Red));
    }
    
    Window window(board);
    window.initialize();

    int row = 0;
    int col = 0;

    do {
        window.render(row, col);

        if (window.getKey(row, col) == 'q')
            break;
    } while(true);
}