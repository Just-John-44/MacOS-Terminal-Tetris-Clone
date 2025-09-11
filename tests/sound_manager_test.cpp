
#ifdef __APPLE__
    #define MA_NO_RUNTIME_LINKING
#endif
#define MINIAUDIO_IMPLEMENTATION



//#include "tetris_theme.h"
#include "../dependencies/include/minaudio.h"
#include <iostream>

#define CHANNEL_COUNT 2
#define SAMPLE_RATE 44100
#define DEFAULT_BUFFER_SIZE 4096
#define SAMPLE_FORMAT ma_format_f32

void data_callback(ma_device* pDevice, void* pOutput, const void* pInput, ma_uint32 frameCount)
{
    ma_audio_buffer* audio_buffer = (ma_audio_buffer*)pDevice->pUserData;

    ma_audio_buffer_read_pcm_frames(audio_buffer, pOutput, frameCount, false);

    (void)pInput;
}

int main()
{
    ma_result result;
    ma_decoder decoder;
    ma_decoder_config decoder_config;
    ma_device device;
    ma_device_config deviceConfig;
    ma_audio_buffer audio_buffer;
    ma_audio_buffer_config audio_buffer_config;

    // DECODER INITIALIZATION
    decoder_config = ma_decoder_config_init(SAMPLE_FORMAT, CHANNEL_COUNT, SAMPLE_RATE);
    decoder_config.encodingFormat = ma_encoding_format_mp3;

    result = ma_decoder_init_file("Tetris.mp3", &decoder_config, &decoder);
    if (result != MA_SUCCESS) {
        printf("Could not load file\n");
        return -2;
    }

    // audio_Buffer for PCM data
    ma_uint64 frames_to_read_per_iteration = DEFAULT_BUFFER_SIZE / CHANNEL_COUNT;
    ma_uint64 frames_read_this_iteration = 0;
    ma_uint64 total_frames_read = 0;
    ma_uint64 len_in_pcm_frames;

    ma_decoder_get_length_in_pcm_frames(&decoder, &len_in_pcm_frames);
    float read_audio_buffer[DEFAULT_BUFFER_SIZE];
    float* float_audio_buffer = (float*)malloc(sizeof(float) * len_in_pcm_frames * CHANNEL_COUNT);

    if (float_audio_buffer == NULL){
        std::cout << "MALLOC FAILED!!!\n";
    }

    ma_decoder_read_pcm_frames(&decoder, read_audio_buffer, frames_to_read_per_iteration, &frames_read_this_iteration);
    total_frames_read += frames_read_this_iteration;
    while (total_frames_read < len_in_pcm_frames){

        if (len_in_pcm_frames - total_frames_read <= frames_to_read_per_iteration){
            frames_to_read_per_iteration = len_in_pcm_frames - total_frames_read;
        }

        for (int i = 0; i < DEFAULT_BUFFER_SIZE; i++){
            float_audio_buffer[(total_frames_read - frames_read_this_iteration) * CHANNEL_COUNT + i] = read_audio_buffer[i];
        }

        ma_decoder_read_pcm_frames(&decoder, read_audio_buffer, frames_to_read_per_iteration, &frames_read_this_iteration);
        total_frames_read += frames_read_this_iteration;
    }

    // Print the first 100 samples for debugging
    for (int i = len_in_pcm_frames * CHANNEL_COUNT - 1; i > 0; --i){
        std::cout << float_audio_buffer[i] << "\n";
    }

    std::cout << "frames read: " << frames_read_this_iteration << "\n";
    std::cout << "Total frames read: " << total_frames_read << "\n";

    // audio_BUFFER_INITIALIZATION
    audio_buffer_config = ma_audio_buffer_config_init(SAMPLE_FORMAT,
                                                CHANNEL_COUNT,
                                                total_frames_read,
                                                float_audio_buffer,
                                                nullptr);  

    ma_audio_buffer_init_copy(&audio_buffer_config, &audio_buffer);

    // DEVICE_INITIALIZATION
    deviceConfig = ma_device_config_init(ma_device_type_playback);
    deviceConfig.playback.format   = SAMPLE_FORMAT;
    deviceConfig.playback.channels = CHANNEL_COUNT;
    deviceConfig.sampleRate        = SAMPLE_RATE;
    deviceConfig.dataCallback      = data_callback;
    deviceConfig.pUserData         = &audio_buffer;

    if (ma_device_init(NULL, &deviceConfig, &device) != MA_SUCCESS) {
        printf("Failed to open playback device.\n");
        ma_decoder_uninit(&decoder);
        return -3;
    }

    if (ma_device_start(&device) != MA_SUCCESS) {
        printf("Failed to start playback device.\n");
        ma_device_uninit(&device);
        ma_decoder_uninit(&decoder);
        return -4;
    }

    printf("Press Enter to quit...");
    getchar();

    ma_device_uninit(&device);
    ma_decoder_uninit(&decoder);
    getchar();

    return 0;
}

