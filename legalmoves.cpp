#include "mainwindow.h"
#include <QGridLayout>
#include <QDebug>
#include <QPixmap>
#include <QLabel>
#include <QRect>
#include <QString>
#include <QPushButton>
#include <map>

std::vector<std::pair<int,int>> MainWindow::GetLegalMoves(int i, int j)
{
    std::map<std::pair<int,int>,bool> attackedSquares;
    std::vector<std::pair<int,int>> legalMoves;
    std::vector<std::pair<int,int>> moves = GetMoves(i, j);
    ChessPiece oldPiece[8][8];

    for (int x = 0; x < 8; ++x)
        for (int y = 0; y < 8; ++y)
            oldPiece[x][y] = piece[x][y];

    for (const auto& curr : moves)
    {
        Move(i, j, curr.first, curr.second, false);

        if (LegalPosition(IsBlack(oldPiece[i][j])))
            legalMoves.push_back(curr);

        for (int x = 0; x < 8; ++x)
            for (int y = 0; y < 8; ++y)
                SetPiece(x, y, oldPiece[x][y], false);
    }
    return legalMoves;
}

std::map<std::pair<int,int>,bool> MainWindow::GetAllAttackedSquares(bool attackerColor)
{
    std::map<std::pair<int,int>,bool> attackedSquares;
    for (int x = 0; x < 8; ++x)
        for (int y = 0; y < 8; ++y)
        {
            if (piece[x][y] == blank)
                continue;
            if (GetColor(piece[x][y]) == attackerColor)
            {
                std::vector<std::pair<int,int>> temp = GetAttackedSquares(x, y);
                for (const auto& curr : temp)
                    attackedSquares[curr] = true;
            }
        }
    return attackedSquares;
}

bool MainWindow::LegalPosition(bool lastMove)
{
    for (int x = 0; x < 8; ++x)
        for (int y = 0; y < 8; ++y)
            if (piece[x][y] == (lastMove == 0 ? white_king : black_king))
                return !SquareAttacked(x, y, !lastMove);
    return false;
}

bool MainWindow::SquareAttacked(int i, int j, bool attackerColor)
{
    for (int direction = 0; direction < 4; ++direction)
    {
        int x = i + deltaX[direction];
        int y = j + deltaY[direction];
        while (!OutOfTable(x, y))
        {
            if (piece[x][y] == blank)
            {
                x += deltaX[direction];
                y += deltaY[direction];
                continue;
            }
            if (piece[x][y] == Queen(attackerColor) || piece[x][y] == Rook(attackerColor))
                return true;
            break;
        }
    }
    for (int direction = 4; direction < 8; ++direction)
    {
        int x = i + deltaX[direction];
        int y = j + deltaY[direction];
        while (!OutOfTable(x, y))
        {
            if (piece[x][y] == blank)
            {
                x += deltaX[direction];
                y += deltaY[direction];
                continue;
            }
            if (piece[x][y] == Queen(attackerColor) || piece[x][y] == Bishop(attackerColor))
                return true;
            break;
        }

        x = i + deltaX[direction];
        y = j + deltaY[direction] * 2;

        if (!OutOfTable(x, y) && piece[x][y] == Knight(attackerColor)) return true;

        x = i + deltaX[direction] * 2;
        y = j + deltaY[direction];

        if (!OutOfTable(x, y) && piece[x][y] == Knight(attackerColor)) return true;
    }
    for (int direction = 0; direction < 8; ++direction)
    {
        int x = i + deltaX[direction];
        int y = j + deltaY[direction];

        if (!OutOfTable(x, y) && piece[x][y] == King(attackerColor)) return true;
    }

    int dx = (attackerColor == 0 ? -1 : +1);
    int x = i - dx, y = j;

    if (!OutOfTable(x, y - 1) && piece[x][y - 1] == Pawn(attackerColor)) return true;
    if (!OutOfTable(x, y + 1) && piece[x][y + 1] == Pawn(attackerColor)) return true;

    return false;
}

std::vector<std::pair<int,int>> MainWindow::GetMoves(int i, int j)
{
    std::vector<std::pair<int,int>> moves;

    if (piece[i][j] == white_pawn)
    {
        if (!OutOfTable(i - 1, j) && piece[i - 1][j] == blank)
            moves.push_back(std::make_pair(i - 1, j));
        if (!OutOfTable(i - 1, j - 1) && IsBlack(piece[i - 1][j - 1]))
            moves.push_back(std::make_pair(i - 1, j - 1));
        if (!OutOfTable(i - 1, j + 1) && IsBlack(piece[i - 1][j + 1]))
            moves.push_back(std::make_pair(i - 1, j + 1));

        if (i == 6 && piece[i - 1][j] == blank && piece[i - 2][j] == blank)
            moves.push_back(std::make_pair(i - 2, j));
    }
    if (piece[i][j] == black_pawn)
    {
        if (!OutOfTable(i + 1, j) && piece[i + 1][j] == blank)
            moves.push_back(std::make_pair(i + 1, j));
        if (!OutOfTable(i + 1, j - 1) && IsWhite(piece[i + 1][j - 1]))
            moves.push_back(std::make_pair(i + 1, j - 1));
        if (!OutOfTable(i + 1, j + 1) && IsWhite(piece[i + 1][j + 1]))
            moves.push_back(std::make_pair(i + 1, j + 1));

        if (i == 1 && piece[i + 1][j] == blank && piece[i + 2][j] == blank)
            moves.push_back(std::make_pair(i + 2, j));
    }
    if (piece[i][j] == white_rook || piece[i][j] == black_rook)
    {
        for (int direction = 0; direction < 4; ++direction)
        {
            int x = i + deltaX[direction];
            int y = j + deltaY[direction];
            while (!OutOfTable(x, y))
            {
                if (piece[x][y] == blank)
                    moves.push_back(std::make_pair(x, y));
                else
                {
                    if (IsBlack(piece[x][y]) != IsBlack(piece[i][j]))
                        moves.push_back(std::make_pair(x, y));
                    break;
                }
                x += deltaX[direction];
                y += deltaY[direction];
            }
        }
    }
    if (piece[i][j] == white_bishop || piece[i][j] == black_bishop)
    {
        for (int direction = 4; direction < 8; ++direction)
        {
            int x = i + deltaX[direction];
            int y = j + deltaY[direction];
            while (!OutOfTable(x, y))
            {
                if (piece[x][y] == blank)
                    moves.push_back(std::make_pair(x, y));
                else
                {
                    if (IsBlack(piece[x][y]) != IsBlack(piece[i][j]))
                        moves.push_back(std::make_pair(x, y));
                    break;
                }
                x += deltaX[direction];
                y += deltaY[direction];
            }
        }
    }
    if (piece[i][j] == white_queen || piece[i][j] == black_queen)
    {
        for (int direction = 0; direction < 8; ++direction)
        {
            int x = i + deltaX[direction];
            int y = j + deltaY[direction];
            while (!OutOfTable(x, y))
            {
                if (piece[x][y] == blank)
                    moves.push_back(std::make_pair(x, y));
                else
                {
                    if (IsBlack(piece[x][y]) != IsBlack(piece[i][j]))
                        moves.push_back(std::make_pair(x, y));
                    break;
                }
                x += deltaX[direction];
                y += deltaY[direction];
            }
        }
    }
    if (piece[i][j] == white_knight || piece[i][j] == black_knight)
    {
        for (int direction = 4; direction < 8; ++direction)
        {
            int x = i + 2 * deltaX[direction];
            int y = j + deltaY[direction];
            if (!OutOfTable(x, y) && (piece[x][y] == blank || IsWhite(piece[x][y]) != IsWhite(piece[i][j])))
                moves.push_back(std::make_pair(x, y));
        }
        for (int direction = 4; direction < 8; ++direction)
        {
            int x = i + deltaX[direction];
            int y = j + 2 * deltaY[direction];
            if (!OutOfTable(x, y) && (piece[x][y] == blank || IsWhite(piece[x][y]) != IsWhite(piece[i][j])))
                moves.push_back(std::make_pair(x, y));
        }
    }
    if (piece[i][j] == white_king || piece[i][j] == black_king)
    {
        for (int direction = 0; direction < 8; ++direction)
        {
            int x = i + deltaX[direction];
            int y = j + deltaY[direction];

            if (OutOfTable(x, y)) continue;

            if (piece[x][y] == blank)
                moves.push_back(std::make_pair(x, y));
            else
            {
                if (IsWhite(piece[x][y]) != IsWhite(piece[i][j]))
                    moves.push_back(std::make_pair(x, y));
            }
        }
    }
    if (piece[i][j] == white_king && !WKingMoved && !OOWRookMoved)
    {
        std::map<std::pair<int,int>,bool> attackedSquares = GetAllAttackedSquares(1);
        if (!attackedSquares[std::make_pair(7, 4)] &&
            !attackedSquares[std::make_pair(7, 5)] &&
            !attackedSquares[std::make_pair(7, 6)] &&
            piece[7][5] == blank && piece[7][6] == blank && piece[7][7] == white_rook)
            moves.push_back(std::make_pair(7, 6));
    }
    if (piece[i][j] == white_king && !WKingMoved && !OOOWRookMoved)
    {
        std::map<std::pair<int,int>,bool> attackedSquares = GetAllAttackedSquares(1);
        if (!attackedSquares[std::make_pair(7, 4)] &&
            !attackedSquares[std::make_pair(7, 3)] &&
            !attackedSquares[std::make_pair(7, 2)] &&
            piece[7][3] == blank && piece[7][2] == blank && piece[7][0] == white_rook)
            moves.push_back(std::make_pair(7, 2));
    }
    if (piece[i][j] == black_king && !BKingMoved && !OOBRookMoved)
    {
        std::map<std::pair<int,int>,bool> attackedSquares = GetAllAttackedSquares(0);
        if (!attackedSquares[std::make_pair(0, 4)] &&
            !attackedSquares[std::make_pair(0, 5)] &&
            !attackedSquares[std::make_pair(0, 6)] &&
            piece[0][5] == blank && piece[0][6] == blank && piece[0][7] == black_rook)
            moves.push_back(std::make_pair(0, 6));
    }
    if (piece[i][j] == black_king && !BKingMoved && !OOOBRookMoved)
    {
        std::map<std::pair<int,int>,bool> attackedSquares = GetAllAttackedSquares(0);
        if (!attackedSquares[std::make_pair(0, 4)] &&
            !attackedSquares[std::make_pair(0, 3)] &&
            !attackedSquares[std::make_pair(0, 2)] &&
            piece[0][3] == blank && piece[0][2] == blank && piece[0][0] == black_rook)
            moves.push_back(std::make_pair(0, 2));
    }
    return moves;
}

std::vector<std::pair<int,int>> MainWindow::GetAttackedSquares(int i, int j)
{
    std::vector<std::pair<int,int>> moves;
    if (piece[i][j] == white_pawn)
    {
        if (!OutOfTable(i - 1, j - 1))
            moves.push_back(std::make_pair(i - 1, j - 1));
        if (!OutOfTable(i - 1, j + 1))
            moves.push_back(std::make_pair(i - 1, j + 1));

    }
    if (piece[i][j] == black_pawn)
    {
        if (!OutOfTable(i + 1, j - 1))
            moves.push_back(std::make_pair(i + 1, j - 1));
        if (!OutOfTable(i + 1, j + 1))
            moves.push_back(std::make_pair(i + 1, j + 1));
    }
    if (piece[i][j] == white_rook || piece[i][j] == black_rook)
    {
        for (int direction = 0; direction < 4; ++direction)
        {
            int x = i + deltaX[direction];
            int y = j + deltaY[direction];
            while (!OutOfTable(x, y))
            {
                if (piece[x][y] == blank)
                    moves.push_back(std::make_pair(x, y));
                else
                {
                    moves.push_back(std::make_pair(x, y));
                    break;
                }
                x += deltaX[direction];
                y += deltaY[direction];
            }
        }
    }
    if (piece[i][j] == white_bishop || piece[i][j] == black_bishop)
    {
        for (int direction = 4; direction < 8; ++direction)
        {
            int x = i + deltaX[direction];
            int y = j + deltaY[direction];
            while (!OutOfTable(x, y))
            {
                if (piece[x][y] == blank)
                    moves.push_back(std::make_pair(x, y));
                else
                {
                    moves.push_back(std::make_pair(x, y));
                    break;
                }
                x += deltaX[direction];
                y += deltaY[direction];
            }
        }
    }
    if (piece[i][j] == white_queen || piece[i][j] == black_queen)
    {
        for (int direction = 0; direction < 8; ++direction)
        {
            int x = i + deltaX[direction];
            int y = j + deltaY[direction];
            while (!OutOfTable(x, y))
            {
                if (piece[x][y] == blank)
                    moves.push_back(std::make_pair(x, y));
                else
                {
                    moves.push_back(std::make_pair(x, y));
                    break;
                }
                x += deltaX[direction];
                y += deltaY[direction];
            }
        }
    }
    if (piece[i][j] == white_knight || piece[i][j] == black_knight)
    {
        for (int direction = 4; direction < 8; ++direction)
        {
            int x = i + 2 * deltaX[direction];
            int y = j + deltaY[direction];
            if (!OutOfTable(x, y))
                moves.push_back(std::make_pair(x, y));
        }
        for (int direction = 4; direction < 8; ++direction)
        {
            int x = i + deltaX[direction];
            int y = j + 2 * deltaY[direction];
            if (!OutOfTable(x, y))
                moves.push_back(std::make_pair(x, y));
        }
    }
    if (piece[i][j] == white_king || piece[i][j] == black_king)
    {
        for (int direction = 0; direction < 8; ++direction)
        {
            int x = i + deltaX[direction];
            int y = j + deltaY[direction];

            if (!OutOfTable(x, y))
                moves.push_back(std::make_pair(x, y));
        }
    }
    return moves;
}
