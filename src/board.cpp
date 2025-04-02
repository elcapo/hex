#include <iostream>
#include <cmath>
#include "board.hpp"

const char* turnAsChar(const Turn& t)
{
    switch (t) {
        case Blue:
            return "B";
        case Red:
            return "R";
        default:
            return "o";
    }
}

std::string turnAsLabel(const Turn& t)
{
    switch (t) {
        case Blue:
            return "Blue plays";
        case Red:
            return "Red plays";
        default:
            return "Game finished";
    }
}

Board::Board(int size) :
    size(size),
    turn(Turn::Blue),
    winner(Turn::Undecided),
    movements(0),
    opening({-1, -1}),
    blueGraph(size*size, size*size*6),
    redGraph(size*size, size*size*6),
    blueDijkstra(blueGraph),
    redDijkstra(redGraph),
    positions(size)
{
    for (int col = 0; col < size; col++) {
        if (exists(0, col + 1))
            redGraph.connect(cell(0, col), cell(0, col + 1));

        if (exists(size - 1, col + 1))
            redGraph.connect(cell(size - 1, col), cell(size - 1, col + 1));
    }

    for (int row = 0; row < size; row++) {
        if (exists(row + 1, 0))
            blueGraph.connect(cell(row, 0), cell(row + 1, 0));

        if (exists(row + 1, size - 1))
            blueGraph.connect(cell(row, size - 1), cell(row + 1, size - 1));
    }
};

Board Board::clone() const
{
    Board copy(size);
    
    copy.turn = turn;
    copy.winner = winner;
    copy.movements = movements;
    copy.opening = opening;
    
    for (int row = 0; row < size; row++) {
        for (int col = 0; col < size; col++) {
            Position position = std::make_pair(row, col);
            if (positions[position] != Turn::Undecided) {
                copy.positions[position] = positions[position];
            }
        }
    }
    
    for (int row = 0; row < size; row++) {
        for (int col = 0; col < size; col++) {
            Position position = std::make_pair(row, col);
            if (positions[position] == Turn::Blue) {
                copy.connectBlue(row, col);
            } else if (positions[position] == Turn::Red) {
                copy.connectRed(row, col);
            }
        }
    }
    
    return copy;
}

void Board::next()
{
    if (turn == Turn::Blue) {
        turn = Turn::Red;
    } else if (turn == Turn::Red) {
        turn = Turn::Blue;
    }

    movements++;
}

void Board::connectBlue(int row, int col)
{
    if (exists(row - 1, col) && isBlue(row - 1, col))
        blueGraph.connect(cell(row, col), cell(row - 1, col));

    if (exists(row - 1, col + 1) && isBlue(row - 1, col + 1))
        blueGraph.connect(cell(row, col), cell(row - 1, col + 1));

    if (exists(row, col - 1) && isBlue(row, col - 1))
        blueGraph.connect(cell(row, col), cell(row, col - 1));

    if (exists(row, col + 1) && isBlue(row, col + 1))
        blueGraph.connect(cell(row, col), cell(row, col + 1));

    if (exists(row + 1, col - 1) && isBlue(row + 1, col - 1))
        blueGraph.connect(cell(row, col), cell(row + 1, col - 1));

    if (exists(row + 1, col) && isBlue(row + 1, col))
        blueGraph.connect(cell(row, col), cell(row + 1, col));
}

void Board::connectRed(int row, int col)
{
    if (exists(row - 1, col) && isRed(row - 1, col))
        redGraph.connect(cell(row, col), cell(row - 1, col));

    if (exists(row - 1, col + 1) && isRed(row - 1, col + 1))
        redGraph.connect(cell(row, col), cell(row - 1, col + 1));

    if (exists(row, col - 1) && isRed(row, col - 1))
        redGraph.connect(cell(row, col), cell(row, col - 1));

    if (exists(row, col + 1) && isRed(row, col + 1))
        redGraph.connect(cell(row, col), cell(row, col + 1));

    if (exists(row + 1, col - 1) && isRed(row + 1, col - 1))
        redGraph.connect(cell(row, col), cell(row + 1, col - 1));

    if (exists(row + 1, col) && isRed(row + 1, col))
        redGraph.connect(cell(row, col), cell(row + 1, col));
}

void Board::checkGame()
{
    if (blueDijkstra.nodesAreConnected(cell(0, 0), cell(size - 1, size - 1))) {
        winner = Turn::Blue;
        turn = Turn::Undecided;
    }

    if (redDijkstra.nodesAreConnected(cell(0, 0), cell(size - 1, size - 1))) {
        winner = Turn::Red;
        turn = Turn::Undecided;
    }
}

bool Board::exists(int row, int col) const
{
    return row >= 0 && row < size && col >= 0 && col < size;
}

int Board::getY(int row, int col) const
{
    return row*2;
}

int Board::getX(int row, int col) const
{
    return col*4 + row*2;
}

int Board::cell(int row, int col) const
{
    if (! exists(row, col)) {
        return -1;
    }

    return row * size + col;
}

const Turn& Board::current() const
{
    return turn;
}

const int& Board::countMovements() const
{
    return movements;
}

bool Board::isBlue(int row, int col)
{
    Position position = std::make_pair(row, col);

    return positions[position] == Turn::Blue;
}

bool Board::isRed(int row, int col)
{
    Position position = std::make_pair(row, col);

    return positions[position] == Turn::Red;
}

Turn Board::playerWon()
{
    return winner;
}

void Board::set(int row, int col)
{
    if (turn == Turn::Undecided)
        throw std::runtime_error("The game already ended");

    Position position = std::make_pair(row, col);

    if (positions[position] != Turn::Undecided)
        throw std::invalid_argument("The cell has already been chosen");

    if (opening.first == -1 || opening.second == -1)
        opening = position;

    positions[position] = turn;

    if (turn == Turn::Blue)
        connectBlue(row, col);

    if (turn == Turn::Red)
        connectRed(row, col);

    checkGame();

    next();
}

const Turn& Board::get(int row, int col) const
{
    Position position = std::make_pair(row, col);
    return positions[position];
}

void Board::pieRule()
{
    if (movements != 1) {
        throw std::invalid_argument("The pie rule can only be invoked right after the first move");
    }

    if (turn != Turn::Red) {
        throw std::invalid_argument("The pie rule can only be invoked by the red player after a blue opening");
    }

    positions[opening] = turn;
    next();
}

const Graph& Board::getBlueGraph() const {
    return blueGraph;
}

const Graph& Board::getRedGraph() const {
    return redGraph;
}

void Board::forEachLine(std::function<void(const char* line)> callback) const
{
    char line[128] = "";
    Turn piece;

    for (int row = 0; row < size; row++) {
        memset(line, 0, sizeof(line));

        for (int i = 0; i < row * 2; i++) {
            line[i] = ' ';
        }

        int pos = row * 2;

        for (int col = 0; col < size; col++) {
            line[pos++] = 'o';
            
            if (col + 1 < size) {
                line[pos++] = ' ';
                line[pos++] = '-';
                line[pos++] = ' ';
            }
        }

        line[pos++] = '\n';
        line[pos] = '\0';
        callback(line);

        if (row + 1 == size)
            break;

        memset(line, 0, sizeof(line));

        for (int i = 0; i < row * 2 + 1; i++) {
            line[i] = ' ';
        }
        
        pos = row * 2 + 1;

        for (int col = 0; col < size; col++) {
            line[pos++] = '\\';
            
            if (col + 1 < size) {
                line[pos++] = ' ';
                line[pos++] = '/';
                line[pos++] = ' ';
            }
        }

        line[pos++] = '\n';
        line[pos] = '\0';
        
        callback(line);
    }
}

void Board::forEachPiece(std::function<void(const int row, const int col, Turn turn)> callback) const
{
    Position position;

    for (int i = 0; i < size; i++) {
        for (int j = 0; j < size; j++) {
            position = std::make_pair(i, j);
            if (positions[position] != Turn::Undecided)
                callback(i, j, positions[position]);
        }
    }
}

std::ostream& operator<<(std::ostream& os, const Board& board)
{
    board.forEachLine([&os](const char* line) {
        os << line;
    });

    return os;
}