#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QPushButton>
#include <QGridLayout>
#include <QLabel>
#include <vector>

class MainWindow : public QWidget
{
    Q_OBJECT
public:
    const int deltaX[8] = {+1, 0, -1, 0, 1, 1, -1, -1};
    const int deltaY[8] = {0, +1, 0, -1, 1, -1, 1, -1};

    enum ChessPiece {
        blank           = 0,
        white_rook      = 1,
        white_knight    = 2,
        white_bishop    = 3,
        white_queen     = 4,
        white_king      = 5,
        white_pawn      = 6,
        black_rook      = 7,
        black_knight    = 8,
        black_bishop    = 9,
        black_queen     = 10,
        black_king      = 11,
        black_pawn      = 12
    };
    const int value[13] = {0, 5, 3, 3, 9, 0, 1, -5, -3, -3, -9, 0, 1};
    struct ChessMove {
        int ii, ij, fi, fj;
        ChessMove(int ii, int ij, int fi, int fj) : ii{ii}, ij{ij}, fi{fi}, fj{fj} {}
        ChessMove() {}
    };

    bool IsBlack(const ChessPiece& currentPiece)
        { return (7 <= currentPiece && currentPiece <= 12); }
    bool IsWhite(const ChessPiece& currentPiece)
        { return (1 <= currentPiece && currentPiece <= 6); }
    bool GetColor(const ChessPiece& currentPiece)
        { return IsBlack(currentPiece); }

    ChessPiece Rook(const bool& color)
        { return (!color ? white_rook : black_rook);}
    ChessPiece Knight(const bool& color)
        { return (!color ? white_knight : black_knight);}
    ChessPiece Bishop(const bool& color)
        { return (!color ? white_bishop : black_bishop);}
    ChessPiece King(const bool& color)
        { return (!color ? white_king : black_king);}
    ChessPiece Queen(const bool& color)
        { return (!color ? white_queen : black_queen);}
    ChessPiece Pawn(const bool& color)
        { return (!color ? white_pawn : black_pawn);}

    explicit MainWindow(QWidget *parent = nullptr);
    void CreateGUI();
    void ResetChessboard();
    void SetPiece(const int&, const int&, const ChessPiece&, bool);
    bool SquareType(const int&, const int&);
    bool OutOfTable(const int&, const int&);
    std::vector<std::pair<int,int>> GetLegalMoves(int, int);
    std::vector<std::pair<int,int>> GetMoves(int, int);
    std::vector<std::pair<int,int>> GetAttackedSquares(int, int);
    std::map<std::pair<int,int>,bool> GetAllAttackedSquares(bool);
    void CreateConections();
    void HighlightMoves(int, int);
    void UnhighlightMoves();
    void OnSquareClicked(int, int );
    void Move(int, int, int, int, bool);
    bool LegalPosition(bool);
    void CheckForMate(bool);
    int CalculatePosition();
    ChessMove GetBestMove(bool);
    bool Mate(bool);
    void CheckForDraw(bool);
    bool Draw(bool);
    void ComputerMove();
    bool SquareAttacked(int, int, bool);
signals:

private:
    ChessPiece piece[8][8];
    QPushButton* chessboard[8][8];

    int hi_i = -1;
    int hi_j = -1;

    bool OOWRookMoved = false;
    bool OOOWRookMoved = false;
    bool OOBRookMoved = false;
    bool OOOBRookMoved = false;
    bool WKingMoved = false;
    bool BKingMoved = false;

    const QString LIGHT_SQUARE = "background-color: white; border: 0px solid black";
    const QString DARK_SQUARE = "background-color: #2fad3d; border: 0px solid black";
    const QString H_LIGHT_SQUARE  = "background-color: #ed3939; border: 0px solid black";;
    const QString H_DARK_SQUARE = "background-color: #f02222; border: 0px solid black";;
};
#endif // MAINWINDOW_H
