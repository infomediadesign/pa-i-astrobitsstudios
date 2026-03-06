//
// Created by Ben on 23.01.2026.
//

#include "Options.h"
#include "config.h"
#include <raylib.h>
#include "SFX/audio_globals.h"
#include <fstream>
#include <sstream>

Options::Options() : optionsBackground{} {
    // Try to load a specific options background; fallback to main menu background if not found
    const char* optPath = "assets/graphics/backgrounds/Options_neu_neu.png";
    optionsBackground = LoadTexture(optPath);
}

Options::~Options() {
    if (optionsBackground.id > 0) UnloadTexture(optionsBackground);
}

void Options::Update() {
    // Difficulty selection via W/S remains handled here
    if (IsKeyPressed(KEY_W)) {
        difficultyIndex = (difficultyIndex - 1 + static_cast<int>(difficulties.size())) % static_cast<int>(difficulties.size());
    }
    if (IsKeyPressed(KEY_S)) {
        difficultyIndex = (difficultyIndex + 1) % static_cast<int>(difficulties.size());
    }
}

void Options::Draw() {
    // Draw background
    if (optionsBackground.id > 0) {
        DrawTexturePro(optionsBackground,
                       Rectangle{0, 0, (float)optionsBackground.width, (float)optionsBackground.height},
                       Rectangle{0, 0, (float)Game::ScreenWidth, (float)Game::ScreenHeight},
                       Vector2{0,0}, 0.0f, WHITE);
    }

    // Simple layout: center column
    int centerX = Game::ScreenWidth / 2;
    int startY = static_cast<int>(Game::ScreenHeight / 6.0f);

    // Volume header
    DrawText("Volume", centerX - 60, startY, 40, BLACK);

    // Slider
    int sliderW = 135; // noch einmal 5px verkleinert (war 140)
    int sliderH = 24;
    const int sliderXOffset = 210; // um weitere 5px nach links verschoben
    int sliderX = centerX - sliderW / 2 + sliderXOffset;
    int sliderY = startY + 70; // 5 px weiter nach unten
    DrawRectangle(sliderX, sliderY, sliderW, sliderH, GRAY);
    int filled = static_cast<int>(masterVolume * static_cast<float>(sliderW));
    DrawRectangle(sliderX, sliderY, filled, sliderH, GREEN);
    DrawRectangleLines(sliderX, sliderY, sliderW, sliderH, WHITE);
    if (muted) DrawText("/", sliderX + sliderW + 7, sliderY + 4, 20, RED);
   // DrawText(TextFormat("%.0f%%", masterVolume * 100.0f), sliderX + sliderW + 50, sliderY, 20, WHITE);
    DrawText("A/D = vol, M = mute", sliderX, sliderY + sliderH + 5, 14, RED);
    DrawText("???", sliderX + 35, sliderY + 241, 20, WHITE); // Placeholder for volume percentage (optional)

    // Difficulty header + options

    // Draw the three difficulty options centered directly under the volume slider
    int optionStartY = sliderY + sliderH + 80; // halb so weit wie vorher (140px unter der Leiste)
    const int optionFontSize = 20;
    const int optionSpacing = 47; // Abstand zwischen den Options (erhöht von 48 auf 64)
    for (int i = 0; i < static_cast<int>(difficulties.size()); ++i) {
        Color c = (i == difficultyIndex) ? RED : WHITE;
        int textW = MeasureText(difficulties[i].c_str(), optionFontSize);
        int textX = sliderX + (sliderW / 2) - (textW / 2) - 10; // 10px nach links verschoben
        DrawText(difficulties[i].c_str(), textX, optionStartY + i * optionSpacing, optionFontSize, c);
}
}

void Options::SetMasterVolume(float v) {
    float old = masterVolume;
    masterVolume = (v < 0.0f ? 0.0f : (v > 1.0f ? 1.0f : v));
    // Apply immediately to global audio (raylib master volume)
    ::SetMasterVolume(masterVolume);
    // publish global tracking variable
    g_gameMasterVolume = masterVolume;
    TraceLog(LOG_INFO, "Options::SetMasterVolume old=%.2f new=%.2f", old, masterVolume);
}

void Options::ToggleMute() {
    muted = !muted;
    // publish global muted flag
    g_gameMuted = muted;
    // Apply to global master volume: if muted set to 0, otherwise restore masterVolume
    ::SetMasterVolume(muted ? 0.0f : masterVolume);
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
    // Apply loaded settings to globals so audio starts correctly
    g_gameMasterVolume = masterVolume;
    g_gameMuted = muted;
    ::SetMasterVolume(muted ? 0.0f : masterVolume);
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
