// John Wesley Thompson 
// Created: 9/27/2025
// Last Edited: 9/27/2025
// s_and_sm_common.h

#ifndef S_AND_SM_COMMON_H
#define S_AND_SM_COMMON_H

#include "../dependencies/include/minaudio.h"

constexpr int CHANNEL_COUNT = 2;
constexpr int SAMPLE_RATE = 48000;  // hz
constexpr int DEFAULT_BUFFER_SIZE = 4096;
constexpr ma_format SAMPLE_FORMAT = ma_format_f32;

#endif // S_AND_SM_COMMON_H 