#include "mainwindow.h"
#include <QGridLayout>
#include <QDebug>
#include <QPixmap>
#include <QLabel>
#include <QRect>
#include <QString>
#include <QPushButton>
#include <cmath>
#include <QMessageBox>
#include <future>

bool MainWindow::SquareType(const int& i, const int& j)
{
    return (i + j) % 2;
}

bool MainWindow::OutOfTable(const int& i, const int& j)
{
    return (i > 7 || j > 7 || i < 0 || j < 0);
}

void MainWindow::SetPiece(const int& i, const int& j, const ChessPiece& currentPiece, bool realReplacement)
{
    if (realReplacement)
    {
        QString imagePath = ":/old_pieces/" + QString::number(currentPiece) + ".png";
        QPixmap pixmap(imagePath);
        QIcon ButtonIcon(pixmap);
        chessboard[i][j]->setIcon(ButtonIcon);
        chessboard[i][j]->setIconSize(QSize(80, 80));
    }
    piece[i][j] = currentPiece;
}

void MainWindow::ResetChessboard()
{
    SetPiece(0, 0, black_rook, true);
    SetPiece(0, 1, black_knight, true);
    SetPiece(0, 2, black_bishop, true);
    SetPiece(0, 3, black_queen, true);
    SetPiece(0, 4, black_king, true);
    SetPiece(0, 5, black_bishop, true);
    SetPiece(0, 6, black_knight, true);
    SetPiece(0, 7, black_rook, true);

    SetPiece(7, 0, white_rook, true);
    SetPiece(7, 1, white_knight, true);
    SetPiece(7, 2, white_bishop, true);
    SetPiece(7, 3, white_queen, true);
    SetPiece(7, 4, white_king, true);
    SetPiece(7, 5, white_bishop, true);
    SetPiece(7, 6, white_knight, true);
    SetPiece(7, 7, white_rook, true);

    for (int i = 0; i < 8; ++i)
    {
        SetPiece(1, i, black_pawn, true);
        SetPiece(6, i, white_pawn, true);
        for (int j = 2; j <= 5; ++j)
            SetPiece(j, i, blank, true);
    }

    hi_i = -1;
    hi_j = -1;

    OOWRookMoved = false;
    OOOWRookMoved = false;
    OOBRookMoved = false;
    OOOBRookMoved = false;
    WKingMoved = false;
    BKingMoved = false;
}

void MainWindow::CreateGUI()
{
    QGridLayout* grid = new QGridLayout;
    grid->setSpacing(0);
    for (int i = 0; i < 8; ++i)
    {
        for (int j = 0; j < 8; ++j)
        {
            chessboard[i][j] = new QPushButton();
            chessboard[i][j]->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Preferred);

            if (SquareType(i, j) == 0)
                 chessboard[i][j]->setStyleSheet(LIGHT_SQUARE);
            else chessboard[i][j]->setStyleSheet(DARK_SQUARE);
            grid->addWidget(chessboard[i][j], i, j);
        }
    }
    setLayout(grid);
}

void MainWindow::HighlightMoves(int i, int j)
{
    UnhighlightMoves();
    std::vector<std::pair<int,int>> moves = GetLegalMoves(i, j);

    if (SquareType(i, j) == 0)
        chessboard[i][j]->setStyleSheet(H_LIGHT_SQUARE);
    else chessboard[i][j]->setStyleSheet(H_DARK_SQUARE);

    for (const auto& curr : moves)
    {
        int i = curr.first;
        int j = curr.second;
        if (SquareType(i, j) == 0)
            chessboard[i][j]->setStyleSheet(H_LIGHT_SQUARE);
        else chessboard[i][j]->setStyleSheet(H_DARK_SQUARE);
    }
    hi_i = i;
    hi_j = j;
}

void MainWindow::UnhighlightMoves()
{
    hi_i = -1;
    hi_j = -1;
    for (int i = 0; i < 8; ++i)
        for (int j = 0; j < 8; ++j)
            if (SquareType(i, j) == 0)
                 chessboard[i][j]->setStyleSheet(LIGHT_SQUARE);
            else chessboard[i][j]->setStyleSheet(DARK_SQUARE);
}

void MainWindow::Move(int ii, int ij, int fi, int fj, bool realMove)
{
    if (realMove)
    {
        if (std::make_pair(ii, ij) == std::make_pair(7, 4))
            WKingMoved = true;
        if (std::make_pair(ii, ij) == std::make_pair(7, 7))
            OOWRookMoved = true;
        if (std::make_pair(ii, ij) == std::make_pair(7, 0))
            OOOWRookMoved = true;
        if (std::make_pair(ii, ij) == std::make_pair(0, 4))
            BKingMoved = true;
        if (std::make_pair(ii, ij) == std::make_pair(0, 7))
            OOBRookMoved = true;
        if (std::make_pair(ii, ij) == std::make_pair(0, 0))
            OOOBRookMoved = true;

        qDebug() << "moved" << piece[ii][ij] << fi << fj;
    }
    if (piece[ii][ij] == white_king && fj - ij == 2)
    {
        SetPiece(7, 4, blank, realMove);
        SetPiece(7, 5, white_rook, realMove);
        SetPiece(7, 6, white_king, realMove);
        SetPiece(7, 7, blank, realMove);
        return;
    }
    if (piece[ii][ij] == white_king && fj - ij == -2)
    {
        SetPiece(7, 0, blank, realMove);
        SetPiece(7, 1, blank, realMove);
        SetPiece(7, 2, white_king, realMove);
        SetPiece(7, 3, white_rook, realMove);
        SetPiece(7, 4, blank, realMove);
        return;
    }

    if (piece[ii][ij] == black_king && fj - ij == 2)
    {
        SetPiece(0, 4, blank, realMove);
        SetPiece(0, 5, black_rook, realMove);
        SetPiece(0, 6, black_king, realMove);
        SetPiece(0, 7, blank, realMove);
        return;
    }
    if (piece[ii][ij] == black_king && fj - ij == -2)
    {
        SetPiece(0, 0, blank, realMove);
        SetPiece(0, 1, blank, realMove);
        SetPiece(0, 2, black_king, realMove);
        SetPiece(0, 3, black_rook, realMove);
        SetPiece(0, 4, blank, realMove);
        return;
    }

    if ((piece[ii][ij] == black_pawn && fi == 7))
        piece[ii][ij] = black_queen;

    if ((piece[ii][ij] == white_pawn && fi == 0))
        piece[ii][ij] = white_queen;

    SetPiece(fi, fj, piece[ii][ij], realMove);
    SetPiece(ii, ij, blank, realMove);
}

void MainWindow::OnSquareClicked(int i, int j)
{
    qDebug() << piece[i][j];
    if (std::make_pair(hi_i, hi_j) == std::make_pair(i, j))
    {
        UnhighlightMoves();
        return;
    }
    if (IsWhite(piece[i][j]))
    {
        HighlightMoves(i, j);
        return;
    }
    if (OutOfTable(hi_i, hi_j))
        return;

    std::vector<std::pair<int,int>> moves = GetLegalMoves(hi_i, hi_j);
    bool foundMove = false;
    for (const auto& curr : moves)
        if (curr == std::make_pair(i, j))
        {
            Move(hi_i, hi_j, i, j, true);
            UnhighlightMoves();
            CheckForMate(0);
            CheckForDraw(0);
            foundMove = true;
            break;
        }
    if (!foundMove)
        return;
    if (Mate(0) || Draw(0))
        ResetChessboard();
    else
        ComputerMove();
}

void MainWindow::ComputerMove()
{
    ChessMove blackMove = GetBestMove(1);
    Move(blackMove.ii, blackMove.ij, blackMove.fi, blackMove.fj, true);
    CheckForDraw(1);
    CheckForMate(1);
    if (Mate(1) || Draw(1))
        ResetChessboard();
}

void MainWindow::CheckForMate(bool color)
{
    if (!Mate(color))
        return;

    QString qmbText;
    if (color == 0) qmbText = "White won!!!";
    else qmbText = "Black won!!!";

    QMessageBox::information(this, "Checkmate", qmbText);
}

void MainWindow::CheckForDraw(bool color)
{
    if (!Draw(color))
        return;
    QMessageBox::information(this, "Draw", "It's a draw!!!");
}

bool MainWindow::Mate(bool color)
{
    for (int i = 0; i < 8; ++i)
        for (int j = 0; j < 8; ++j)
            if (IsWhite(piece[i][j]) == color && piece[i][j] != blank)
            {
                std::vector<std::pair<int,int>> moves = GetLegalMoves(i, j);
                if (!moves.empty())
                    return false;
            }
    auto attacked = GetAllAttackedSquares(color);
    for (int i = 0; i < 8; ++i)
        for (int j = 0; j < 8; ++j)
            if (piece[i][j] == (color == 1 ? white_king : black_king))
                return attacked[std::make_pair(i, j)];
    return true;
}

bool MainWindow::Draw(bool color)
{
    int totalValue = 0;
    for (int i = 0; i < 8; ++i)
        for (int j = 0; j < 8; ++j)
            totalValue += std::abs(value[piece[i][j]]);
    if (totalValue == 0)
        return 1;

    for (int i = 0; i < 8; ++i)
        for (int j = 0; j < 8; ++j)
            if (IsWhite(piece[i][j]) == color && piece[i][j] != blank)
            {
                std::vector<std::pair<int,int>> moves = GetLegalMoves(i, j);
                if (!moves.empty())
                    return false;
            }
    auto attacked = GetAllAttackedSquares(color);
    for (int i = 0; i < 8; ++i)
        for (int j = 0; j < 8; ++j)
            if (piece[i][j] == (color == 1 ? white_king : black_king))
                return !attacked[std::make_pair(i, j)];

    return 0;
}

void MainWindow::CreateConections()
{
    for (int i = 0; i < 8; ++i)
        for (int j = 0; j < 8; ++j)
        {
            connect(chessboard[i][j], &QPushButton::clicked, this, [=]{ OnSquareClicked(i, j); });
        }
}

MainWindow::MainWindow(QWidget *parent)
    : QWidget(parent)
{
    CreateGUI();
    ResetChessboard();
    CreateConections();
}


