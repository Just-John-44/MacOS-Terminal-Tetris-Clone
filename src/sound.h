// John Wesley Thompson
// Created: 9/27/2024
// Last edited: 9/27/2025
// sound.h

#ifndef SOUND_H
#define SOUND_H

#include "../dependencies/include/minaudio.h"


struct sound {
public:
    const char* s_filename;
    ma_audio_buffer s_buffer;
    ma_audio_buffer_config s_buffer_config;
    ma_uint64 s_len_in_pcm_frames;
    float* s_raw_audio_data;
    bool s_playing;
    bool s_interrupt;
    bool s_looping;

    sound(const char*, bool);
    sound(const sound &);
    sound();
    sound& operator = (const sound&);
    ~sound();

private:
    ma_uint64 decodeAudio(ma_decoder&, ma_decoder_config&);
    void bufferInit();
    // void uninit();
};

#endif // SOUND_H