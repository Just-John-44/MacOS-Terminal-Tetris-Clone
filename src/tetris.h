// tetris.h
#ifndef _BLOCKS_H
#define _BLOCKS_H

#include <ncurses.h>
#include <ctime>
#include <vector>

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

enum direction {
    UP,
    DOWN,
    LEFT,
    RIGHT
};


// The following block rotations follow the NES tetris rules.
const int L_BLOCK_ARR[36] = 
{
    0, 0, 1,  0, 1, 0,  0, 0, 0,  2, 1, 0,  // [ ]
    2, 2, 2,  0, 1, 0,  1, 2, 2,  0, 1, 0,  // [ ]		[ ][_][_]
    0, 0, 0,  0, 2, 2,  2, 0, 0,  0, 2, 0   // [_][_]	[_]
};
const int J_BLOCK_ARR[36] = 
{
    1, 0, 0,  0, 1, 2,  0, 0, 0,  0, 1, 0,  // [ ][ ]
    2, 2, 2,  0, 1, 0,  2, 2, 1,  0, 1, 0,  // [ ]		[_][_][ ]
    0, 0, 0,  0, 2, 0,  0, 0, 2,  2, 2, 0   // [_]   	      [_]
};
const int T_BLOCK_ARR[36] = 
{
    0, 1, 0,  0, 1, 0,  0, 0, 0,  0, 1, 0,  // [ ]		
    2, 2, 2,  0, 1, 2,  2, 1, 2,  2, 1, 0,  // [ ][_]	   [ ]
    0, 0, 0,  0, 2, 0,  0, 2, 0,  0, 2, 0   // [_]		[_][_][_]
};
const int I_BLOCK_ARR[64] = 
{ 
    0, 0, 0, 0,  0, 0, 1, 0,  0, 0, 0, 0,  0, 0, 1, 0,  // [ ]	
    0, 0, 0, 0,  0, 0, 1, 0,  0, 0, 0, 0,  0, 0, 1, 0,  // [ ]	
    2, 2, 2, 2,  0, 0, 1, 0,  2, 2, 2, 2,  0, 0, 1, 0,  // [ ]	
    0, 0, 0, 0,  0, 0, 2, 0,  0, 0, 0, 0,  0, 0, 2, 0   // [_]	[_][_][_][_]
}; 
const int S_BLOCK_ARR[36] = 
{
    0, 0, 0,  0, 1, 0,  0, 0, 0,  0, 1, 0,  // [ ]		
    0, 1, 2,  0, 2, 1,  0, 1, 2,  0, 2, 1,  // [_][ ]	   [ ][_]
    2, 2, 0,  0, 0, 2,  2, 2, 0,  0, 0, 2   //    [_]	[_][_]	
};
const int Z_BLOCK_ARR[36] = 
{
    0, 0, 0,  0, 0, 1,  0, 0, 0,  0, 0, 1,  //    [ ]	
    2, 1, 0,  0, 1, 2,  2, 1, 0,  0, 1, 2,  // [ ][_]	[_][ ]
    0, 2, 2,  0, 2, 0,  0, 2, 2,  0, 2, 0,  // [_]	 	   [_][_]
};
const int O_BLOCK_ARR[36] = 
{
    0, 1, 1,  0, 1, 1,  0, 1, 1,  0, 1, 1,  // [ ][ ]	[ ][ ] 
    0, 2, 2,  0, 2, 2,  0, 2, 2,  0, 2, 2,  // [_][_]	[_][_] 
    0, 0, 0,  0, 0, 0,  0, 0, 0,  0, 0, 0,  //                
};

const int POSSIBLE_ROTATIONS = 4; // This doesn't apply to the O_BLOCK

// ============================================================================

inline void tetris_init(){
    unsigned int generator_seed = time(0);
    srand(generator_seed);
}

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
public:
    bool canShiftUp(const int*, int);
    bool canShiftDown(const int*, int);
    bool canShiftLeft(const int*, int);
    bool canShiftRight(const int*, int);

    block* curr_block;
    block* next_block;
    const int* rotation_offset;
    int highest_square_offset;
    int lowest_square_offset;
    int leftmost_square_offset;
    int rightmost_square_offset;
    std::vector<int> grid;
    // This is the index at the bottom left of the block thats in the grid
    // int grid_block_location;
    
    int grid_search;        
    int l_collisions,       
        r_collisions,       
        t_collisions,       
        b_collisions,       
        c_collision_y,      
        c_collision_x;      
    bool c_collision;       
    
    block tetris_blocks[7] = 
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
    void printInfo(WINDOW*);
    void gridMoveCursor(WINDOW*, int, int);
    void printGrid(WINDOW*);
    bool canRotate(const int*);
    void rotateR();
    void rotateL();
    void generateNextBlock();
    void calcTopandBottomSquare(const int*);
    void calcLeftandRightSquare(const int*);
    void setCurrBlockOnGrid();
    
    void shiftBlock(int, direction);
    // void shiftDown(int);
    // void shiftLeft(int);
    // void shiftRight(int);
    void settleCurrBlock(); 

private:
};

#endif