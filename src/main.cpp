#include <cstddef>
#include <cstdlib>
#include <iostream>
#include <ncurses.h>
#include "window.hpp"
#include "dijkstra.hpp"
#include "graph.hpp"
#include "board.hpp"

int main()
{
    Board board(BOARD_SIZE);
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