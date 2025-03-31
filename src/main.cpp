#include <cstddef>
#include <cstdlib>
#include <iostream>
#include <ncurses.h>
#include "window.hpp"
#include "graph.hpp"
#include "board.hpp"

int main()
{
    Board board(BOARD_SIZE);

    int row, col;

    row = 0;
    col = 0;

    Window window(board);
    window.initialize();

    do {
        window.render(row, col);

        if (window.getKey(row, col) == 'q')
            break;
    } while(true);
}