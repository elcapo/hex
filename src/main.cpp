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

    int key, row, col;

    row = 0;
    col = 0;

    Window window(board);
    window.initialize();

    do {
        window.render(row, col);

        key = getch();

        if (key == 'q') {
            break;
        } else if (key == KEY_LEFT) {
            if (col > 0)
                col--;
        } else if (key == KEY_RIGHT) {
            if (col < BOARD_SIZE - 1)
                col++;
        } else if (key == KEY_UP) {
            if (row > 0)
                row--;
        } else if (key == KEY_DOWN) {
            if (row < BOARD_SIZE - 1)
                row++;
        } else if (key == ' ') {
            if (board.get(row, col) == Turn::Undecided)
                board.set(row, col);
        } else if (key == 'p') {
            if (board.countMovements() == 1)
                board.pieRule();
        }
    } while(true);
}