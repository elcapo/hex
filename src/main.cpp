#include <cstddef>
#include <cstdlib>
#include <iostream>
#include <ncurses.h>
#include "graph.hpp"
#include "board.hpp"

/**
 * The `BOARD_SIZE` constant represents the size of the board.
 *
 * @example If set to 11, the board will have 11 rows and 11 columns.
 */
constexpr int BOARD_SIZE = 11;

/**
 * Row and column of the window where the starting (top left)
 * part of the board will be anchored. This is not meant to be
 * a configurable parameter.
 */
constexpr int BOARD_START_ROW = 3;
constexpr int BOARD_START_COL = 3;

/**
 * We use `allocations` to track the number of allocations
 * made while building the program.
 */
static int allocations = 0;

void* operator new(size_t size)
{
    allocations++;
    return malloc(size);
}

void renderBoard(WINDOW* win, Board& board)
{
    int row = BOARD_START_ROW;

    board.forEachLine([win, &row](const char* line) {
        mvwprintw(win, row, BOARD_START_COL, "%s", line);
        row++;
    });
}

int main()
{
    Board board(BOARD_SIZE);
    const Graph& graph = board.getGraph();

    initscr();
    cbreak();
    noecho();
    keypad(stdscr, true);

    bool exit = false;
    int key;
    int row = 0;
    int col = 0;

    WINDOW* win = newwin(0, 0, 0, 0);

    do {
        int height = LINES;
        int width = COLS;

        // Print the window
        refresh();

        box(win, 0, 0);
        mvwprintw(win, 0, 2, "Hex");

        mvwprintw(win, 1, 2, "Turn %s / Movements %d / Row %2d, Col %2d",
            turnAsLabel(board.current()).c_str(),
            board.countMovements(),
            row + 1,
            col + 1
        );

        mvwprintw(win, LINES-3, 2, "Allocations %d", allocations);
        mvwprintw(win, LINES-2, 2, "Press `q` to quit. Use the `arrows` to move the cursor. Use `space` to make a movement.");

        renderBoard(win, board);

        // Print the cursor
        move(BOARD_START_ROW + row*2, BOARD_START_COL + col*4 + row*2);

        wrefresh(win);

        // Manage key strokes
        key = getch();

        if (key == 'q') {
            exit = true;
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
        }    
    } while(! exit);

    endwin();
}