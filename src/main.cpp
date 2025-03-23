#include <cstddef>
#include <cstdlib>
#include <iostream>
#include <ncurses.h>
#include "graph.hpp"
#include "board.hpp"

constexpr int BOARD_SIZE = 7;
constexpr int BOARD_START_ROW = 3;
constexpr int BOARD_START_COL = 3;
constexpr int KEY_ESCAPE = 27;

static int allocations = 0;

void* operator new(size_t size)
{
    allocations++;
    return malloc(size);
}

void renderBoard(WINDOW* win, Board& board)
{
    std::string render = board.toString();
    std::istringstream renderStream(render);

    int row = BOARD_START_ROW;
    for (std::string line; std::getline(renderStream, line);) {
        mvwprintw(win, row, BOARD_START_COL, "%s", line.c_str());
        row++;
    }
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
        refresh();

        box(win, 0, 0);
        mvwprintw(win, 0, 2, "Hex");
        mvwprintw(win, 1, 2, "Turn %s / Movements %d",
            turnAsLabel(board.current()).c_str(),
            board.countMovements()
        );

        renderBoard(win, board);
        mvwprintw(win, 1, 40, "Row %d, Col %d", row, col);

        // Print the cursor
        move(BOARD_START_ROW + row*2, BOARD_START_COL + col*4 + row*2);

        wrefresh(win);

        // Manage key strokes
        key = getch();

        if (key == KEY_ESCAPE) {
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