#include <fstream>
#include <iostream>
#include <string>

int main(){

    std::ifstream infile;
    infile.open("Shifting.mp3");

    infile.seekg(0, std::ios::end);
    std::streampos size = infile.tellg();
    infile.seekg(0, std::ios::beg);

    char* buffer = new char[size];
    infile.read(buffer, size);


    for (int i = 0; i < size; i++){
        std::cout << (int)buffer[i] << '\n';
    }

    std::cout << size;

    delete [] buffer;
    infile.close();
    return 0;
}