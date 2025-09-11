
#ifdef __APPLE__
    #define MA_NO_RUNTIME_LINKING
#endif
#define MINIAUDIO_IMPLEMENTATION
#include "../dependencies/include/minaudio.h"
#include "sound_manager.h"
#include <iostream>

enum sound_idx {
    GAME_START,
    THEME,
    ONE_ROW,
    TWO_ROWS,
    THREE_ROWS,
    FOUR_ROWS,
    SHIFT,
    ROTATE,
    SOFT_DROP,
    HARD_DROP,
    LEVEL_UP,
    GAME_OVER
};

int main(){
    
    sound_manager mp;
    mp.addSound("../attachments/sounds/silly/lets_play.mp3", false);
    mp.addSound("../attachments/sounds/silly/theme.mp3", true);
    mp.addSound("../attachments/sounds/silly/one_row.mp3", false);
    mp.addSound("../attachments/sounds/silly/two_rows.mp3", false);
    mp.addSound("../attachments/sounds/silly/three_rows.mp3", false);
    mp.addSound("../attachments/sounds/silly/four_rows.mp3", false);
    mp.addSound("../attachments/sounds/silly/shift.mp3", false);
    mp.addSound("../attachments/sounds/silly/rotation.mp3", false);
    mp.addSound("../attachments/sounds/silly/soft_drop.mp3", false);
    // mp.addSound("../attachments/sounds/silly/hard_drop.mp3", false);
    // mp.addSound("../attachments/sounds/silly/faster.mp3", false);
    // mp.addSound("../attachments/sounds/silly/game_over.mp3", false);
    mp.on();

    for (sound sound : mp.sounds){
        std::cout << sound.filename << "\n";
    }

    mp.playSound("../attachments/sounds/silly/lets_play.mp3");
    std::cin.get();
    mp.playSound("../attachments/sounds/silly/theme.mp3");
    std::cin.get();
    mp.playSound("../attachments/sounds/silly/one_row.mp3");
    std::cin.get();
    mp.playSound("../attachments/sounds/silly/two_rows.mp3");
    std::cin.get();
    mp.playSound("../attachments/sounds/silly/three_rows.mp3");
    std::cin.get();
    mp.playSound("../attachments/sounds/silly/four_rows.mp3");
    std::cin.get();
    mp.playSound("../attachments/sounds/silly/shift.mp3");
    std::cin.get();
    mp.playSound("../attachments/sounds/silly/rotation.mp3");
    std::cin.get();
    mp.playSound("../attachments/sounds/silly/soft_drop.mp3");
    std::cin.get();
    // mp.playSound(9);
    // std::cin.get();
    // mp.playSound(10);
    // std::cin.get();
    // mp.playSound(11);
    // std::cin.get();

    // int c;
    // c = std::cin.get();
    // while (c != 'q'){
    //     if (c == 'c'){
    //         mp.playSound(1);
    //     } else if (c == 'p'){
    //         mp.on();
    //     }
    //     c = std::cin.get();
    // }


    mp.off();
    std::cin.get();
    std::cin.get();

    return 0;
}
