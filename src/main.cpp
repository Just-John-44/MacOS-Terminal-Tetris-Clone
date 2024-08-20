// Author: John Wesley Thompson
// Creation Date: 8/10/2023
// Last Edited:
// Completed:
// main.cpp

// "/Users/jwt44/Documents/Music/Media.localized/Boston/Boston/Peace\\ Of\\ Mind.wav";

// This program is my best shot at copying tetris in the console.

#ifdef __APPLE__
    #define MA_NO_RUNTIME_LINKING
#endif
#define MINIAUDIO_IMPLEMENTATION
#include "minaudio.h"

#include <stdio.h>
#include <iostream>
#include <thread>

// line 1730 - handling multpile 'sounds'
// line 2546 - looping?

void initGame();

int main(){
    
    ma_result result;

    // DECODER
    ma_decoder decoder;
    result = ma_decoder_init_file("Tetris.mp3", nullptr, &decoder);
    if (result != MA_SUCCESS){
        std::cout << "ma_decoder_init_file failed\n";
        ma_decoder_uninit(&decoder);
        return -1;
    }
    ma_data_source_set_looping(&decoder, MA_TRUE);

    // DEVICE
    ma_device_config device_config;
    device_config = ma_device_config_init(ma_device_type_playback);
    device_config.playback.channels = decoder.outputChannels;
    device_config.sampleRate = decoder.outputSampleRate;
    device_config.dataCallback = dataCallback;
    device_config.pUserData = &decoder;
    device_config.noPreSilencedOutputBuffer = true;

    ma_device device; 
    if (ma_device_init(nullptr, &device_config, &device) != MA_SUCCESS){
        std::cout << "failed to initialize device\n";
        ma_decoder_uninit(&decoder);
        return -1;
    }

    ma_device_start(&device);
    // Testing pauses
    

    ma_device_stop(&device);
    std::cin.get();

    ma_device_start(&device);
    std::cin.get();

    ma_decoder_uninit(&decoder);
    ma_device_uninit(&device);
}

// This function is retrieves info from a sound file decoder and is used to
// give that info to the playback device object.
void dataCallback(ma_device* pDevice, void* pOutput, 
                  const void* pInput, ma_uint32 frame_count){

    ma_decoder* pDecoder = (ma_decoder*)pDevice->pUserData;
    if (pDecoder == NULL) {
        return;
    }

    ma_result result = ma_data_source_read_pcm_frames(pDecoder, pOutput, 
                                                      frame_count, nullptr);

    (void)pInput;
}


void initGame(){

}
// std::cout << "pausing in 3\n";
    // std::this_thread::sleep_for(std::chrono::seconds(1));

    // std::cout << "pausing in 2\n";
    // std::this_thread::sleep_for(std::chrono::seconds(1));

    // std::cout << "pausing in 1\n";
    // std::this_thread::sleep_for(std::chrono::seconds(1));

// void improveColors(){
//     if (can_change_color()){
//         init_color(COLOR_MAGENTA, 255, 165, 0); // making magenta orange
//     }
// }