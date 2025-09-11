// John Wesley Thompson
// Created: 24 Mar 2025
// Last Edited: 24 Mar 2025
// filesystem_test.cpp

// This is a driver program to make sure that I understand how the
// filesystem library works before I use it in the main tetris program

#include <iostream>
#include <filesystem>
#include <vector>
#include <string>
#include <regex>

namespace fs = std::filesystem;
const fs::path working_directory = fs::current_path();

bool filenameGreaterThan(std::string file1, std::string file2){

    std::regex pattern(R"([0-9]*)");
    std::smatch file1_num, file2_num;
    std::regex_search(file1, file1_num, pattern);
    std::regex_search(file2, file2_num, pattern);

    int file1_num_len = file1_num.size();
    int file2_num_len = file2_num.size();
    int largest_match_length = file1_num_len > file2_num_len? file1_num_len : file2_num_len;
    for (int i = 0; i < largest_match_length; i++){

    }
}

void sortSounds(std::vector<std::string> & vec){

    if (vec.size() <= 1) return;

    bool sorted = false;

    int vec_sort_len = vec.size() - 1;
    std::string temp;
    while(!sorted && vec_sort_len != 1){
        sorted = true;

        for (int i = 0; i < vec_sort_len; i++){
            // swap elements if the first index points to a greater value
            if (vec.at(i) > vec.at(i + 1)){
                sorted = false;
                temp = vec.at(i);
                vec.at(i) = vec.at(i + 1);
                vec.at(i + 1) = temp;
            }
        }
        vec_sort_len--;
    }
}

int main(){

    std::vector<std::string> path_vec;

    const std::string extension = ".mp3\0";

    for (fs::directory_entry dir_entry : fs::directory_iterator{working_directory}){
        std::string filename = dir_entry.path().string();
        
        if (filename.find(extension) != std::string::npos){
            path_vec.push_back(filename);
        }
    }

    sortSounds(path_vec);

    for (int i = 0; i < path_vec.size(); i++){
        std::cout << path_vec.at(i) << "\n";
    }


    return 0;
}