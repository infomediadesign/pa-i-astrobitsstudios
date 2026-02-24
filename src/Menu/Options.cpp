//
// Created by Ben on 23.01.2026.
//

#include "Options.h"
#include "config.h"
#include <algorithm>
#include <raylib.h>

Options::Options() {
    options={"Steuerung","Volume"};
}

void Options::Update() {
    // Simple navigation: if Volume is selected (index 1), A/D adjust volume
    if (selectedItem == 1) {
        // hold A/D to change volume
        // Note: Options::Update doesn't have dt; use IsKeyDown with a fixed small step
        if (IsKeyDown(KEY_A)) {
            masterVolume = std::max(0.0f, masterVolume - 0.01f);
        }
        if (IsKeyDown(KEY_D)) {
            masterVolume = std::min(1.0f, masterVolume + 0.01f);
        }
        if (IsKeyPressed(KEY_M)) {
            ToggleMute();
        }
    }

    // Simple menu left/right selection (only when not on volume)
    if (selectedItem != 1 && (IsKeyPressed(KEY_TAB) || IsKeyPressed(KEY_RIGHT))) {
        selectedItem = (selectedItem + 1) % static_cast<int>(options.size());
    }
    if (selectedItem != 1 && IsKeyPressed(KEY_LEFT)) {
        selectedItem = (selectedItem - 1 + static_cast<int>(options.size())) % static_cast<int>(options.size());
    }
}

void Options::Draw() {
    DrawText("Optionen", static_cast<int>(Game::ScreenWidth/2.5), 0, 40, BLACK);

    for (int i = 0; i < static_cast<int>(options.size()); i++) {
        Color c = (i == selectedItem) ? RED : BLACK;
        DrawText(options[i].c_str(), 100 + (i * (Game::ScreenWidth/2)), static_cast<int>(Game::ScreenHeight/6.5), 30, c);
    }
    int x = 100;
    int y = (int)(Game::ScreenHeight / 3.0f);

    DrawText("Bewegung  - W/A/S/D", x, y + 45, 25, RED);
    DrawText("Angriff   - SPACE", x, y + 95, 25, RED);
    DrawText("Dash    - SHIFT", x, y + 145, 25, RED);

    // Volume UI
    // Position slider unter dem "Volume" Header auf der rechten Seite
    int volHeaderX = 100 + (1 * (Game::ScreenWidth / 2));
    int sliderX = volHeaderX - 160; // noch weiter links
    int sliderY = static_cast<int>(Game::ScreenHeight / 6.5) + 220; // noch weiter nach unten
    int sliderW = 320; // etwas schmaler, damit es rechts gut passt
    int sliderH = 24;

    DrawText("Master Volume", sliderX, sliderY - 30, 20, BLACK);

    // background bar
    DrawRectangle(sliderX, sliderY, sliderW, sliderH, GRAY);
    // filled portion
    int filled = static_cast<int>(masterVolume * static_cast<float>(sliderW));
    DrawRectangle(sliderX, sliderY, filled, sliderH, GREEN);
    // border
    DrawRectangleLines(sliderX, sliderY, sliderW, sliderH, BLACK);

    // volume percentage
    DrawText(TextFormat("%.0f%%", masterVolume * 100.0f), sliderX + sliderW + 10, sliderY, 20, BLACK);

    if (muted) {
        DrawText("(Muted)", sliderX + sliderW + 80, sliderY, 20, RED);
    }

    // Controls hint (links bleibt Steuerung, rechts der Slider)
    DrawText("A/D = vol, M = mute", sliderX, sliderY + 40, 14, DARKGRAY);
}

void Options::SetMasterVolume(float v) {
    float old = masterVolume;
    masterVolume = (v < 0.0f ? 0.0f : (v > 1.0f ? 1.0f : v));
    TraceLog(LOG_INFO, "Options::SetMasterVolume old=%.2f new=%.2f", old, masterVolume);
}

void Options::ToggleMute() {
    muted = !muted;
    TraceLog(LOG_INFO, "Options::ToggleMute muted=%d", muted);
}
