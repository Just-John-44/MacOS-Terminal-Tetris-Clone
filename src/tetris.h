// tetris.h
#ifndef _TETROMINOS_H
#define _TETROMINOS_H

#include <ncurses.h>
#include <ctime>
#include <vector>

// DATA =======================================================================


#define COLOR_PURPLE 8  // All other colors used are defined in the ncurses 
#define COLOR_ORANGE 9  // header file.

enum tetromino_type {
    L_TETROMINO = 1,
    J_TETROMINO,
    T_TETROMINO,
    I_TETROMINO,
    S_TETROMINO,
    Z_TETROMINO,
    O_TETROMINO
};

enum direction {
    UP,
    DOWN,
    LEFT,
    RIGHT, 
    CWISE,
    CCWISE
};

enum tetromino_color_id{
    STANDARD = 1,
    ORANGE,
    BLUE,
    PURPLE,
    CYAN,
    GREEN,
    RED,
    YELLOW
};

// The following tetromino rotations follow the NES tetris rules.
const int L_TETROMINO_ARR[36] = 
{
    0, 0, 1,  0, 1, 0,  0, 0, 0,  2, 1, 0,  // [ ]
    2, 2, 2,  0, 1, 0,  1, 2, 2,  0, 1, 0,  // [ ]		[ ][_][_]
    0, 0, 0,  0, 2, 2,  2, 0, 0,  0, 2, 0   // [_][_]	[_]
};
const int J_TETROMINO_ARR[36] = 
{
    1, 0, 0,  0, 1, 2,  0, 0, 0,  0, 1, 0,  // [ ][ ]
    2, 2, 2,  0, 1, 0,  2, 2, 1,  0, 1, 0,  // [ ]		[_][_][ ]
    0, 0, 0,  0, 2, 0,  0, 0, 2,  2, 2, 0   // [_]   	      [_]
};
const int T_TETROMINO_ARR[36] = 
{
    0, 1, 0,  0, 1, 0,  0, 0, 0,  0, 1, 0,  // [ ]		
    2, 2, 2,  0, 1, 2,  2, 1, 2,  2, 1, 0,  // [ ][_]	   [ ]
    0, 0, 0,  0, 2, 0,  0, 2, 0,  0, 2, 0   // [_]		[_][_][_]
};
const int I_TETROMINO_ARR[64] = 
{ 
    0, 0, 0, 0,  0, 0, 1, 0,  0, 0, 0, 0,  0, 0, 1, 0,  // [ ]	
    0, 0, 0, 0,  0, 0, 1, 0,  0, 0, 0, 0,  0, 0, 1, 0,  // [ ]	
    2, 2, 2, 2,  0, 0, 1, 0,  2, 2, 2, 2,  0, 0, 1, 0,  // [ ]	[_][_][_][_]
    0, 0, 0, 0,  0, 0, 2, 0,  0, 0, 0, 0,  0, 0, 2, 0   // [_]	
}; 
const int S_TETROMINO_ARR[36] = 
{
    0, 0, 0,  0, 1, 0,  0, 0, 0,  0, 1, 0,  // [ ]		
    0, 1, 2,  0, 2, 1,  0, 1, 2,  0, 2, 1,  // [_][ ]	   [ ][_]
    2, 2, 0,  0, 0, 2,  2, 2, 0,  0, 0, 2   //    [_]	[_][_]	
};
const int Z_TETROMINO_ARR[36] = 
{
    0, 0, 0,  0, 0, 1,  0, 0, 0,  0, 0, 1,  //    [ ]	
    2, 1, 0,  0, 1, 2,  2, 1, 0,  0, 1, 2,  // [ ][_]	[_][ ]
    0, 2, 2,  0, 2, 0,  0, 2, 2,  0, 2, 0,  // [_]	 	   [_][_]
};
const int O_TETROMINO_ARR[36] = 
{
    0, 1, 1,  0, 1, 1,  0, 1, 1,  0, 1, 1,  // [ ][ ]	[ ][ ] 
    0, 2, 2,  0, 2, 2,  0, 2, 2,  0, 2, 2,  // [_][_]	[_][_] 
    0, 0, 0,  0, 0, 0,  0, 0, 0,  0, 0, 0,  //                
};

const int POSSIBLE_ROTATIONS = 4; // This doesn't apply to the O_TETROMINO

// ============================================================================

inline void tetris_init(){
    unsigned int generator_seed = time(0);
    srand(generator_seed);

    curs_set(0);

    if (has_colors()){
        start_color();

        if (can_change_color()){
            
            init_color(COLOR_PURPLE, 626, 125, 940);
            init_color(COLOR_ORANGE, 999, 646, 0);     
            
            init_pair(PURPLE, COLOR_PURPLE, COLOR_BLACK);
            init_pair(ORANGE, COLOR_ORANGE, COLOR_BLACK);
        } 

        init_pair(STANDARD, COLOR_WHITE, COLOR_BLACK);
        init_pair(BLUE, COLOR_BLUE, COLOR_BLACK);
        init_pair(CYAN, COLOR_CYAN, COLOR_BLACK);
        init_pair(GREEN, COLOR_GREEN, COLOR_BLACK);
        init_pair(RED, COLOR_RED, COLOR_BLACK);
        init_pair(YELLOW, COLOR_YELLOW, COLOR_BLACK);

        bkgd(COLOR_PAIR(STANDARD));
    }
}

// TETROMINO CLASS ================================================================

class tetromino {
    private:

    int m_color_id;
    int m_shape_stride; // the distance from one rotation to another in the tetromino shape array
    const int* m_shape_arr_ptr; // the tetromino's shape array

    public:

    tetromino(tetromino_type);
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

    tetromino* curr_tetromino;
    tetromino* next_tetromino;
    const int* m_rotation_offset;
    int highest_square_offset;
    int lowest_square_offset;
    int leftmost_square_offset;
    int rightmost_square_offset;
    std::vector<int> grid;
    // This is the index at the bottom left of the tetromino thats in the grid
    // int grid_tetromino_location
    
    int grid_pos;        
    int l_collisions,       
        r_collisions,       
        t_collisions,       
        b_collisions,       
        c_collision_y,      
        c_collision_x;      
    bool c_collision;       
    
    tetromino tetris_tetrominoes[7] = 
    {
        tetromino(L_TETROMINO),
        tetromino(J_TETROMINO),
        tetromino(T_TETROMINO),
        tetromino(I_TETROMINO),
        tetromino(S_TETROMINO),
        tetromino(Z_TETROMINO),
        tetromino(O_TETROMINO)
    };

    tetris_grid();
    void printInfo(WINDOW*);
    void printGrid(WINDOW*);
    void refreshTetromino(WINDOW*);
    void rotateTetromino(direction);
    void generateNextTetromino();
    void calcTopandBottomSquare(const int*);
    void calcLeftandRightSquare(const int*);
    void setCurrTetrominoOnGrid();
    void shiftTetromino(int, direction);
    void settleCurrTetromino(); 

private:
    void gridMoveCursor(WINDOW*, int, int);
    bool canRotate(const int*);
};

#endif