#include <iostream>
#include <vector>
#include <cstdlib>
#include <ctime>
#include <stdexcept>

using namespace std;

const int MAXSIDE = 25;
const int MAXMINES = 99;
const int MOVESIZE = 526;

class Game {
public:
    virtual ~Game() {}
    virtual void play() = 0;
    virtual void printBoard(const vector<vector<char>> &myBoard) = 0;
    virtual bool makeMove(int *x, int *y) = 0;

};

class MinesweeperGame : public Game {
private:
    int SIDE;
    int MINES;
    vector<vector<char>> realBoard;
    vector<vector<char>> myBoard;
    int movesLeft;
    int mines[MAXMINES][2];

public:
    MinesweeperGame(int side, int mines) : SIDE(side), MINES(mines), movesLeft(SIDE * SIDE - MINES) {
        realBoard.resize(SIDE, vector<char>(SIDE, '-'));
        myBoard.resize(SIDE, vector<char>(SIDE, '-'));

        initialize();
        placeMines();
    }

    void play() override {
        bool gameOver = false;
        int x, y;

        while (!gameOver) {
            cout << "Current Status of Board : \n";
            printBoard(myBoard);

            if (!makeMove(&x, &y)) {
                gameOver = playMinesweeperUtil(x, y);

                if (!gameOver && movesLeft == 0) {
                    cout << "\nYou won!\n";
                    gameOver = true;
                }
            }
        }


    }

    void printBoard(const vector<vector<char>> &board) override {
        int i, j;

        cout << "    ";

        for (i = 0; i < SIDE; i++)
            cout << i << ' ';

        cout << "\n\n";

        for (i = 0; i < SIDE; i++) {
            cout << i << "   ";

            for (j = 0; j < SIDE; j++)
                cout << board[i][j] << ' ';

            cout << '\n';
        }
    }

    bool makeMove(int *x, int *y) override {
        cout << "Enter your move, (row, column) -> ";
        cin >> *x >> *y;
        return (*x < 0 || *x >= SIDE || *y < 0 || *y >= SIDE);
    }



private:
    void initialize() {
        srand(time(NULL));

        for (int i = 0; i < SIDE; i++) {
            for (int j = 0; j < SIDE; j++) {
                myBoard[i][j] = realBoard[i][j] = '-';
            }
        }
    }

    void placeMines() {
        vector<bool> mark(SIDE * SIDE, false);

        for (int i = 0; i < MINES;) {
            int random = rand() % (SIDE * SIDE);
            int x = random / SIDE;
            int y = random % SIDE;

            if (!mark[random]) {
                mines[i][0] = x;
                mines[i][1] = y;

                realBoard[mines[i][0]][mines[i][1]] = '*';
                mark[random] = true;
                i++;
            }
        }
    }

    bool playMinesweeperUtil(int row, int col) {
        if (myBoard[row][col] != '-')
            return false;

        if (realBoard[row][col] == '*') {
            myBoard[row][col] = '*';

            for (int i = 0; i < MINES; i++)
                myBoard[mines[i][0]][mines[i][1]] = '*';

            printBoard(myBoard);
            cout << "\nYou lost!\n";
            return true;
        } else {
            int count = countAdjacentMines(row, col);
            movesLeft--;

            myBoard[row][col] = count + '0';

            if (!count) {
                for (int i = row - 1; i <= row + 1; i++) {
                    for (int j = col - 1; j <= col + 1; j++) {
                        if (isValid(i, j) && !isMine(i, j)) {
                            playMinesweeperUtil(i, j);
                        }
                    }
                }
            }

            return false;
        }
    }

    int countAdjacentMines(int row, int col) {
        int count = 0;

        for (int i = row - 1; i <= row + 1; i++) {
            for (int j = col - 1; j <= col + 1; j++) {
                if (isValid(i, j) && isMine(i, j)) {
                    count++;
                }
            }
        }

        return count;
    }

    bool isValid(int row, int col) {
        return (row >= 0) && (row < SIDE) &&
               (col >= 0) && (col < SIDE);
    }

    bool isMine(int row, int col) {
        return realBoard[row][col] == '*';
    }
};

void chooseDifficultyLevel() {
    int level;

    cout << "Enter the Difficulty Level\n";
    cout << "Press 0 for EASY (9 * 9 Cells and 10 Mines)\n";
    cout << "Press 1 for MEDIUM (16 * 16 Cells and 40 Mines)\n";
    cout << "Press 2 for HARD (24 * 24 Cells and 99 Mines)\n";

    cin >> level;

    if (level < 0 || level > 2) {
        throw invalid_argument("Invalid difficulty level");
    }

    int side, mines;
    if (level == 0) {
        side = 9;
        mines = 10;
    } else if (level == 1) {
        side = 16;
        mines = 40;
    } else {
        side = 24;
        mines = 99;
    }

    MinesweeperGame game(side, mines);
    game.play();
}

int main() {
    try {
        chooseDifficultyLevel();
    } catch (const exception &e) {
        cerr << "Exception: " << e.what() << endl;
    }

    return 0;
}
