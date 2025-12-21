#pragma once

#include <cstddef>
#include <vector>

using namespace std;

enum class Cell {
    Empty,
    Cross,
    Nought
};

char cellToChar(Cell cell);

class TicTacToe {
public:
    explicit TicTacToe(size_t boardSize);

    size_t size() const noexcept;

    Cell& operator[](size_t index);
    const Cell& operator[](size_t index) const;

    Cell& cellAt(size_t rowIndex, size_t columnIndex);
    Cell cellAt(size_t rowIndex, size_t columnIndex) const;

    bool canPlace(size_t rowIndex, size_t columnIndex) const;
    bool hasEmptyCells() const;
    Cell winner() const;

private:
    size_t boardSize_;
    vector<Cell> cells_;

    size_t index(size_t rowIndex, size_t columnIndex) const;

    bool checkWinFor(Cell player) const;
    bool checkRows(Cell player) const;
    bool checkColumns(Cell player) const;
    bool checkDiagonals(Cell player) const;
};