//
// Created by Ben on 23.01.2026.
//

#include "Options.h"
#include "config.h"
#include <algorithm>
#include <raylib.h>
#include <fstream>
#include <sstream>

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

    // Difficulty selection via W/S (works whenever Options is active)
    static int prevDifficultyIndex = -1;
    if (IsKeyPressed(KEY_W)) {
        difficultyIndex = (difficultyIndex - 1 + static_cast<int>(difficulties.size())) % static_cast<int>(difficulties.size());
    }
    if (IsKeyPressed(KEY_S)) {
        difficultyIndex = (difficultyIndex + 1) % static_cast<int>(difficulties.size());
    }

    // If difficulty changed, persist settings
    if (prevDifficultyIndex != difficultyIndex) {
        prevDifficultyIndex = difficultyIndex;
        SaveSettings("settings.txt");
        TraceLog(LOG_INFO, "Options::Update - difficulty changed, saved settings: %d", difficultyIndex);
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
    // Difficulty wird jetzt direkt unterhalb der Volume-Leiste gezeichnet (weiter unten im Code)

    // Volume UI
    // Position slider unter dem "Volume" Header auf der rechten Seite
    int volHeaderX = 100 + (1 * (Game::ScreenWidth / 2));
    int sliderX = volHeaderX - 100; // noch weiter links
    int sliderY = static_cast<int>(Game::ScreenHeight / 6.5) + 100; // noch weiter nach unten
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

    // Controls hint directly under the volume bar
    int controlsY = sliderY + sliderH + 10;
    DrawText("A/D = vol, M = mute", sliderX, controlsY, 14, RED);

    // Difficulty placed further down under the controls hint
    int difficultyY = controlsY + 40;
    DrawText("Difficulty", sliderX, difficultyY, 40, BLACK);

    // Draw difficulty options selectable with W/S
    int optionStartY = difficultyY + 50;
    for (int i = 0; i < static_cast<int>(difficulties.size()); ++i) {
        Color c = (i == difficultyIndex) ? RED : BLACK;
        DrawText(difficulties[i].c_str(), sliderX, optionStartY + i * 36, 28, c);
    }
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

bool Options::LoadSettings(const std::string &path) {
    std::ifstream f(path);
    if (!f.is_open()) return false;
    std::string line;
    while (std::getline(f, line)) {
        std::istringstream iss(line);
        std::string key, val;
        if (!std::getline(iss, key, '=')) continue;
        if (!std::getline(iss, val)) continue;
        if (key == "difficulty") {
            try {
                int d = std::stoi(val);
                if (d >= 0 && d < static_cast<int>(difficulties.size())) difficultyIndex = d;
            } catch (...) {}
        } else if (key == "masterVolume") {
            try { masterVolume = std::stof(val); } catch(...){}
        } else if (key == "muted") {
            muted = (val == "1" || val == "true");
        }
    }
    TraceLog(LOG_INFO, "Options::LoadSettings loaded difficulty=%d volume=%.2f muted=%d", difficultyIndex, masterVolume, muted);
    return true;
}

bool Options::SaveSettings(const std::string &path) const {
    std::ofstream f(path);
    if (!f.is_open()) return false;
    f << "difficulty=" << difficultyIndex << "\n";
    f << "masterVolume=" << masterVolume << "\n";
    f << "muted=" << (muted ? "1" : "0") << "\n";
    TraceLog(LOG_INFO, "Options::SaveSettings saved difficulty=%d volume=%.2f muted=%d", difficultyIndex, masterVolume, muted);
    return true;
}
