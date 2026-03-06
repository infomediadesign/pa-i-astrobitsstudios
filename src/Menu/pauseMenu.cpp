#pragma once
#include "pauseMenu.h"



#include "config.h"

// ---------------- Constructor ----------------
pauseMenu::pauseMenu() : background{} {
    options = {"Weiter", "Options", "Hauptmenu"};
    choice = -1;
    selectedItem = 0;

    // Load pause menu background (fallback to Main Menu if not available)
    const char* pausePath = "assets/graphics/backgrounds/pause_menupixel_1_unbeschriftet.png";
    const char* fallback = "assets/graphics/backgrounds/Main Menu (1).png";
    background = LoadTexture(pausePath);
    if (background.id == 0) background = LoadTexture(fallback);
}

pauseMenu::~pauseMenu() {
    if (background.id > 0) UnloadTexture(background);
}

void pauseMenu::Update() {


    // Dynamische Anpassung an die Anzahl der Optionen (hier 3)
    if (IsKeyPressed(KEY_S)) selectedItem = (selectedItem + 1) % 3;
    if (IsKeyPressed(KEY_W)) selectedItem = (selectedItem - 1 + 3) % 3;

    if (IsKeyPressed(KEY_ENTER)) {
        choice = selectedItem; // Setzt 0 für Start, 1 für Exit
    }
}

void pauseMenu::Open() {
    // Sichtbar "Weiterspielen" auswählen, wenn das Menü geöffnet wird
    selectedItem = 0;
    choice = -1;
}

void pauseMenu::Draw() {
    // Draw background full-screen
    if (background.id > 0) {
        DrawTexturePro(background,
                       Rectangle{0, 0, (float)background.width, (float)background.height},
                       Rectangle{0, 0, (float)Game::ScreenWidth, (float)Game::ScreenHeight},
                       Vector2{0,0}, 0.0f, WHITE);
    }


    int spacing = 65; // Abstand zwischen Einträgen in Pixel (erhöht von 20 auf 40)
    int baseY = 160;  // Start-y für erste Option
    // 3. Optionen dynamisch zeichnen
    for (int i = 0; i < options.size(); i++) {
        // Text zentrieren
        int textWidth = MeasureText(options[i].c_str(), 30);
        int xPos = Game::ScreenWidth / 2 - textWidth / 2;
        int yPos = baseY + (i * spacing);

        // Farbe wählen: Rot wenn ausgewählt, Weiß wenn nicht
        Color color = (i == selectedItem) ? RED : WHITE;

        // Kleiner Indikator (Pfeil) vor dem ausgewählten Text
        if (i == selectedItem) {
            DrawText("> ", xPos - 30, yPos, 30, RED);
        }

        DrawText(options[i].c_str(), xPos, yPos, 30, color);
    }
}