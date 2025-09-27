// John Wesley Thompson
// Created: 1/3/2024
// Last edited: 9/27/2025
// sound_manager.h

#ifndef SOUND_MANAGER_H
#define SOUND_MANAGER_H

#include "../dependencies/include/minaudio.h"
#include "sound.h"

#define SOUND_COUNT 5

enum sound_idx {    // sounds in the sound array need to be added in this exact same order
    THEME = 0,
    TETRIS,
    SOFT_DROP,
    HARD_DROP,
    LEVEL_UP
};


struct sound_manager {
public:
    sound_manager();
    ~sound_manager();
    void on();                          // starts the audio feed
    void off();                         // stops the audio feed
    void playSound(int);                // adds a sound to the audio feed by the index of sounds vector
    void initSounds(const char**, int size); // adds all of the sounds to the sounds array by file name
    void setRepeating(sound_idx, bool);

    sound sounds[SOUND_COUNT];

private:
    ma_device player;
    ma_device_config player_config;
};

// function used to mix multiple sounds' audio data so sounds can play at the same time
ma_uint32 read_and_mix_pcm_frames_f32(ma_audio_buffer*, float*, ma_uint32);

// function used to callback raw audio data when a sound is scheduled or is playing
void dataCallback(ma_device*, void*, const void*, ma_uint32);

#endif // SOUND_MANAGER_H