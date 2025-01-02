#ifndef TETRIS_TETROMINO_H
#define TETRIS_TETROMINO_H

#include "tetris_types.h"

// John Wesley Thompson
// Created: 8/10/2024
// Last Edited: 12/30/2024
// tetris_tetromino.h

// DATA =======================================================================

// The following tetromino rotations follow the NES tetris rules.
extern const int L_TETROMINO_ARR[3][12];
extern const int J_TETROMINO_ARR[3][12];
extern const int T_TETROMINO_ARR[3][12];
extern const int I_TETROMINO_ARR[4][16]; 
extern const int S_TETROMINO_ARR[3][12];
extern const int Z_TETROMINO_ARR[3][12];
extern const int O_TETROMINO_ARR[3][12];


const int POSSIBLE_ROTATIONS = 4; // This doesn't apply to the O_TETROMINO

// TETROMINO CLASS ================================================================

class tetromino {

public:

    // Constructors and Destructors
    tetromino(tetromino_type);
    tetromino(const tetromino &);
    ~tetromino();

    // Operators
    tetromino& operator = (const tetromino&);

    // Functions
    void rotate(direction);
    int shapeAt(int, int);
    void resetRotation();

    int current_rotation;  // this holds the x index of the top left corner of 
                           // the current rotation.

    int sstride; // the length of each section of the shape array. sstride is shape stride
    int salength; // the length of one row of the entire shape array

    int topmost_sqr,
        bottommost_sqr,
        leftmost_sqr,
        rightmost_sqr;

    tetromino_type type;
    color_id color;    

private:

    int** shape_arr; // the tetromino's shape array
    
    void calcEdges();
    // original for 2D arrays
    template<int rows, int cols>
    void setShapeArray(const int(&)[rows][cols]);
    // overload for int**
    void setShapeArray(int**, int, int);
};

#endif