#include "TicTacToe.hpp"

char cellToChar(Cell cell) {
    if (cell == Cell::Cross) {
        return 'X';
    }
    if (cell == Cell::Nought) {
        return 'O';
    }
    return ' ';
}

TicTacToe::TicTacToe(size_t boardSize) {
    boardSize_ = boardSize;
    cells_.resize(boardSize * boardSize, Cell::Empty);
}

size_t TicTacToe::size() const noexcept {
    return boardSize_;
}

Cell& TicTacToe::operator[](size_t index) {
    return cells_[index];
}

const Cell& TicTacToe::operator[](size_t index) const {
    return cells_[index];
}

Cell& TicTacToe::cellAt(size_t rowIndex, size_t columnIndex) {
    return cells_[index(rowIndex, columnIndex)];
}

Cell TicTacToe::cellAt(size_t rowIndex, size_t columnIndex) const {
    return cells_[index(rowIndex, columnIndex)];
}

bool TicTacToe::canPlace(size_t rowIndex, size_t columnIndex) const {
    if (rowIndex >= boardSize_ || columnIndex >= boardSize_) {
        return false;
    }
    return cellAt(rowIndex, columnIndex) == Cell::Empty;
}

bool TicTacToe::hasEmptyCells() const {
    for (size_t i = 0; i < cells_.size(); ++i) {
        if (cells_[i] == Cell::Empty) {
            return true;
        }
    }
    return false;
}

Cell TicTacToe::winner() const {
    if (checkWinFor(Cell::Cross)) {
        return Cell::Cross;
    }
    if (checkWinFor(Cell::Nought)) {
        return Cell::Nought;
    }
    return Cell::Empty;
}

size_t TicTacToe::index(size_t rowIndex, size_t columnIndex) const {
    return rowIndex * boardSize_ + columnIndex;
}

bool TicTacToe::checkWinFor(Cell player) const {
    return checkRows(player) || checkColumns(player) || checkDiagonals(player);
}

bool TicTacToe::checkRows(Cell player) const {
    for (size_t rowIndex = 0; rowIndex < boardSize_; ++rowIndex) {
        bool allMatch = true;
        for (size_t columnIndex = 0; columnIndex < boardSize_; ++columnIndex) {
            if (cellAt(rowIndex, columnIndex) != player) {
                allMatch = false;
                break;
            }
        }
        if (allMatch) {
            return true;
        }
    }
    return false;
}

bool TicTacToe::checkColumns(Cell player) const {
    for (size_t columnIndex = 0; columnIndex < boardSize_; ++columnIndex) {
        bool allMatch = true;
        for (size_t rowIndex = 0; rowIndex < boardSize_; ++rowIndex) {
            if (cellAt(rowIndex, columnIndex) != player) {
                allMatch = false;
                break;
            }
        }
        if (allMatch) {
            return true;
        }
    }
    return false;
}

bool TicTacToe::checkDiagonals(Cell player) const {
    bool mainDiagonalMatch = true;
    bool secondaryDiagonalMatch = true;

    for (size_t i = 0; i < boardSize_; ++i) {
        if (cellAt(i, i) != player) {
            mainDiagonalMatch = false;
        }
        if (cellAt(i, boardSize_ - 1 - i) != player) {
            secondaryDiagonalMatch = false;
        }
    }

    return mainDiagonalMatch || secondaryDiagonalMatch;
}