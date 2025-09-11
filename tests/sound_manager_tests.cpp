// This program is to debug the bug that is causing the sound manager
// to play the same sounds even when they should be different.

#ifdef __APPLE__
    #define MA_NO_RUNTIME_LINKING
#endif
#define MINIAUDIO_IMPLEMENTATION
#include "../src/sound_manager.h"
#include <iostream>

int main(){

    sound_manager sm;

    sm.addSound("1_test.mp3", false);
    sm.addSound("2_test.mp3", false);
    sm.addSound("3_test.mp3", false);
    sm.addSound("4_test.mp3", false);
    sm.addSound("5_test.mp3", false);
    sm.addSound("6_test.mp3", false);
    sm.addSound("7_test.mp3", false);
    sm.addSound("8_test.mp3", false);
    sm.addSound("9_test.mp3", false);
    sm.addSound("10_test.mp3", false);


    for (int i = 0; i < sm.sound_count; i++){
        std::cout << "filename: " << sm.sounds[i].s_filename << "\n";
        std::cout << "buffer address: " << &sm.sounds[i].s_buffer.ref.pData << "\n";
        std::cout << "buffer_conf address: " << &sm.sounds[i].s_buffer_config.pData << "\n";
        std::cout << "pcm frame length: " << sm.sounds[i].s_len_in_pcm_frames << "\n";
        std::cout << "playing: " << sm.sounds[i].s_playing << "\n";
        std::cout << "looping: " << sm.sounds[i].s_looping << "\n";
        std::cout << "interrupt: " << sm.sounds[i].s_interrupt << "\n";
        std::cout << "--------------------------------------\n";
    }

    sm.on();

    char in;
    while (true){
        std::fflush(stdin);
        in = std::getchar();
        switch (in){
            std::cout << "in switch.\n";
            case '1':
                std::cout << "in case 1.\n";
                sm.playSound("1_test.mp3");
                std::cout << "currently playing: " << sm.sounds[0].s_playing << "\n";
                break;

            case '2':
                sm.playSound("2_test.mp3");
                break;

            case '3':
                sm.playSound("3_test.mp3");
                break;

            case '4':
                sm.playSound("4_test.mp3");
                break;

            case '5':
                sm.playSound("5_test.mp3");
                break;

            case '6':
                sm.playSound("6_test.mp3");
                break;

            case '7':
                sm.playSound("7_test.mp3");
                break;

            case '8':
                sm.playSound("8_test.mp3");
                break;

            case '9':
                sm.playSound("9_test.mp3");
                break;

            case '0':
                sm.playSound("10_test.mp3");
                break;
        }
    }
}



