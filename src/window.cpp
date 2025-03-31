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
void Window::startBlue()
{
    wattron(win, COLOR_PAIR(BLUE_PAIR));
}

/**
 * Deactivate the blue color.
 */
void Window::endBlue()
{
    wattroff(win, COLOR_PAIR(BLUE_PAIR));
}

/**
 * Activate the red color.
 */
void Window::startRed()
{
    wattron(win, COLOR_PAIR(RED_PAIR));
}

/**
 * Deactivate the blue color.
 */
void Window::endRed()
{
    wattroff(win, COLOR_PAIR(RED_PAIR));
}

/**
 * Print the title of the window.
 */
void Window::printTitle()
{
    mvwprintw(win, 0, 2, " The Zig-Zag Game ");
}

/**
 * Print the header of the window.
 */
void Window::printHeader(int& row, int&col)
{
    mvwprintw(win, 1, 2,
        "Board (%dx%d) / %s / Movements %d / Row %2d, Col %2d / Memory allocations %2d",
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
void Window::printFooter()
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
void Window::renderColorMarkers()
{
    startBlue();

    for (int i = 0; i < BOARD_SIZE; i++) {
        mvwprintw(win,
            BOARD_START_ROW + board.getY(i, 0),
            BOARD_START_COL + board.getX(i, 0) - 2,
            "B"
        );

        mvwprintw(win,
            BOARD_START_ROW + board.getY(i, 0),
            BOARD_START_COL + board.getX(i, BOARD_SIZE - 1) + 2,
            "B"
        );
    }

    endBlue();

    startRed();

    for (int i = 0; i < BOARD_SIZE; i++) {
        mvwprintw(win,
            BOARD_START_ROW + board.getY(0, i) - 1,
            BOARD_START_COL + board.getX(0, i),
            "R"
        );

        mvwprintw(win,
            BOARD_START_ROW + board.getY(BOARD_SIZE - 1, i) + 1,
            BOARD_START_COL + board.getX(BOARD_SIZE - 1, i),
            "R"
        );
    }

    endRed();
}

/**
 * Render the board in the given window starting at the
 * position given by BOARD_START_ROW and BOARD_START_COL.
 */
void Window::renderBoard()
{
    int row = BOARD_START_ROW;

    board.forEachLine([this, &row](const char* line) {
        mvwprintw(this->win, row, BOARD_START_COL, "%s", line);
        row++;
    });
}

/**
 * Render the pieces of the board in the given window starting at the
 * position given by BOARD_START_ROW and BOARD_START_COL.
 */
void Window::renderPieces()
{
    board.forEachPiece([this](const int row, const int col, Turn turn) {
        if (turn == Turn::Blue) {
            this->startBlue();

            mvwprintw(this->win,
                BOARD_START_ROW + this->board.getY(row, col),
                BOARD_START_COL + this->board.getX(row, col),
                "B"
            );

            this->endBlue();
        }

        if (turn == Turn::Red) {
            this->startRed();

            mvwprintw(this->win,
                BOARD_START_ROW + this->board.getY(row, col),
                BOARD_START_COL + this->board.getX(row, col),
                "R"
            );

            this->endRed();
        }
    });
}

/**
 * Initialize the screen.
 */
void Window::initialize()
{
    initscr();
    cbreak();
    noecho();
    keypad(stdscr, true);
    curs_set(1);

    win = stdscr;

    if (has_colors()) {
        start_color();
        use_default_colors();

        init_pair(BASE_PAIR, COLOR_BLACK, COLOR_BLACK);
        init_pair(BLUE_PAIR, COLOR_CYAN, COLOR_BLACK);
        init_pair(RED_PAIR, COLOR_RED, COLOR_BLACK);

        bkgd(COLOR_PAIR(BASE_PAIR));
        clear();
    }

    wrefresh(win);
}

/**
 * Render the game window.
 */
void Window::render(int& row, int&col)
{
    printHeader(row, col);
    renderBoard();
    renderPieces();
    renderColorMarkers();
    printFooter();
    box(win, 0, 0);
    printTitle();
    move(
        BOARD_START_ROW + board.getY(row, col),
        BOARD_START_COL + board.getX(row, col)
    );

    wrefresh(win);
}

/**
 * Process the user key.
 *
 * @param row The row number.
 * @param col The column number.
 *
 * @return Key pressed by the user.
 */
int Window::getKey(int& row, int&col)
{
    int key = getch();

    if (board.playerWon() != Turn::Undecided)
        return key;

    if (key == KEY_LEFT) {
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

    return key;
}