#ifndef TETRIS_BAG_H
#define TETRIS_BAG_H

// John Wesley Thompson 
// Created: 31/12/2024
// Last Edited: 31/12/2024

#include "tetris_tetromino.h"
#include <vector>
#include <random>

struct bag {

    std::random_device rd;
    std::mt19937 gen;
    std::uniform_int_distribution<> dist;

    tetromino tetrominoes[7] = {
        tetromino(L_TETROMINO),
        tetromino(J_TETROMINO),
        tetromino(T_TETROMINO),
        tetromino(I_TETROMINO),
        tetromino(S_TETROMINO),
        tetromino(Z_TETROMINO),
        tetromino(O_TETROMINO)
    };
    
    std::vector<int>selections;

    bag();
    tetromino* select();
    void refill();
    bool empty();
};

#endif