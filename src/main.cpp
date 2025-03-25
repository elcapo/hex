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
constexpr int BOARD_START_ROW = 4;
constexpr int BOARD_START_COL = 5;

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

/**
 * Render the board in the given window starting at the
 * position given by BOARD_START_ROW and BOARD_START_COL.
 */
void renderBoard(WINDOW* win, Board& board)
{
    int row = BOARD_START_ROW;

    board.forEachLine([win, &row](const char* line) {
        mvwprintw(win, row, BOARD_START_COL, "%s", line);
        row++;
    });
}

/**
 * Get the row number of the screen that corresponds
 * to a given row and column of the board.
 *
 * @return Row number.
 */
int getY(int row, int col)
{
    return BOARD_START_ROW + row*2;
}

/**
 * Get the column number of the screen that corresponds
 * to a given row and column of the board.
 *
 * @return Column number.
 */
int getX(int row, int col)
{
    return BOARD_START_COL + col*4 + row*2;
}

/**
 * Print the title of the window.
 */
void printTitle(WINDOW* win)
{
    mvwprintw(win, 0, 2, " The Zig-Zag Game ");
}

/**
 * Print the header of the window.
 */
void printHeader(WINDOW* win, Board& board, int& row, int&col)
{
    mvwprintw(win, 1, 2,
        "Board (%dx%d) / Turn %4s / Movements %d / Row %2d, Col %2d / Memory allocations %2d",
        BOARD_SIZE,
        BOARD_SIZE,
        turnAsLabel(board.current()).c_str(),
        board.countMovements(),
        row + 1,
        col + 1,
        allocations
    );
}

/**
 * Print the footer of the window.
 */
void printFooter(WINDOW* win, Board& board)
{
    if (board.countMovements() == 1) {
        mvwprintw(win, LINES-5, 2, "Press `p` to apply the pie rule.");
    } else {
        wmove(win, LINES-5, 0);
        wclrtoeol(win);
    }

    mvwprintw(win, LINES-4, 2, "Press the `arrows` to move the cursor.");
    mvwprintw(win, LINES-3, 2, "Press `space` to make a movement.");
    mvwprintw(win, LINES-2, 2, "Press `q` to quit.");    
}

/**
 * Render the marks that indicate which side corresponds to
 * each player.
 */
void renderColorMarkers(WINDOW* win)
{
    mvwprintw(win,
        getY(0, 0) - 1,
        getX(0, BOARD_SIZE/2),
        "R"
    );

    mvwprintw(win,
        getY(BOARD_SIZE - 1, BOARD_SIZE/2) + 1,
        getX(BOARD_SIZE - 1, BOARD_SIZE/2),
        "R"
    );

    mvwprintw(win,
        getY(BOARD_SIZE/2, 0),
        getX(BOARD_SIZE/2, 0) - 2,
        "B"
    );

    mvwprintw(win,
        getY(BOARD_SIZE/2, 0),
        getX(BOARD_SIZE/2, BOARD_SIZE - 1) + 2,
        "B"
    );
}

/**
 * Initialize the screen.
 */
void initialize()
{
    initscr();
    cbreak();
    noecho();
    keypad(stdscr, true);
}

/**
 * Render the game window.
 */
void render(WINDOW* win, Board& board, int& row, int&col)
{
    refresh();
    printHeader(win, board, row, col);
    renderBoard(win, board);
    renderColorMarkers(win);
    printFooter(win, board);
    box(win, 0, 0);
    printTitle(win);
    move(getY(row, col), getX(row, col));
    wrefresh(win);
}

int main()
{
    Board board(BOARD_SIZE);
    const Graph& graph = board.getGraph();

    int key, row, col, height, width;

    row = 0;
    col = 0;

    initialize();

    WINDOW* win = newwin(0, 0, 0, 0);

    do {
        height = LINES;
        width = COLS;

        render(win, board, row, col);

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

    delwin(win);
    endwin();
}