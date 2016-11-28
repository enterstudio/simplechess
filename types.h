// Author: Simone Madeo

#ifndef __CHESS_TYPES_H
#define __CHESS_TYPES_H

#include <cstdio>
#include <cstring>
#include <vector>
#include <iostream>
#include <algorithm>

using namespace std;

const int BOARD_DIM = 4; // Chessboard size is BOARD_DIM x BOARD_DIM 

/* Square types */
enum square_type {EMPTY = 0, WHITE, BLACK};


/* Coordinations */
struct Coord
{
    int r;
    int c;
    
    Coord() {r = 0, c = 0;}
    Coord(int _r, int _c) {r = _r, c = _c;}
};


/* Pieces */
struct Piece
{
    char type;
    Coord pos;
    bool active;
    
    Piece(char _t, int _r, int _c, bool _a = true)
        {type = _t; pos.r = _r; pos.c = _c; active = _a;}
};


/* Squares */
struct Square
{
    square_type color;
    int id;
    
    Square() {color = EMPTY; id = -1;}
    Square(square_type _color, int _id) {color = _color; id = _id;}
};
    

/* Boards */
class Board
{   
    square_type color[BOARD_DIM][BOARD_DIM];
    int id[BOARD_DIM][BOARD_DIM];                   // 'owner' of the square

    public:
        Board() {
            for (int i = 0; i < BOARD_DIM; i++)
                for (int j = 0; j < BOARD_DIM; j++) {
                    color[i][j] = EMPTY;
                    id[i][j] = -1;
                }
        }
        void set(int r, int c, int x, square_type col) {
            id[r][c] = x;
            color[r][c] = col;
        }
        int getID(int r, int c) {
            return id[r][c];
        }
        square_type getColor(int r, int c) {
            return color[r][c];
        }
        void print(vector<Piece> &A, vector<Piece> &B)
        {
            char output[BOARD_DIM][BOARD_DIM];
            
            for (int a1 = 0; a1 < BOARD_DIM; a1++)
                for (int a2 = 0; a2 < BOARD_DIM; a2++)
                    output[a1][a2] = '.';
                
            /* White pieces are printed in upper case */
            for (int i = 0; i < A.size(); i++)
                if (A[i].active)
                    output[A[i].pos.r][A[i].pos.c] = (color[A[i].pos.r][A[i].pos.c] == WHITE) ?
                        A[i].type : tolower(A[i].type);
            for (int i = 0; i < B.size(); i++)
                if (B[i].active)
                    output[B[i].pos.r][B[i].pos.c] = (color[B[i].pos.r][B[i].pos.c] == WHITE) ?
                        B[i].type : tolower(B[i].type);
            
            for (int a1 = 0; a1 < BOARD_DIM; a1++) {
                for (int a2 = 0; a2 < BOARD_DIM; a2++)
                    cout << output[a1][a2] << " ";
                cout << endl;
            }
            cout << endl;
        }
};

#endif // __CHESS_TYPES_H
