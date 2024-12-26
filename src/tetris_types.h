#ifndef TETRIS_TYPES_H
#define TETRIS_TYPES_H

// These are used to set the type variable 
// in the tetris_tetromino class.
enum tetromino_type {
    L_TETROMINO = 1,
    J_TETROMINO,
    T_TETROMINO,
    I_TETROMINO,
    S_TETROMINO,
    Z_TETROMINO,
    O_TETROMINO
};

#define COLOR_PURPLE 8  
#define COLOR_ORANGE 9 
#define COLOR_L_BLUE 10
#define COLOR_L_YELLOW 11

enum color_id{
    STANDARD = 1,
    ORANGE,
    BLUE,
    PURPLE,
    CYAN,
    GREEN,
    RED,
    YELLOW,
    L_BLUE,
    L_YELLOW
};

enum square_type {
    EMPTY_SQR = 0,
    TOP_SQR,
    BOTTOM_SQR
};

enum direction {
    CWISE,
    CCWISE
};

#endif