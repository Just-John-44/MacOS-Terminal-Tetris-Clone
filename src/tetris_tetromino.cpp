// John Wesley Thompson
// Created: 8/10/2024
// Completed:
// Last Edited: 10/17/2024
// tetris_tetromino.h


#include "tetris_tetromino.h"


// DATA =======================================================================

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
    0, 2, 2,  0, 2, 0,  0, 2, 2,  0, 2, 0   // [_]	 	   [_][_]
};
const int O_TETROMINO_ARR[36] = 
{
    0, 1, 1,  0, 1, 1,  0, 1, 1,  0, 1, 1,  // [ ][ ]	[ ][ ] 
    0, 2, 2,  0, 2, 2,  0, 2, 2,  0, 2, 2,  // [_][_]	[_][_] 
    0, 0, 0,  0, 0, 0,  0, 0, 0,  0, 0, 0   //                
};

// ============================================================================
// TETROMINO CLASS 
// ============================================================================

tetromino::tetromino(){
    m_color_id = STANDARD;
    m_shape_stride = 0;
    m_shape_arr_ptr = nullptr;
}

tetromino::tetromino(tetromino_type tet_t){

    switch(tet_t){
        case L_TETROMINO:   
            m_shape_arr_ptr = L_TETROMINO_ARR;
            m_shape_stride = 3;

            m_color_id = ORANGE;
            break;

        case J_TETROMINO: 
            m_shape_arr_ptr = J_TETROMINO_ARR;
            m_shape_stride = 3;
            
            m_color_id = BLUE;
            break;

        case T_TETROMINO: 
            m_shape_arr_ptr = T_TETROMINO_ARR;
            m_shape_stride = 3; 
            
            m_color_id = PURPLE;
            break;

        case I_TETROMINO: 
            m_shape_arr_ptr = I_TETROMINO_ARR;
            m_shape_stride = 4;

            m_color_id = CYAN;
            break;

        case S_TETROMINO: 
            m_shape_arr_ptr = S_TETROMINO_ARR;
            m_shape_stride = 3;

            m_color_id = GREEN;
            break;

        case Z_TETROMINO: 
            m_shape_arr_ptr = Z_TETROMINO_ARR;
            m_shape_stride = 3;

            m_color_id = RED;
            break;

        case O_TETROMINO: 
            m_shape_arr_ptr = O_TETROMINO_ARR;
            m_shape_stride = 3;
            
            m_color_id = YELLOW;
            break;
    }
}

// In the following two functions, the pointer to the array is intentionally
// being set to the same array in memory. That array is unmodifiable and can
// not be deleted. I don't think they're needed, but they I made them just in
// case. 

// Copy constructor
tetromino::tetromino (const tetromino& tet){
    this->m_color_id = tet.m_color_id;
    this->m_shape_stride = tet.m_shape_stride;
    this->m_shape_arr_ptr = tet.m_shape_arr_ptr;
}

// structure assignment
tetromino& tetromino::operator = (const tetromino& rhs){
    this->m_color_id = rhs.m_color_id;
    this->m_shape_stride = rhs.m_shape_stride;
    this->m_shape_arr_ptr = rhs.m_shape_arr_ptr;

    return *this;
}

void tetromino::calcTopandBottomSquare(const int* test_rotation_offset, 
                                       int& top, int& bottom)
{
    // Since the shape array is not 2D, to step through it like it is 2D, there
    // needs to be a multiplier that will get you to the next "row". This case
    // is more complicated because i only want to read a quarter of the array
    // like its 2D, and because I'm working with pointer and not the index
    // operator.
    //   To read from only a section of the array like it's 2D, there needs to
    // be an offset that, once you've made it to the new "row", can be used
    // to put you back on the correct "column".

    int shape_arr_len = m_shape_stride * POSSIBLE_ROTATIONS;

    // Hit is true when a 1 is found in the part of the array that is being searched.
    bool hit = false;
    for (int row = 0; row < m_shape_stride; row++){
        for (int col = 0; col < m_shape_stride; col++){
            if (*(test_rotation_offset + col + row * shape_arr_len) > 0){
                top = row;
                hit = true;
                break;
            }  
        }
        if (hit) break;
    }

    hit = false;
    for (int row = m_shape_stride - 1; row >= 0; row--){
        for (int col = 0; col < m_shape_stride; col++){
            if (*(test_rotation_offset + col + row * shape_arr_len) > 0){
                bottom = row;
                hit = true;
                break;
            }  
        }
        if (hit) break;
    }
}

void tetromino::calcLeftandRightSquare(const int* test_rotation_offset, 
                                       int& left, int& right)
{

    int shape_arr_len = m_shape_stride * POSSIBLE_ROTATIONS;

    bool hit = false;
    for (int col = 0; col < m_shape_stride; col++){
        for (int row = 0; row < m_shape_stride; row++){
            if (*(test_rotation_offset + col + row * shape_arr_len) > 0){
                left = col;
                hit = true;
                break;
            }  
        }
        if (hit) break;
    }

    hit = false;
    for (int col = m_shape_stride - 1; col >= 0; col--){
        for (int row = 0; row < m_shape_stride; row++){
            if (*(test_rotation_offset + col + row * shape_arr_len) > 0){
                right = col;
                hit = true;
                break;
            }  
        }
        if (hit) break;
    }
}


// This function returns the address of the m_shape_arr_ptr
const int* tetromino::shapeArray() const {
    return m_shape_arr_ptr;
}

int tetromino::shapeStride() const {
    return m_shape_stride;
}

color_id tetromino::color() const {
    return m_color_id;
}
