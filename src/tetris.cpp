// tetris.cpp

#include "tetris.h"


// BLOCK CLASS ================================================================
block::block(block_type bt){

    switch(bt){
        case L_BLOCK:   
            m_shape_arr_ptr = L_BLOCK_ARR;
            m_shape_stride = 3;

            m_color = COLOR_MAGENTA;
            break;

        case J_BLOCK: 
            m_shape_arr_ptr = O_BLOCK_ARR;
            m_shape_stride = 3;
            
            m_color = COLOR_BLUE;
            break;

        case T_BLOCK: 
            m_shape_arr_ptr = O_BLOCK_ARR;
            m_shape_stride = 3;
            
            m_color = COLOR_BLUE;
            break;

        case I_BLOCK: 
            m_shape_arr_ptr = O_BLOCK_ARR;
            m_shape_stride = 4;

            m_color = COLOR_CYAN;
            break;

        case S_BLOCK: 
            m_shape_arr_ptr = O_BLOCK_ARR;
            m_shape_stride = 3;

            m_color = COLOR_GREEN;
            break;

        case Z_BLOCK: 
            m_shape_arr_ptr = O_BLOCK_ARR;
            m_shape_stride = 3;

            m_color = COLOR_RED;
            break;

        case O_BLOCK: 
            m_shape_arr_ptr = O_BLOCK_ARR;
            m_shape_stride = 3;
            
            m_color = COLOR_YELLOW;
            break;
    }
}

// This function returns the address of the m_shape_arr_ptr
const int* block::shapeArray(){
    return m_shape_arr_ptr;
}

int block::shapeStride(){
    return m_shape_stride;
}

int block::color(){
    return m_color;
}

// TETRIS_GRID CLASS ==========================================================

void tetris_grid::rotateR(block* curr_block, const int* &rotation_offset){
    const int* array_start = curr_block->shapeArray();
    int m_shape_stride = curr_block->shapeStride();
    
    if (array_start = O_BLOCK_ARR) 
        return;

    if (rotation_offset = array_start + (m_shape_stride * (POSSIBLE_ROTATIONS - 1))){
        rotation_offset = array_start;
    } else {
        rotation_offset += m_shape_stride;
    }
}

void tetris_grid::rotateL(block* curr_block, const int* &rotation_offset){
    const int* array_start = curr_block->shapeArray();
    int m_shape_stride = curr_block->shapeStride();
    
    if (array_start = O_BLOCK_ARR) 
        return;

    if (rotation_offset == array_start){
        rotation_offset += (m_shape_stride * (POSSIBLE_ROTATIONS - 1));
    } else {
        rotation_offset -= m_shape_stride;
    }
}