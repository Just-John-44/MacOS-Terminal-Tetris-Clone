// tetris.h
#ifndef _BLOCKS_H
#define _BLOCKS_H

#include <ncurses.h>
#include <random>
#include <ctime>

// DATA =======================================================================

enum block_type {
    L_BLOCK = 1,
    J_BLOCK,
    T_BLOCK,
    I_BLOCK,
    S_BLOCK,
    Z_BLOCK,
    O_BLOCK
};

const int L_BLOCK_ARR[36]
{
    0, 0, 1,  0, 1, 0,  0, 0, 0,  1, 1, 0,  // [ ]
    1, 1, 1,  0, 1, 0,  1, 1, 1,  0, 1, 0,  // [ ]		[ ][_][_]
    0, 0, 0,  0, 1, 1,  1, 0, 0,  0, 1, 0   // [_][_]	[_]
};
const int J_BLOCK_ARR[36]
{
    1, 0, 0,  0, 1, 1,  0, 0, 0,  0, 1, 0,  // [ ][ ]
    1, 1, 1,  0, 1, 0,  1, 1, 1,  0, 1, 0,  // [ ]		[_][_][ ]
    0, 0, 0,  0, 1, 0,  0, 0, 1,  1, 1, 0   // [_]   	      [_]
};
const int T_BLOCK_ARR[36]
{
    0, 1, 0,  0, 1, 0,  0, 0, 0,  0, 1, 0,  // [ ]		
    1, 1, 1,  0, 1, 1,  1, 1, 1,  1, 1, 0,  // [ ][_]	   [ ]
    0, 0, 0,  0, 1, 0,  0, 1, 0,  0, 1, 0   // [_]		[_][_][_]
};
const int I_BLOCK_ARR[64]
{
    0, 0, 0, 0,  0, 0, 1, 0,  0, 0, 0, 0,  0, 1, 0, 0,  // [ ]	
    1, 1, 1, 1,  0, 0, 1, 0,  0, 0, 0, 0,  0, 1, 0, 0,  // [ ]	
    0, 0, 0, 0,  0, 0, 1, 0,  1, 1, 1, 1,  0, 1, 0, 0,  // [ ]	
    0, 0, 0, 0,  0, 0, 1, 0,  0, 0, 0, 0,  0, 1, 0, 0   // [_]	[_][_][_][_]
}; 
const int S_BLOCK_ARR[36]
{
    0, 1, 1,  0, 1, 0,  0, 0, 0,  1, 0, 0,  // [ ]		
    1, 1, 0,  0, 1, 1,  0, 1, 1,  1, 1, 0,  // [_][ ]	   [ ][_]
    0, 0, 0,  0, 0, 1,  1, 1, 0,  0, 1, 0   //    [_]	[_][_]	
};
const int Z_BLOCK_ARR[36]
{
    1, 1, 0,  0, 0, 1,  0, 0, 0,  0, 1, 0,  //    [ ]	
    0, 1, 1,  0, 1, 1,  1, 1, 0,  1, 1, 0,  // [ ][_]	[_][ ]
    0, 0, 0,  0, 1, 0,  0, 1, 1,  1, 0, 0   // [_]	 	   [_][_]
};
const int O_BLOCK_ARR[9]
{
    1, 1, 0,    // [ ][ ]	[ ][ ]  // The O block doesnt change when rotated,
    1, 1, 0,    // [_][_]	[_][_]  // so there is no need to have multiple 
    0, 0, 0     //                  // rotations stored.
};

const int POSSIBLE_ROTATIONS = 4; // This doesn't apply to the O_BLOCK

// BLOCK CLASS ================================================================

class block {
    private:

    int m_color;
    int m_shape_stride; // the distance from one rotation to another in the block shape array
    const int* m_shape_arr_ptr; // the block's shape array

    public:

    block(block_type);
    const int* shapeArray();
    int shapeStride();
    int color();
};

// TETRIS_GRID CLASS ==========================================================

class tetris_grid {
    private:
    public:
    block* curr_block;
    block* next_block;
    const int* rotation_offset;
    block tetris_blocks[7] 
    {
        block(L_BLOCK),
        block(J_BLOCK),
        block(T_BLOCK),
        block(I_BLOCK),
        block(S_BLOCK),
        block(Z_BLOCK),
        block(O_BLOCK)
    };

    tetris_grid();
    void placeBlock();
    void printGrid();
    void rotateR(block*, const int* &);
    void rotateL(block*, const int* &);
    void generateNextBlock();
};

#endif