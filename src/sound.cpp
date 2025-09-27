// John Wesley Thompson
// Created: 9/27/2024
// Last edited: 9/27/2025
// sound.cpp


#include "sound.h"
#include "s_and_sm_common.h"
#include "iostream"


sound::sound(const char* filename, bool loop){
    s_filename = filename;
    s_raw_audio_data = nullptr;
    s_playing = false;
    s_interrupt = false;
    s_looping = loop; 

    bufferInit();
}

sound::~sound(){

    ma_audio_buffer_uninit(&s_buffer);
    free(s_raw_audio_data);
}

sound::sound(){};

// decodeAudio reads the audio from the audio file and stores its raw data in
// a buffer to be read on demand
ma_uint64 sound::decodeAudio(ma_decoder& decoder, ma_decoder_config& decoder_config){
    ma_result result = ma_decoder_init_file(s_filename, &decoder_config, &decoder);
    if (result != MA_SUCCESS) {
        printf("ma_decoder could not load file: %s\n", s_filename);
        return -1;
    }

    // Initialize variables used to read data into the audio buffer
    ma_uint64 frames_to_read_this_iteration = DEFAULT_BUFFER_SIZE / 
                                              CHANNEL_COUNT;
    ma_uint64 frames_read_this_iteration = 0;
    ma_uint64 total_frames_read = 0;

    // allocate a buffer large enough to store the audio data
    ma_decoder_get_length_in_pcm_frames(&decoder, &s_len_in_pcm_frames);

    float temp[DEFAULT_BUFFER_SIZE];
    ma_uint64 raw_audio_data_size = s_len_in_pcm_frames * CHANNEL_COUNT;
    s_raw_audio_data = (float*)malloc(raw_audio_data_size * sizeof(float));

    if (s_raw_audio_data == NULL){
        std::cout << "failed to malloc s_raw_audio_data buffer\n";
        return -1;
    }

    // read audio data into s_raw_audio_data
    ma_decoder_read_pcm_frames(&decoder, temp, frames_to_read_this_iteration,
                               &frames_read_this_iteration);

    total_frames_read += frames_read_this_iteration;

    while(total_frames_read < s_len_in_pcm_frames){

        ma_uint64 frames_left_to_read = s_len_in_pcm_frames - total_frames_read;
        if (frames_left_to_read <= frames_to_read_this_iteration){
            frames_to_read_this_iteration = frames_left_to_read;
        }

        for (int i = 0; i <= frames_read_this_iteration * CHANNEL_COUNT; i++){
            s_raw_audio_data[(total_frames_read - frames_read_this_iteration) * 
                           CHANNEL_COUNT + i] = temp[i];
        }

        ma_decoder_read_pcm_frames(&decoder, temp, 
                                   frames_to_read_this_iteration,
                                   &frames_read_this_iteration);
        
        total_frames_read += frames_read_this_iteration;
    }

    return total_frames_read;
}

// This function reads data from an audio file, stores that raw data in an
// array, and initializes the sound structs audio buffer with that data.
void sound::bufferInit(){
    
    // Initialize decoder
    ma_decoder decoder;
    ma_decoder_config decoder_config;

    decoder_config = ma_decoder_config_init(SAMPLE_FORMAT, 
                                            CHANNEL_COUNT, 
                                            SAMPLE_RATE);
    decoder_config.encodingFormat = ma_encoding_format_mp3;

    ma_uint64 total_frames_read = decodeAudio(decoder, decoder_config);
    ma_decoder_uninit(&decoder);

    // Initialize the audio buffer and config
    s_buffer_config = ma_audio_buffer_config_init(SAMPLE_FORMAT,
                                                  CHANNEL_COUNT,
                                                  total_frames_read,
                                                  s_raw_audio_data,
                                                  nullptr);

    ma_audio_buffer_init(&s_buffer_config, &s_buffer);
}