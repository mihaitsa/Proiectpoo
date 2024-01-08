#include <iostream>
#include <vector>
#include <cstdlib>
#include <ctime>
#include <stdexcept>
#include<limits>

using namespace std;


const int MAXSIDE = 25;
const int MAXMINES = 99;

// abstract base class
class Game {
private:
    static int gameCount;

public:
    virtual ~Game() {
        gameCount--;
    }
    //pure virtual functions that are to be inherited
    virtual void play() = 0;
    virtual void printBoard(const vector<vector<char>> &myBoard) = 0;
    virtual bool makeMove(int *x, int *y) = 0;
    virtual Game *clone() const = 0;

    static int getGameCount() {
        return gameCount;
    }

protected:
    Game() {//increment of the gamecount in the constructor
        gameCount++;
    }
};

int Game::gameCount = 0;

//derived class representing the Minesweeper game
class MinesweeperGame : public Game {
private:
    int SIDE; //size
    int MINES; //no. of mines
    vector<vector<char>> realBoard; //real board of Minesweeper
    vector<vector<char>> myBoard; // player's view of that board
    int movesLeft;
    int mines[MAXMINES][2]; //array to store mine coordinates

public:
    // constructor initializes game parameters, creates boards, and places mines
    MinesweeperGame(int side, int mines) : SIDE(side), MINES(mines), movesLeft(SIDE * SIDE - MINES) {
        realBoard.resize(SIDE, vector<char>(SIDE, '-'));
        myBoard.resize(SIDE, vector<char>(SIDE, '-'));

        initialize();
        placeMines();
    }
    //function to start and play the Minesweeper game
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
    //print board function
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

    MinesweeperGame *clone() const override {
        return new MinesweeperGame(*this);
    }

private:
    //function to initialize
    void initialize() {
        srand(time(NULL));

        for (int i = 0; i < SIDE; i++) {
            for (int j = 0; j < SIDE; j++) {
                myBoard[i][j] = realBoard[i][j] = '-';
            }
        }
    }
    //function to place mines
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
    //recursive function to play the Minesweeper game
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
    //function that counts the adjacent mines
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
    //check function to see if the input coordinates are valid
    bool isValid(int row, int col) {
        if (row < 0 || row >= SIDE || col < 0 || col >= SIDE) {
            return false;
        }
        return true;
    }
    //function to check if at the input coordinates there's a mine
    bool isMine(int row, int col) {
        return realBoard[row][col] == '*';
    }
};

// abstract factory class for creating games
class GameFactory {
public:
    virtual Game *createGame() = 0;
    virtual ~GameFactory() {}
};

// concrete factory for creating MinesweeperGame with EASY difficulty
class EasyGameFactory : public GameFactory {
public:
    Game *createGame() override {
        return new MinesweeperGame(9, 10);
    }
};

// concrete factory for creating MinesweeperGame with MEDIUM difficulty
class MediumGameFactory : public GameFactory {
public:
    Game *createGame() override {
        return new MinesweeperGame(16, 40);
    }
};

// concrete factory for creating MinesweeperGame with HARD difficulty
class HardGameFactory : public GameFactory {
public:
    Game *createGame() override {
        return new MinesweeperGame(24, 99);
    }
};

// singleton class representing the Minesweeper game manager
class MinesweeperGameManager {
private:
    Game *gameInstance;

    MinesweeperGameManager() : gameInstance(nullptr) {}

    // prevent copying and assignment
    MinesweeperGameManager(const MinesweeperGameManager &) = delete;
    MinesweeperGameManager &operator=(const MinesweeperGameManager &) = delete;

public:
    static MinesweeperGameManager &getInstance() {
        static MinesweeperGameManager instance;
        return instance;
    }

    ~MinesweeperGameManager() {
        delete gameInstance;
    }

    void startGame(GameFactory *factory) {
        if (gameInstance) {
            cout << "A game is already in progress.\n";
            return;
        }

        gameInstance = factory->createGame();
        gameInstance->play();
        delete gameInstance;
        gameInstance = nullptr;
    }
};

// Main function with an interactive menu
int main() {
    MinesweeperGameManager &manager = MinesweeperGameManager::getInstance();
    int choice;

    do {
        cout << "\nMinesweeper Game Menu:\n";
        cout << "1. Start Easy Game\n";
        cout << "2. Start Medium Game\n";
        cout << "3. Start Hard Game\n";
        cout << "4. Exit\n";
        cout << "Enter your choice: ";

        try {
            if (!(cin >> choice)) {
                // Input is not an integer
                throw invalid_argument("Invalid input. Please enter a valid integer.");
            }

            switch (choice) {
                case 1:
                    manager.startGame(new EasyGameFactory());
                    break;
                case 2:
                    manager.startGame(new MediumGameFactory());
                    break;
                case 3:
                    manager.startGame(new HardGameFactory());
                    break;
                case 4:
                    cout << "Goodbye!...\n";
                    break;
                default:
                    throw invalid_argument("Invalid choice. Please enter a valid option.");
            }
        } catch (const exception &e) {
            cerr << "Error: " << e.what() << "\n";
            cin.clear();  // clear the error flag
            cin.ignore(numeric_limits<streamsize>::max(), '\n');  // discard invalid input
        }
    } while (choice != 4);

    cout << "Total game instances: " << Game::getGameCount() << endl;

 return 0;
}
