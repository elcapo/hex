#include <cstddef>
#include <cstdlib>
#include <iostream>
#include <ncurses.h>
#include "graph.hpp"
#include "board.hpp"

static int allocations = 0;

void* operator new(size_t size)
{
    allocations++;
    return malloc(size);
}

int main()
{
    Board board(7);
    const Graph& graph = board.getGraph();

    initscr();

    board.set(1, 1);
    board.pieRule();
    board.set(2, 1);
    board.set(2, 2);

    WINDOW* win = newwin(0, 0, 0, 0);
    refresh();

    box(win, 0, 0);

    mvwprintw(win, 0, 2, "Hex");

    mvwprintw(win, 1, 2, "Nodes: %d / Edges %d / Turn %c / Movements %d",
        graph.countNodes(),
        graph.countEdges(),
        turnAsChar(board.current()),
        board.countMovements()
    );

    std::string render = board.toString();
    std::istringstream renderStream(render);

    int row = 3;
    for (std::string line; std::getline(renderStream, line);) {
        mvwprintw(win, row, 2, "%s", line.c_str());
        row++;
    }

    move(3, 2);

    wrefresh(win);

    getch();
    endwin();
}