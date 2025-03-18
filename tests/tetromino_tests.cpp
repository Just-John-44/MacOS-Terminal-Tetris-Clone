

#include "tetris_tetromino.h"
#include <iostream>

void printCurrTet(tetromino &);

int main(){

    // TESTING CONSTRUCTORS AND ASSIGNMENTS
    std::cout << "TESTING CONSTRUCTORS AND ASSIGNMENTS ========================\n\n";

    tetromino test_buddy(T_TETROMINO);
    std::cout << "test_buddy:\n";
    printCurrTet(test_buddy);

    tetromino test_buddys_buddy(I_TETROMINO);
    std::cout << "test_buddy's buddy:\n";
    printCurrTet(test_buddys_buddy);

    std::cout << "copy construtor copy:\n";
    tetromino copy_cat(test_buddy);
    printCurrTet(copy_cat);

    std::cout << "assignment operator copy:\n";
    copy_cat = test_buddys_buddy;
    printCurrTet(copy_cat);

    // TESTING ROTATION FUNCTIONS
    std::cout << "ROTATIONS ===================================================\n\n";

    std::cout << "4 CWISE rotations for test_buddy:\n";
    printCurrTet(test_buddy);
    test_buddy.rotate(CWISE);
    printCurrTet(test_buddy);
    test_buddy.rotate(CWISE);
    printCurrTet(test_buddy);
    test_buddy.rotate(CWISE);
    printCurrTet(test_buddy);
    test_buddy.rotate(CWISE);
    printCurrTet(test_buddy);
    test_buddy.rotate(CCWISE);
    printCurrTet(test_buddy);
    test_buddy.rotate(CCWISE);
    printCurrTet(test_buddy);
    test_buddy.rotate(CCWISE);
    printCurrTet(test_buddy);
    test_buddy.rotate(CCWISE);
    printCurrTet(test_buddy);

    std::cout << "4 CWISE rotations for test_buddys_buddy:\n";
    printCurrTet(test_buddys_buddy);
    test_buddys_buddy.rotate(CWISE);
    printCurrTet(test_buddys_buddy);
    test_buddys_buddy.rotate(CWISE);
    printCurrTet(test_buddys_buddy);
    test_buddys_buddy.rotate(CWISE);
    printCurrTet(test_buddys_buddy);
    test_buddys_buddy.rotate(CWISE);
    printCurrTet(test_buddys_buddy);
    test_buddys_buddy.rotate(CCWISE);
    printCurrTet(test_buddys_buddy);
    test_buddys_buddy.rotate(CCWISE);
    printCurrTet(test_buddys_buddy);
    test_buddys_buddy.rotate(CCWISE);
    printCurrTet(test_buddys_buddy);
    test_buddys_buddy.rotate(CCWISE);
    printCurrTet(test_buddys_buddy);

    std::cout << "4 CWISE rotations for copy_cat:\n";
    printCurrTet(copy_cat);
    copy_cat.rotate(CWISE);
    printCurrTet(copy_cat);
    copy_cat.rotate(CWISE);
    printCurrTet(copy_cat);
    copy_cat.rotate(CWISE);
    printCurrTet(copy_cat);
    copy_cat.rotate(CWISE);
    printCurrTet(copy_cat);
    copy_cat.rotate(CCWISE);
    printCurrTet(copy_cat);
    copy_cat.rotate(CCWISE);
    printCurrTet(copy_cat);
    copy_cat.rotate(CCWISE);
    printCurrTet(copy_cat);
    copy_cat.rotate(CCWISE);
    printCurrTet(copy_cat);


}

void printCurrTet(tetromino &tet){
    for (int i = 0; i < tet.sstride; i++){
        for (int j = 0; j < tet.sstride; j++){
            std::cout << tet.shapeAt(i, j)
                      << " "; 
        }
        std::cout << "\n";
    }

    std::cout << "topmost: " << tet.topmost_sqr << "\n"
              << "bottommost: " << tet.bottommost_sqr << "\n"
              << "CCWISEmost: " << tet.leftmost_sqr << "\n"
              << "CWISEmost: " << tet.rightmost_sqr << "\n"
              << "curr rotation: " << tet.current_rotation << "\n"; 

    std::cout << "\n";
}