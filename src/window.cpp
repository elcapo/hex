#include "window.hpp"

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
 * Activate the blue color.
 */
void startBlue(WINDOW* win)
{
    wattron(win, COLOR_PAIR(BLUE_PAIR));
}

/**
 * Deactivate the blue color.
 */
void endBlue(WINDOW* win)
{
    wattroff(win, COLOR_PAIR(BLUE_PAIR));
}

/**
 * Activate the red color.
 */
void startRed(WINDOW* win)
{
    wattron(win, COLOR_PAIR(RED_PAIR));
}

/**
 * Deactivate the blue color.
 */
void endRed(WINDOW* win)
{
    wattroff(win, COLOR_PAIR(RED_PAIR));
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
void renderColorMarkers(WINDOW* win, Board& board)
{
    startBlue(win);

    mvwprintw(win,
        BOARD_START_ROW + board.getY(BOARD_SIZE/2, 0),
        BOARD_START_COL + board.getX(BOARD_SIZE/2, 0) - 2,
        "B"
    );

    mvwprintw(win,
        BOARD_START_ROW + board.getY(BOARD_SIZE/2, 0),
        BOARD_START_COL + board.getX(BOARD_SIZE/2, BOARD_SIZE - 1) + 2,
        "B"
    );

    endBlue(win);

    startRed(win);

    mvwprintw(win,
        BOARD_START_ROW + board.getY(0, 0) - 1,
        BOARD_START_COL + board.getX(0, BOARD_SIZE/2),
        "R"
    );

    mvwprintw(win,
        BOARD_START_ROW + board.getY(BOARD_SIZE - 1, BOARD_SIZE/2) + 1,
        BOARD_START_COL + board.getX(BOARD_SIZE - 1, BOARD_SIZE/2),
        "R"
    );

    endRed(win);
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
 * Render the pieces of the board in the given window starting at the
 * position given by BOARD_START_ROW and BOARD_START_COL.
 */
void renderPieces(WINDOW* win, Board& board)
{
    board.forEachPiece([&win, &board](const int row, const int col, Turn turn) {
        if (turn == Turn::Blue) {
            startBlue(win);
            mvwprintw(win,
                BOARD_START_ROW + board.getY(row, col),
                BOARD_START_COL + board.getX(row, col),
                "B"
            );
            endBlue(win);
        }

        if (turn == Turn::Red) {
            startRed(win);
            mvwprintw(win,
                BOARD_START_ROW + board.getY(row, col),
                BOARD_START_COL + board.getX(row, col),
                "R"
            );
            endRed(win);
        }
    });
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

    if (has_colors()) {
        start_color();

        use_default_colors();

        init_pair(BASE_PAIR, COLOR_BLACK, COLOR_WHITE);
        init_pair(BLUE_PAIR, COLOR_BLACK, COLOR_CYAN);
        init_pair(RED_PAIR, COLOR_BLACK, COLOR_RED);

        bkgd(COLOR_PAIR(BASE_PAIR));
        clear();
    }
}

/**
 * Render the game window.
 */
void render(WINDOW* win, Board& board, int& row, int&col)
{
    refresh();

    printHeader(win, board, row, col);
    renderBoard(win, board);
    renderPieces(win, board);
    renderColorMarkers(win, board);
    printFooter(win, board);
    box(win, 0, 0);
    printTitle(win);
    move(
        BOARD_START_ROW + board.getY(row, col),
        BOARD_START_COL + board.getX(row, col)
    );

    wrefresh(win);
}