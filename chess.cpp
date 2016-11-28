// Author: Simone Madeo

#include "types.h"

#define FILE_INPUT // Comment this to test the program online

/* Check if knight in (r, c) for color myColor can be played */
inline void checkKnight(Board &board, int r, int c, int myColor, vector<Coord> &x)
{
    if (r < BOARD_DIM && c < BOARD_DIM && r >= 0 && c >= 0 && board.getColor(r, c) != myColor) {
        x.push_back(Coord(r, c));
    }
}

/* Return all the possible moves for piece x */
void enumerate(Board &board, Piece &x, int myColor, int opponentQueenID, vector<Coord> &moves)
{
    int foundCheckmate = -1;
    int i, j;
    int r = x.pos.r;
    int c = x.pos.c;
    char piece = x.type;
    square_type curColor;
    
    if (piece == 'Q' || piece == 'R') {
        /* Horizontal/vertical moves */
        for (i = r + 1, j = c; i < BOARD_DIM; i++) {
            curColor = board.getColor(i, j);
            if (curColor != myColor)
                moves.push_back(Coord(i, j));
            if (curColor != EMPTY)
                break;
        }
        for (i = r, j = c + 1; j < BOARD_DIM; j++) {
            curColor = board.getColor(i, j);
            if (curColor != myColor)
                moves.push_back(Coord(i, j));
            if (curColor != EMPTY)
                break;
        }
        for (i = r - 1, j = c; i >= 0; i--) {
            curColor = board.getColor(i, j);
            if (curColor != myColor)
                moves.push_back(Coord(i, j));
            if (curColor != EMPTY)
                break;
        }
        for (i = r, j = c - 1; j >= 0; j--) {
            curColor = board.getColor(i, j);
            if (curColor != myColor)
                moves.push_back(Coord(i, j));
            if (curColor != EMPTY)
                break;
        }
    }
    if (piece == 'Q' || piece == 'B') {
        /* Diagonal moves */
        for (i = r + 1, j = c + 1; (i < BOARD_DIM && j < BOARD_DIM); i++, j++) {
            curColor = board.getColor(i, j);
            if (curColor != myColor)
                moves.push_back(Coord(i, j));
            if (curColor != EMPTY)
                break;
        }
        for (i = r + 1, j = c - 1; (i < BOARD_DIM && j >= 0); i++, j--) {
            curColor = board.getColor(i, j);
            if (curColor != myColor)
                moves.push_back(Coord(i, j));
            if (curColor != EMPTY)
                break;
        }
        for (i = r - 1, j = c + 1; (i >= 0 && j < BOARD_DIM); i--, j++) {
            curColor = board.getColor(i, j);
            if (curColor != myColor)
                moves.push_back(Coord(i, j));
            if (curColor != EMPTY)
                break;
        }
        for (i = r - 1, j = c - 1; (i >= 0 && j >= 0); i--, j--) {
            curColor = board.getColor(i, j);
            if (curColor != myColor)
                moves.push_back(Coord(i, j));
            if (curColor != EMPTY)
                break;
        }
    }
    if (piece == 'N') {
        /* Knight moves */
        checkKnight(board, r + 2, c + 1, myColor, moves);
        checkKnight(board, r + 2, c - 1, myColor, moves);
        checkKnight(board, r + 1, c - 2, myColor, moves);
        checkKnight(board, r + 1, c + 2, myColor, moves);
        checkKnight(board, r - 2, c + 1, myColor, moves);
        checkKnight(board, r - 2, c - 1, myColor, moves);
        checkKnight(board, r - 1, c + 2, myColor, moves);
        checkKnight(board, r - 1, c - 2, myColor, moves);
    }

    /* Optimization: if the opposite queen can be captured, other combinations
       are discarded */  
    for (int i = 0; i < moves.size() && foundCheckmate == -1; i++)
        if (board.getID(moves[i].r, moves[i].c) == opponentQueenID)
            foundCheckmate = i;
     
    if (foundCheckmate != -1) {
        moves[0] = moves[foundCheckmate];
        moves.resize(1);
    }
}

/* Recursive function controlling the end of the game: return TRUE if white has
   a forced win.
   board: current board status,
   A: player who is moving
   B: the other player
   n: move number
   m: max moves allowed
   pieceID: id of A's moving piece
   r: new row for the moving piece
   c: new column for the moving piece
   queenAID, id of A's queen
   queenBID, id of B's queen
 */
bool moves(Board board, vector<Piece> A, vector<Piece> B, int n, int m,
    int pieceID, int r, int c, int queenAID, int queenBID)
{
    int playerID = (n % 2) + 1;          // 1 (white) or 2 (black)
    int opponentID = ((n + 1) % 2) + 1;  // 1 (white) or 2 (black)
    int old_r = A[pieceID].pos.r;        // previos position of the moving piece
    int old_c = A[pieceID].pos.c;        // previos position of the moving piece
    int takenSquareID = board.getID(r, c);     // new square of the moving piece
    int nB = B.size();
    bool result;
   
    /* Remove captured piece */
    if (takenSquareID != -1) {
        /* Check for checkmate */
        if (B[takenSquareID].type == 'Q') {
            if (playerID == WHITE) {
                /* White moved */
                return true;
            } else {
                /* Black moved */
                return false;
            }        
        }
        B[takenSquareID].active = false;
    }

    /* Check number of moves */
    if (n == m - 1)
        return false;

    /* Update the squares */
    board.set(r, c, pieceID, (square_type)playerID);
    board.set(old_r, old_c, -1, EMPTY);

    /* Update the position of the moving piece */
    A[pieceID].pos.r = r;
    A[pieceID].pos.c = c;

    /* Analyze all opponent's possible moves */
    for (int i = 0; i < nB; i++) {
        if (B[i].active) {
            vector<Coord> nextMove;

            enumerate(board, B[i], opponentID, queenAID, nextMove);

            for (int j = 0; j < nextMove.size(); j++) {
                result = moves(board, B, A, n + 1, m, i, nextMove[j].r,
                    nextMove[j].c, queenBID, queenAID);

                if (opponentID == WHITE && result) {
                    // At least one white combination led to forced checkmate
                    vector<Coord>().swap(nextMove);
                    return true;
                }
                else if (opponentID == BLACK && !result) {
                    // At least one black comb. did not lead to forced checkmate
                    vector<Coord>().swap(nextMove);
                    return false;
                }
            }
            vector<Coord>().swap(nextMove);
        }
    }       

    if (opponentID == BLACK) {
        /* All black combinations led to forced checkmate */
        return true;
    }

    /* At least one white combination did not lead to forced checkmate */
    return false;
}

int main(int argc, char *argv[])
{
    int g, w, b, m, r, r_, c_, wQueenID, bQueenID;
    char c, p;
    bool result;
      
    #ifdef FILE_INPUT
    FILE *fp;
    char tmp[1];
    if (argc != 2) {
        cout << "Usage: chess [textfile]\n";
        exit(1);
    }
    fp = fopen(argv[1], "r");
    fscanf(fp, "%i", &g);
    #else
    cin >> g;
    #endif

    for (int z = 0; z < g; z++) {
        Board board;
        vector<Piece> W, B;    
        result = false;

        #ifdef FILE_INPUT
        fscanf(fp, "%i", &w);
        fscanf(fp, "%i", &b);
        fscanf(fp, "%i", &m);
        #else
        cin >> w >> b >> m;
        #endif

        for (int j = 0; j < w + b; j++) {
            #ifdef FILE_INPUT
            fscanf(fp, "%s", tmp);
            p = tmp[0];
            fscanf(fp, "%s", tmp);
            c = tmp[0];
            fscanf(fp, "%s", tmp);
            r = atoi(tmp);
            #else
            cin >> p >> c >> r;
            #endif
            r_ = BOARD_DIM - r;
            c_ = c - 'A';
            if (j < w) {
                /* Update white pieces */
                W.push_back(Piece(p, r_, c_));            
                board.set(r_, c_, j, WHITE);
                if (p == 'Q')
                    wQueenID = j;
            } else {
                /* Update black pieces */
                B.push_back(Piece(p, r_, c_));            
                board.set(r_, c_, j - w, BLACK);
                if (p == 'Q')
                    bQueenID = j - w;
            }
        }

        // Use 'board.print(W, B);' to print the current checkboard

        /* Check all white's first moves */
        for (int i = 0; i < W.size() && !result; i++) {
            vector<Coord> nextMove;
            enumerate(board, W[i], 1, bQueenID, nextMove);

            for (int j = 0; j < nextMove.size() && !result; j++)
                result = moves(board, W, B, 0, m, i, nextMove[j].r, nextMove[j].c, wQueenID, bQueenID);

            vector<Coord>().swap(nextMove);
        }

        if (result)
            cout << "YES\n";
        else
            cout << "NO\n";
    }
    
    #ifdef FILE_INPUT
    fclose(fp);
    #endif

    return 0;
}

