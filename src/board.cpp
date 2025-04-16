#include <algorithm>
#include <iostream>
#include <cmath>
#include <random>
#include "board.hpp"
#include "strategy.hpp"

void Board::connectBorders()
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
}

Board::Board(int size, HumanPlayers humanPlayers) :
    size(size),
    humanPlayers(humanPlayers),
    turn(Turn::Blue),
    winner(Turn::Undecided),
    movements(0),
    opening(std::make_pair(-1, -1)),
    blueGraph(size*size, size*size*6),
    redGraph(size*size, size*size*6),
    positions(size),
    blueStrategy(nullptr),
    redStrategy(nullptr)
{
    connectBorders();

    if (turn == Turn::Blue && !humanPlayers.blue)
        playBlueMove();
}

Board::Board(const Board& other) :
    size(other.size),
    humanPlayers({true, true}), // Siempre crear copias con jugadores humanos
    turn(other.turn),
    winner(other.winner),
    movements(other.movements),
    opening(other.opening),
    blueGraph(other.size*other.size, other.size*other.size*6),
    redGraph(other.size*other.size, other.size*other.size*6),
    positions(other.size),
    blueStrategy(nullptr),
    redStrategy(nullptr)
{
    connectBorders();

    // Copiar todas las posiciones
    for (int row = 0; row < size; row++) {
        for (int col = 0; col < size; col++) {
            Position position = std::make_pair(row, col);
            positions[position] = other.positions[position];
            
            if (positions[position] == Turn::Blue) {
                connectBlue(row, col);
            }
            else if (positions[position] == Turn::Red) {
                connectRed(row, col);
            }
        }
    }
}

Board& Board::operator=(const Board& other)
{
    if (this == &other) {
        return *this;
    }

    size = other.size;
    humanPlayers = HumanPlayers({true, true});
    turn = other.turn;
    winner = other.winner;
    movements = other.movements;
    opening = other.opening;

    blueGraph = Graph(size*size, size*size*6);
    redGraph = Graph(size*size, size*size*6);

    connectBorders();

    positions = Positions(size);

    for (int row = 0; row < size; row++) {
        for (int col = 0; col < size; col++) {
            Position position = std::make_pair(row, col);
            positions[position] = other.positions[position];
            
            if (positions[position] == Turn::Blue) {
                connectBlue(row, col);
            }
            else if (positions[position] == Turn::Red) {
                connectRed(row, col);
            }
        }
    }
    
    // No copiar las estrategias
    blueStrategy = nullptr;
    redStrategy = nullptr;
    
    return *this;
}

void Board::setStrategy(Turn player, std::unique_ptr<MoveStrategy> strategy) {
    if (player == Turn::Blue) {
        blueStrategy = std::move(strategy);
    } else if (player == Turn::Red) {
        redStrategy = std::move(strategy);
    }
}

void Board::next()
{
    if (turn == Turn::Blue) {
        turn = Turn::Red;
    } else if (turn == Turn::Red) {
        turn = Turn::Blue;
    }

    movements++;

    if (playerWon() != Turn::Undecided)
        return;

    if (turn == Turn::Blue && !humanPlayers.blue) {
        playBlueMove();
    } else if (turn == Turn::Red && !humanPlayers.red) {
        playRedMove();
    }
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

void Board::playComputerMove()
{
    Position position;
    
    if (turn == Turn::Blue && blueStrategy != nullptr) {
        position = blueStrategy->getNextMove(*this);
    } else if (turn == Turn::Red && redStrategy != nullptr) {
        position = redStrategy->getNextMove(*this);
    } else {
        return; // No strategy available for current player
    }
    
    set(position.first, position.second);
}

void Board::playBlueMove()
{
    if (blueStrategy != nullptr) {
        // Usar la estrategia configurada
        playComputerMove();
    } else {
        // Estrategia por defecto (crear una AIStrategy temporal)
        AIStrategy tempStrategy(Turn::Blue);
        Position position = tempStrategy.getNextMove(*this);
        set(position.first, position.second);
    }
}

void Board::playRedMove()
{
    if (redStrategy != nullptr) {
        // Usar la estrategia configurada
        playComputerMove();
    } else {
        // Estrategia por defecto (crear una AIStrategy temporal)
        AIStrategy tempStrategy(Turn::Red);
        Position position = tempStrategy.getNextMove(*this);
        set(position.first, position.second);
    }
}

void Board::checkGame()
{
    Dijkstra blueDijkstra(blueGraph);

    if (blueDijkstra.nodesAreConnected(cell(0, 0), cell(size - 1, size - 1))) {
        winner = Turn::Blue;
        turn = Turn::Undecided;
    }

    Dijkstra redDijkstra(redGraph);

    if (redDijkstra.nodesAreConnected(cell(0, 0), cell(size - 1, size - 1))) {
        winner = Turn::Red;
        turn = Turn::Undecided;
    }
}

bool Board::exists(int row, int col) const
{
    return row >= 0 && row < size && col >= 0 && col < size;
}

int Board::getSize() const
{
    return size;
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

void Board::set(int row, int col, bool checkWinner)
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

    if (checkWinner)
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

void Board::forEachEmptyPosition(std::function<void(const int row, const int col)> callback) const
{
    int rowIds[size];
    int colIds[size];

    for (int i = 0; i < size; ++i) {
        rowIds[i] = i;
        colIds[i] = i;
    }

    std::random_device random_device;
    std::mt19937 twister(random_device());
    std::shuffle(rowIds, rowIds + size, twister);
    std::shuffle(colIds, colIds + size, twister);

    Position position = std::make_pair(rowIds[0], colIds[0]);

    for (int i = 0; i < size; ++i) {
        for (int j = 0; j < size; ++j) {
            int row = rowIds[i];
            int col = colIds[j];
            position = std::make_pair(row, col);

            if (positions[position] == Turn::Undecided)
                callback(row, col);
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