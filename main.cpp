#include <iostream>
#include <vector>
#include <cstdlib>
#include <ctime>
#include <limits>

class MinesweeperBoard {
private:
    std::vector<std::vector<char>> board;
    std::vector<std::vector<bool>> mineLocations;
    int rows;
    int cols;
    bool gameOver;

    void initializeBoard();
    void placeMines();
    void revealCell(int row, int col);
    void printBoard();
    int countAdjacentMines(int row, int col);
    bool isValidMove(int row, int col);

public:
    MinesweeperBoard(int rows, int cols);
    void play();
};

MinesweeperBoard::MinesweeperBoard(int rows, int cols)
        : rows(rows), cols(cols), gameOver(false) {
    initializeBoard();
    placeMines();
}

void MinesweeperBoard::initializeBoard() {
    board.assign(rows, std::vector<char>(cols, 'H'));
    mineLocations.assign(rows, std::vector<bool>(cols, false));
}

void MinesweeperBoard::placeMines() {
    // Seed for randomization
    std::srand(std::time(0));

    // Place mines randomly
    for (int i = 0; i < rows * cols / 5; ++i) {
        int row = std::rand() % rows;
        int col = std::rand() % cols;

        // Avoid placing mines on the same location
        while (mineLocations[row][col]) {
            row = std::rand() % rows;
            col = std::rand() % cols;
        }

        mineLocations[row][col] = true;
    }
}

void MinesweeperBoard::revealCell(int row, int col) {
    if (!isValidMove(row, col) || board[row][col] != 'H') {
        return;
    }

    if (mineLocations[row][col]) {
        gameOver = true;
        board[row][col] = '*';
    } else {
        int mines = countAdjacentMines(row, col);
        if (mines > 0) {
            board[row][col] = '0' + mines;
        } else {
            board[row][col] = ' ';
            revealCell(row - 1, col - 1);
            revealCell(row - 1, col);
            revealCell(row - 1, col + 1);
            revealCell(row, col - 1);
            revealCell(row, col + 1);
            revealCell(row + 1, col - 1);
            revealCell(row + 1, col);
            revealCell(row + 1, col + 1);
        }
    }
}

int MinesweeperBoard::countAdjacentMines(int row, int col) {
    int count = 0;
    for (int i = row - 1; i <= row + 1; ++i) {
        for (int j = col - 1; j <= col + 1; ++j) {
            if (isValidMove(i, j) && mineLocations[i][j]) {
                ++count;
            }
        }
    }
    return count;
}

bool MinesweeperBoard::isValidMove(int row, int col) {
    return row >= 0 && row < rows && col >= 0 && col < cols;
}

void MinesweeperBoard::printBoard() {
    std::cout << "   ";
    for (int i = 0; i < cols; ++i) {
        std::cout << i << " ";
    }
    std::cout << "\n";

    for (int i = 0; i < rows; ++i) {
        std::cout << i << " |";
        for (int j = 0; j < cols; ++j) {
            std::cout << board[i][j] << " ";
        }
        std::cout << "\n";
    }
}

void MinesweeperBoard::play() {
    while (!gameOver) {
        printBoard();

        int row, col;
        std::cout << "Enter row and column (space-separated): ";
        std::cin >> row >> col;

        if (!isValidMove(row, col) || board[row][col] != 'H') {
            std::cout << "Invalid move! Try again.\n";
            std::cin.clear();
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            continue;
        }

        revealCell(row, col);

        if (gameOver) {
            std::cout << "Game over! You hit a mine.\n";
        }
    }

    printBoard();
}

int main() {
    int rows, cols;
    std::cout << "Enter the number of rows and columns: ";
    std::cin >> rows >> cols;

    MinesweeperBoard minesweeper(rows, cols);
    minesweeper.play();

    return 0;
}
