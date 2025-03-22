// // John Wesley Thompson
// // Created: 1/4/2024
// // Last edited: 18/3/2025
// // tetris_theme.cpp

#include "sound_manager.h"
#include <filesystem>
#include <string>
#include <iostream>
#include <vector>

// default constructor
sound::sound(std::string filename, bool loop){
    this->s_filename = filename;
    s_raw_audio_data = nullptr;
    s_playing = false;
    s_interrupt = false;
    s_looping = loop;
    //std::cout << "sound constructor: " << s_filename << "\n";

    bufferInit();
}

sound::sound(){};

// copy constructor
sound::sound(const sound &og_sound){
    s_filename = og_sound.s_filename;
    s_len_in_pcm_frames = og_sound.s_len_in_pcm_frames;
    s_raw_audio_data = og_sound.s_raw_audio_data;
    s_playing = og_sound.s_playing;
    s_interrupt = og_sound.s_interrupt;
    s_looping = og_sound.s_looping;
    //std::cout << "sound copy constructor " << s_filename << "\n";

    // buffer and buffer config
    s_buffer_config = ma_audio_buffer_config_init(SAMPLE_FORMAT,
                                                  CHANNEL_COUNT,
                                                  s_len_in_pcm_frames,
                                                  s_raw_audio_data, 
                                                  nullptr);

    ma_audio_buffer_init(&s_buffer_config, &s_buffer);
}

sound sound::operator = (const sound& rhs){
    s_filename = rhs.s_filename;
    s_len_in_pcm_frames = rhs.s_len_in_pcm_frames;
    s_raw_audio_data = rhs.s_raw_audio_data;
    s_playing = rhs.s_playing;
    s_interrupt = rhs.s_interrupt;
    s_looping = rhs.s_looping;
    //std::cout << "sound assignment operator " << s_filename << "\n";

    // buffer and buffer config
    s_buffer_config = ma_audio_buffer_config_init(SAMPLE_FORMAT,
                                                CHANNEL_COUNT,
                                                s_len_in_pcm_frames,
                                                s_raw_audio_data, 
                                                nullptr);

    ma_audio_buffer_init_copy(&s_buffer_config, &s_buffer);

    return *this;
}

// This function reads data from an audio file, stores that raw data in an
// array, and initializes the sound structs audio buffer with that data.
void sound::bufferInit(){

    ma_result result;
    // Initialize decoder
    ma_decoder decoder;
    ma_decoder_config decoder_config;

    decoder_config = ma_decoder_config_init(SAMPLE_FORMAT, 
                                            CHANNEL_COUNT, 
                                            SAMPLE_RATE);
    decoder_config.encodingFormat = ma_encoding_format_mp3;

    result = ma_decoder_init_file(s_filename.c_str(), &decoder_config, &decoder);
    if (result != MA_SUCCESS) {
        printf("Could not load file: %s\n", s_filename.c_str());
        return;
    }

    // Initialize buffer read data
    ma_uint64 frames_to_read_this_iteration = DEFAULT_BUFFER_SIZE / 
                                              CHANNEL_COUNT;
    ma_uint64 frames_read_this_iteration = 0;
    ma_uint64 total_frames_read = 0;

    ma_decoder_get_length_in_pcm_frames(&decoder, &s_len_in_pcm_frames);

    float temp[DEFAULT_BUFFER_SIZE];
    ma_uint64 raw_audio_data_size = s_len_in_pcm_frames * CHANNEL_COUNT;
    s_raw_audio_data = (float*)malloc(raw_audio_data_size * sizeof(float));

    if (s_raw_audio_data == NULL){
        std::cout << "MALLOC FAILED!!!\n";
        return;
    }

    // fill the f_audio_buffer with the raw sound data
    ma_decoder_read_pcm_frames(&decoder, temp, 
                               frames_to_read_this_iteration, 
                               &frames_read_this_iteration);

    total_frames_read += frames_read_this_iteration;

    while(total_frames_read < s_len_in_pcm_frames){

        ma_uint64 frames_left_to_read = s_len_in_pcm_frames - total_frames_read;
        if (frames_left_to_read <= frames_to_read_this_iteration){
            frames_to_read_this_iteration = frames_left_to_read;
        }

        for (int i = 0; i <= DEFAULT_BUFFER_SIZE; i++){
            s_raw_audio_data[(total_frames_read - frames_read_this_iteration) * 
                           CHANNEL_COUNT + i] = temp[i];
        }

        ma_decoder_read_pcm_frames(&decoder, temp, 
                                   frames_to_read_this_iteration,
                                   &frames_read_this_iteration);
        
        total_frames_read += frames_read_this_iteration;
    }
 
    // Initialize the audio buffer and config
    s_buffer_config = ma_audio_buffer_config_init(SAMPLE_FORMAT,
                                                  CHANNEL_COUNT,
                                                  total_frames_read,
                                                  s_raw_audio_data,
                                                  nullptr);

    ma_audio_buffer_init(&s_buffer_config, &s_buffer);

    ma_decoder_uninit(&decoder);
}

//
sound::~sound(){
    //std::cout << "sound destructor " << s_filename << "\n";

    ma_audio_buffer_uninit(&s_buffer);
}

//
void sound::uninit(){
    free(s_raw_audio_data);
}

sound_manager::sound_manager(){
    ma_result result;

    player_config = ma_device_config_init(ma_device_type_playback);
    player_config.playback.format = SAMPLE_FORMAT;
    player_config.playback.channels = CHANNEL_COUNT;
    player_config.sampleRate = SAMPLE_RATE;
    player_config.dataCallback = dataCallback;
    player_config.pUserData = (void*)this;

    result = ma_device_init(nullptr, &player_config, &player);
    if (result != MA_SUCCESS){
        std::cout << "device init failed.\n";
    }

    sound_count = 0;
}

//
sound_manager::~sound_manager(){
    for (int i = 0; i < sound_count; i++){
        sounds[i].uninit();
    }
    ma_device_uninit(&player);
}

//
void sound_manager::addSound(std::string filename, bool loop){

    sounds.push_back(sound(filename, loop));
    sound_count++;
}

//
void sound_manager::on(){
    ma_device_start(&player);
}

//
void sound_manager::off(){
    ma_device_stop(&player);
}

//
void sound_manager::playSound(std::string name){

    for (int i = 0; i < sound_count; i++){

        if (sounds[i].s_filename == name){
            
            if (sounds[i].s_playing == true){
                sounds[i].s_interrupt = true;
            } else {
                sounds[i].s_playing = true;
            }
            
        }
    }
}

void sound_manager::playSound(int sound_idx){

    if (sound_idx < 0 || sound_idx >= sound_count)
        return;
        
    if (sounds[sound_idx].s_playing == true){
        sounds[sound_idx].s_interrupt = true;
    } else {
        sounds[sound_idx].s_playing = true;
    }
}

ma_uint32 read_and_mix_pcm_frames_f32(ma_audio_buffer* buffer,
                                             float* pOutput_f32, 
                                             ma_uint32 frame_count)
{
    /*
    The way mixing works is that we just read into a temporary buffer, then take the contents of that buffer and mix it with the
    contents of the output buffer by simply adding the samples together. You could also clip the samples to -1..+1, but I'm not
    doing that in this example.
    */
    
    float temp[DEFAULT_BUFFER_SIZE];
    ma_uint32 tempCapInFrames = DEFAULT_BUFFER_SIZE / CHANNEL_COUNT;
    ma_uint32 totalFramesRead = 0;
    ma_uint64 framesReadThisIteration;
    ma_uint64 framesToReadThisIteration = tempCapInFrames;

    while (totalFramesRead < frame_count) {
        ma_uint32 totalFramesRemaining = frame_count - totalFramesRead;

        if (tempCapInFrames > totalFramesRemaining) {
            framesToReadThisIteration = totalFramesRemaining;
        } else {
            framesToReadThisIteration = tempCapInFrames;
        }

        framesReadThisIteration = ma_audio_buffer_read_pcm_frames(buffer, temp, framesToReadThisIteration, false);

        /* Mix the frames together. */
        for (ma_uint64 i = 0; i < framesReadThisIteration * CHANNEL_COUNT; ++i) {
            pOutput_f32[totalFramesRead * CHANNEL_COUNT + i] += temp[i];
        }

        totalFramesRead += (ma_uint32)framesReadThisIteration;

        if (framesReadThisIteration < (ma_uint32)framesToReadThisIteration) {
            break;  /* Reached EOF. */
        }
    }
    
    return totalFramesRead;
}

//
void dataCallback(ma_device* pDevice, void* pOutput, const void* pInput, ma_uint32 frame_count)
{
    
    float * pOutput_f32 = (float*)pOutput;
    sound_manager* mp = (sound_manager*)pDevice->pUserData;
    
    if (mp == NULL) {
        return;
    }

    // int sound_ct = mp->sounds.size();

    ma_uint32 frames_read;

    for (int i = 0; i < mp->sound_count; i++){
        
        if (mp->sounds[i].s_interrupt){
            ma_audio_buffer_seek_to_pcm_frame(&mp->sounds[i].s_buffer, 0);
            mp->sounds[i].s_interrupt = false;
        }

        if (mp->sounds[i].s_playing){
            
            frames_read = read_and_mix_pcm_frames_f32(&mp->sounds[i].s_buffer, pOutput_f32, frame_count);
            
            if (frames_read < frame_count){
                if (mp->sounds[i].s_looping){
                    ma_audio_buffer_seek_to_pcm_frame(&mp->sounds[i].s_buffer, 0);
                } else {
                    mp->sounds[i].s_playing = false;
                    ma_audio_buffer_seek_to_pcm_frame(&mp->sounds[i].s_buffer, 0);
                }
            }
        }
    } 

    (void)pInput;
}

