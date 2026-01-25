#ifndef PAUSE_MENU_H
#define PAUSE_MENU_H

#include "raylib.h"
#include "config.h" // for Game::ScreenWidth / Game::ScreenHeight

enum class PauseAction {
    None,
    Resume,
    Restart,
    Quit
};

struct PauseMenu {
    bool isPaused = false;
    int selectedIndex = 0; // 0: Continue, 1: Restart, 2: Quit

    void UpdateToggle();       // ESC toggle
    PauseAction UpdateInput(); // UP/DOWN/ENTER when paused
    void Draw() const;         // draw inside canvas (uses Game::ScreenWidth/Height)
};

#endif
