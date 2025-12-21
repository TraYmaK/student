#include <iostream>
#include <string>
#include <limits>

using namespace std;

constexpr size_t kMinBoardSize = 3;
constexpr size_t kMaxBoardSize = 10;

#include "TicTacToe.hpp"

void drawBoard(const TicTacToe& game) {
    size_t boardSize = game.size();

    for (size_t rowIndex = 0; rowIndex < boardSize; ++rowIndex) {
        cout << "+";
        for (size_t columnIndex = 0; columnIndex < boardSize; ++columnIndex) {
            cout << "---+";
        }
        cout << '\n';

        cout << "|";
        for (size_t columnIndex = 0; columnIndex < boardSize; ++columnIndex) {
            char symbol = cellToChar(game.cellAt(rowIndex, columnIndex));
            cout << " " << symbol << " |";
        }
        cout << '\n';
    }

    cout << "+";
    for (size_t columnIndex = 0; columnIndex < boardSize; ++columnIndex) {
        cout << "---+";
    }
    cout << '\n';
}

bool readCoordinate(const string& prompt, size_t boardSize, size_t& coordinate) {
    cout << prompt;
    int inputValue = 0;

    if (!(cin >> inputValue)) {
        return false;
    }

    if (inputValue <= 0) {
        return false;
    }

    size_t value = static_cast<size_t>(inputValue - 1);
    if (value >= boardSize) {
        return false;
    }

    coordinate = value;
    return true;
}

bool readBoardSize(size_t& boardSize) {
    cout << "Введите размер поля (от "<< kMinBoardSize << " до " << kMaxBoardSize << "): ";

    if (!(cin >> boardSize)) {
        return false;
    }
    if (boardSize < kMinBoardSize || boardSize > kMaxBoardSize) {
        return false;
    }
    return true;
}

void switchPlayer(Cell& currentPlayer) {
    if (currentPlayer == Cell::Cross) {
        currentPlayer = Cell::Nought;
    } else {
        currentPlayer = Cell::Cross;
    }
}

void playTurn(TicTacToe& game, Cell currentPlayer) {
    size_t rowIndex = 0;
    size_t columnIndex = 0;
    bool validInput = false;

    while (!validInput) {
        bool rowOk = readCoordinate("Введите номер строки: ", game.size(), rowIndex);
        bool colOk = readCoordinate("Введите номер столбца: ", game.size(), columnIndex);

        if (!rowOk || !colOk) {
            cout << "Некорректные координаты. Попробуйте ещё раз.\n";
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            continue;
        }

        if (!game.canPlace(rowIndex, columnIndex)) {
            cout << "Клетка занята. Выберите другую.\n";
            continue;
        }

        game.cellAt(rowIndex, columnIndex) = currentPlayer;
        validInput = true;
    }
}

void runGame(TicTacToe& game) {
    Cell currentPlayer = Cell::Cross;

    while (true) {
        cout << "\nТекущий игрок: " << cellToChar(currentPlayer) << "\n";
        drawBoard(game);

        playTurn(game, currentPlayer);

        Cell winner = game.winner();
        if (winner != Cell::Empty) {
            drawBoard(game);
            cout << "Игрок " << cellToChar(winner) << " победил!\n";
            break;
        }

        if (!game.hasEmptyCells()) {
            drawBoard(game);
            cout << "Ничья. Поле заполнено.\n";
            break;
        }

        switchPlayer(currentPlayer);
    }
}

int main() {
    size_t boardSize = 0;

    if (!readBoardSize(boardSize)) {
        cout << "Некорректный размер поля.\n";
        return 1;
    }

    TicTacToe game(boardSize);
    runGame(game);

    return 0;
}