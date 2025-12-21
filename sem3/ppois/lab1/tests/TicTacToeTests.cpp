#include <gtest/gtest.h>
#include "../TicTacToe.hpp"

TEST(CellToCharTests, ReturnsCorrectChars) {
    EXPECT_EQ(cellToChar(Cell::Empty), ' ');
    EXPECT_EQ(cellToChar(Cell::Cross), 'X');
    EXPECT_EQ(cellToChar(Cell::Nought), 'O');
}

TEST(TicTacToeTests, BoardSizeIsCorrect) {
    TicTacToe game(3);
    EXPECT_EQ(game.size(), 3u);

    TicTacToe game10(10);
    EXPECT_EQ(game10.size(), 10u);
}

TEST(TicTacToeTests, AllCellsEmptyAfterConstruction3x3) {
    TicTacToe game(3);

    for (size_t row = 0; row < game.size(); ++row) {
        for (size_t col = 0; col < game.size(); ++col) {
            EXPECT_EQ(game.cellAt(row, col), Cell::Empty);
        }
    }

    EXPECT_TRUE(game.hasEmptyCells());
    EXPECT_EQ(game.winner(), Cell::Empty);
}

TEST(TicTacToeTests, AllCellsEmptyAfterConstruction5x5) {
    TicTacToe game(5);

    for (size_t row = 0; row < game.size(); ++row) {
        for (size_t col = 0; col < game.size(); ++col) {
            EXPECT_EQ(game.cellAt(row, col), Cell::Empty);
        }
    }

    EXPECT_TRUE(game.hasEmptyCells());
    EXPECT_EQ(game.winner(), Cell::Empty);
}

TEST(TicTacToeTests, CellAtReadWrite) {
    TicTacToe game(3);

    game.cellAt(1, 1) = Cell::Cross;
    EXPECT_EQ(game.cellAt(1, 1), Cell::Cross);

    game.cellAt(2, 0) = Cell::Nought;
    EXPECT_EQ(game.cellAt(2, 0), Cell::Nought);
}

TEST(TicTacToeTests, OperatorIndexReadWrite) {
    TicTacToe game(3);

    game[0] = Cell::Cross;
    EXPECT_EQ(game[0], Cell::Cross);

    size_t index = 1 * game.size() + 2;
    game[index] = Cell::Nought;
    EXPECT_EQ(game[index], Cell::Nought);
}

TEST(TicTacToeTests, CanPlaceOnEmptyCell) {
    TicTacToe game(3);

    EXPECT_TRUE(game.canPlace(0, 0));
    EXPECT_TRUE(game.canPlace(2, 2));

    game.cellAt(0, 0) = Cell::Cross;
    EXPECT_FALSE(game.canPlace(0, 0));
}

TEST(TicTacToeTests, CanPlaceOutOfBounds) {
    TicTacToe game(3);

    EXPECT_FALSE(game.canPlace(3, 0));
    EXPECT_FALSE(game.canPlace(0, 3));
    EXPECT_FALSE(game.canPlace(100, 100));
}

TEST(TicTacToeTests, HasEmptyCellsBecomesFalseWhenFull) {
    TicTacToe game(3);

    EXPECT_TRUE(game.hasEmptyCells());

    for (size_t row = 0; row < game.size(); ++row) {
        for (size_t col = 0; col < game.size(); ++col) {
            game.cellAt(row, col) = Cell::Cross;
        }
    }

    EXPECT_FALSE(game.hasEmptyCells());
}

TEST(TicTacToeTests, NoWinnerOnEmptyBoard) {
    TicTacToe game(3);
    EXPECT_EQ(game.winner(), Cell::Empty);
}

TEST(TicTacToeTests, NoWinnerOnMixedBoardWithoutLine) {
    TicTacToe game(3);

    game.cellAt(0, 0) = Cell::Cross;
    game.cellAt(0, 1) = Cell::Nought;
    game.cellAt(1, 1) = Cell::Cross;
    game.cellAt(2, 2) = Cell::Nought;

    EXPECT_EQ(game.winner(), Cell::Empty);
}

TEST(TicTacToeTests, RowWinForCross) {
    TicTacToe game(3);

    game.cellAt(1, 0) = Cell::Cross;
    game.cellAt(1, 1) = Cell::Cross;
    game.cellAt(1, 2) = Cell::Cross;

    EXPECT_EQ(game.winner(), Cell::Cross);
}

TEST(TicTacToeTests, RowWinForNought) {
    TicTacToe game(3);

    game.cellAt(2, 0) = Cell::Nought;
    game.cellAt(2, 1) = Cell::Nought;
    game.cellAt(2, 2) = Cell::Nought;

    EXPECT_EQ(game.winner(), Cell::Nought);
}

TEST(TicTacToeTests, ColumnWinForCross) {
    TicTacToe game(3);

    game.cellAt(0, 2) = Cell::Cross;
    game.cellAt(1, 2) = Cell::Cross;
    game.cellAt(2, 2) = Cell::Cross;

    EXPECT_EQ(game.winner(), Cell::Cross);
}

TEST(TicTacToeTests, ColumnWinForNought) {
    TicTacToe game(3);

    game.cellAt(0, 1) = Cell::Nought;
    game.cellAt(1, 1) = Cell::Nought;
    game.cellAt(2, 1) = Cell::Nought;

    EXPECT_EQ(game.winner(), Cell::Nought);
}

TEST(TicTacToeTests, MainDiagonalWinForCross) {
    TicTacToe game(3);

    game.cellAt(0, 0) = Cell::Cross;
    game.cellAt(1, 1) = Cell::Cross;
    game.cellAt(2, 2) = Cell::Cross;

    EXPECT_EQ(game.winner(), Cell::Cross);
}

TEST(TicTacToeTests, SecondDiagonalWinForNought) {
    TicTacToe game(3);

    game.cellAt(0, 2) = Cell::Nought;
    game.cellAt(1, 1) = Cell::Nought;
    game.cellAt(2, 0) = Cell::Nought;

    EXPECT_EQ(game.winner(), Cell::Nought);
}

TEST(TicTacToeTests, WinOnLargerBoardRow) {
    TicTacToe game(5);

    size_t row = 3;
    for (size_t col = 0; col < game.size(); ++col) {
        game.cellAt(row, col) = Cell::Cross;
    }

    EXPECT_EQ(game.winner(), Cell::Cross);
}

TEST(TicTacToeTests, WinOnLargerBoardColumn) {
    TicTacToe game(5);

    size_t col = 4;
    for (size_t row = 0; row < game.size(); ++row) {
        game.cellAt(row, col) = Cell::Nought;
    }

    EXPECT_EQ(game.winner(), Cell::Nought);
}

TEST(TicTacToeTests, WinOnLargerBoardMainDiagonal) {
    TicTacToe game(5);

    for (size_t i = 0; i < game.size(); ++i) {
        game.cellAt(i, i) = Cell::Cross;
    }

    EXPECT_EQ(game.winner(), Cell::Cross);
}

TEST(TicTacToeTests, WinOnLargerBoardSecondDiagonal) {
    TicTacToe game(5);

    size_t n = game.size();
    for (size_t i = 0; i < n; ++i) {
        game.cellAt(i, n - 1 - i) = Cell::Nought;
    }

    EXPECT_EQ(game.winner(), Cell::Nought);
}