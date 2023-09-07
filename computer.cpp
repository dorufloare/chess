#include "mainwindow.h"
#include <QGridLayout>
#include <QDebug>
#include <QPixmap>
#include <QLabel>
#include <QRect>
#include <QString>
#include <QPushButton>
#include <map>
#include <QRandomGenerator>

int MainWindow::CalculatePosition()
{
    int totalValue = 0;
    if (Draw(0) || Draw(1))
        return 0;
    if (Mate(0))
        return 100;
    if (Mate(1))
        return -100;

    for (int i = 0; i < 8; ++i)
        for (int j = 0; j < 8; ++j)
            totalValue += value[piece[i][j]];
    return totalValue;
}

MainWindow::ChessMove MainWindow::GetBestMove(bool color)
{
    int bestValue = (color ? 101 : -101);
    std::vector<ChessMove> bestMoves;

    ChessPiece oldPiece[8][8];
    for (int i = 0; i < 8; ++i)
        for (int j = 0; j < 8; ++j)
            oldPiece[i][j] = piece[i][j];

    for (int i = 0; i < 8; ++i)
        for (int j = 0; j < 8; ++j)
        {
            if (IsWhite(piece[i][j]) == color || piece[i][j] == blank)
                continue;

            std::vector<std::pair<int,int>> moves = GetLegalMoves(i, j);

            for (const auto& curr : moves)
            {
                Move(i, j, curr.first, curr.second, false);
                ChessMove wMove;

                if (color)
                {
                    wMove = GetBestMove(0);
                    Move(wMove.ii, wMove.ij, wMove.fi, wMove.fj, false);
                }

                int currValue = CalculatePosition();
                if ((color == 0 ? -1 : 1) * bestValue > (color == 0 ? -1 : 1) * currValue)
                {
                    bestValue = currValue;
                    bestMoves.clear();
                }
                if (bestValue == currValue)
                    bestMoves.push_back(ChessMove(i, j, curr.first, curr.second));

                SetPiece(i, j, oldPiece[i][j], false);
                SetPiece(curr.first, curr.second, oldPiece[curr.first][curr.second], false);

                if (color)
                {
                    SetPiece(wMove.ii, wMove.ij, oldPiece[wMove.ii][wMove.ij], false);
                    SetPiece(wMove.fi, wMove.fj, oldPiece[wMove.fi][wMove.fj], false);
                }
            }
        }

    for (int i = 0; i < 8; ++i)
        for (int j = 0; j < 8; ++j)
            SetPiece(i, j, oldPiece[i][j], true);

    if (bestMoves.empty())
        return ChessMove(0, 0, 0, 0);

    int randIndx = QRandomGenerator::global()->generate() % bestMoves.size();
    return bestMoves[randIndx];
}

