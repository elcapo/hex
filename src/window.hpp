#ifndef WINDOW_H
#define WINDOW_H

#include <cstddef>
#include <cstdlib>
#include <iostream>
#include <ncurses.h>
#include "board.hpp"

#define BASE_PAIR 0
#define BLUE_PAIR 1
#define RED_PAIR 2

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

class Window
{
private:
    WINDOW* win;
    Board& board;
public:
    Window(Board& board) : board(board) {
        win = nullptr;
    }

    ~Window() {
        endwin();
    }

    /**
     * Activate the blue color.
     */
    void startBlue();

    /**
     * Deactivate the blue color.
     */
    void endBlue();

    /**
     * Activate the red color.
     */
    void startRed();

    /**
     * Deactivate the blue color.
     */
    void endRed();

    /**
     * Print the title of the window.
     */
    void printTitle();

    /**
     * Print the header of the window.
     */
    void printHeader(int& row, int&col);

    /**
     * Print the footer of the window.
     */
    void printFooter();

    /**
     * Render the marks that indicate which side corresponds to
     * each player.
     */
    void renderColorMarkers();

    /**
     * Render the board in the given window starting at the
     * position given by BOARD_START_ROW and BOARD_START_COL.
     */
    void renderBoard();

    /**
     * Render the pieces of the board in the given window starting at the
     * position given by BOARD_START_ROW and BOARD_START_COL.
     */
    void renderPieces();

    /**
     * Initialize the screen.
     */
    void initialize();

    /**
     * Render the game window.
     */
    void render(int& row, int&col);
};

#endif // WINDOW_H