// // John Wesley Thompson
// // Created: 1/4/2024
// // Last edited: 9/27/2025
// // tetris_theme.cpp

#include "sound_manager.h"
#include "sound.h"
#include "s_and_sm_common.h"
#include <iostream>


// small_crash.mp3
// Driken5482 https://pixabay.com/users/driken5482-45721595/
// big_crash.mp3
// freesound_community https://pixabay.com/users/freesound_community-46691455/
// Tetris.mp3
// jkotas1 https://archive.org/details/@jkotas1
// tetris.mp3
// RasoolAsaad https://pixabay.com/users/rasoolasaad-47313572/
// level_up.mp3
// mattwasser https://pixabay.com/sound-effects/coin-upaif-14631/
// 
const char* sound_filenames[SOUND_COUNT] = {
    "../attachments/sounds/theme.mp3",
    "../attachments/sounds/tetris.mp3",
    "../attachments/sounds/small_crash.mp3",
    "../attachments/sounds/big_crash.mp3",
    "../attachments/sounds/level_up.mp3"
};


sound_manager::sound_manager(){

    // set all of the tracks
    initSounds(sound_filenames, SOUND_COUNT);
    setRepeating(THEME, true);

    player_config = ma_device_config_init(ma_device_type_playback);
    player_config.playback.format = SAMPLE_FORMAT;
    player_config.playback.channels = CHANNEL_COUNT;
    player_config.sampleRate = SAMPLE_RATE;
    player_config.dataCallback = dataCallback;
    player_config.pUserData = (void*)this;

    ma_result result = ma_device_init(nullptr, &player_config, &player);
    if (result != MA_SUCCESS){
        std::cout << "sound manager device init failed.\n";
    }
}

//
sound_manager::~sound_manager(){
    for (int i = 0; i < SOUND_COUNT; i++){
        sounds[i].~sound();
    }
    ma_device_uninit(&player);
}

//
void sound_manager::on(){
    ma_device_start(&player);
}

//
void sound_manager::off(){
    ma_device_stop(&player);
}

void sound_manager::playSound(int sound_idx){

    if (sound_idx < 0 || sound_idx >= SOUND_COUNT)
        return;
        
    if (sounds[sound_idx].s_playing == true){
        sounds[sound_idx].s_interrupt = true;
    } else {
        sounds[sound_idx].s_playing = true;
    }
}

void sound_manager::initSounds(const char** filenames, int size){
    for (int i = 0; i < size; i++){
        new (&sounds[i]) sound(filenames[i], false);
    }
}

void sound_manager::setRepeating(sound_idx track, bool setting){
    sounds[track].s_looping = setting;
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

    ma_uint32 frames_read;

    for (int i = 0; i < SOUND_COUNT; i++){
        
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

