// John Wesley Thompson
// Created: 1/3/2024
// Last edited: 18/3/2025
// tetris_theme.h

// Block_settle.mp3
// Driken5482 https://pixabay.com/users/driken5482-45721595/
// 8_bit_crash.mp3
// freesound_community https://pixabay.com/users/freesound_community-46691455/
// Tetris.mp3
// jkotas1 https://archive.org/details/@jkotas1
// Row_clear.mp3
// RasoolAsaad https://pixabay.com/users/rasoolasaad-47313572/
// 

// TO DO: create a way to read all audio files from a directory instead of 
//        entering all of the file names.

#define MINIAUDIO_IMPLEMENTATION

#include "../dependencies/include/minaudio.h"
#include <thread>
#include <string>
#include <vector>
#include <filesystem>

#define CHANNEL_COUNT 2
#define SAMPLE_RATE 48000
#define DEFAULT_BUFFER_SIZE 4096
#define SAMPLE_FORMAT ma_format_f32

struct sound {
public:
    std::string s_filename;
    ma_audio_buffer s_buffer;
    ma_audio_buffer_config s_buffer_config;
    ma_uint64 s_len_in_pcm_frames;
    float* s_raw_audio_data;
    bool s_playing;
    bool s_interrupt;
    bool s_looping;

    // add default constructor (implicit ones are not a thing if you specify an overload)
    sound(std::string, bool);
    sound(const sound &);
    sound();
    sound operator = (const sound&);
    ~sound();
    void uninit();
private:
    void bufferInit();
};


struct sound_manager {
public:

    sound_manager();
    ~sound_manager();
    //void loadSounds(); // TO DO
    void addSound(std::string, bool);
    void on();
    void off();
    void playSound(std::string);
    void playSound(int);
    int sound_count;

    std::vector<sound> sounds;
private:
    ma_device player;
    ma_device_config player_config;

};

ma_uint32 read_and_mix_pcm_frames_f32(ma_audio_buffer*, float*, ma_uint32);
void dataCallback(ma_device*, void*, const void*, ma_uint32);