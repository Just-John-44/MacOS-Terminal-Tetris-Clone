// John Wesley Thompson 
// Created: 12/31/2024
// Last Edited: 1/1/2024

#include "tetrose_bag.h"

// the default constructor for bag sets up the random number generator and
// fills the tetrominoes array with all of the different tetrominoes.
bag::bag(){

    gen = std::mt19937(rd());
    dist = std::uniform_int_distribution<>(0, 6);

    for (int i = 0; i < 7; i++){
        selections.push_back(i);
    }
}

// select removes a tetromino selction index from selections and returns
// a pointer to the tetromino it corresponds do in the tetrominoes array.
tetromino* bag::select(){

    dist = std::uniform_int_distribution<>(0, selections.size() - 1);
    int random_idx = dist(gen);
    
    tetromino* selection = &tetrominoes[selections.at(random_idx)];
    selections.erase(selections.begin() + random_idx);

    return selection;
    
}

// refill refills the bag with all of the available tetromino indexes. If 
// selections is not empty, it does nothing.
void bag::refill(){

    if (!selections.empty()){
        return;
    }

    dist = std::uniform_int_distribution<>(0, 6);

    for (int i = 0; i < 7; i++){
        selections.push_back(i);
    }
}

// empty returns true if the selections vector is empty, meaning that all of
// the tetrominoes have been selected.
bool bag::empty(){

    return selections.empty();
}
