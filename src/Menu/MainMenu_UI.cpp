//
// Created by benco on 20.01.2026.
//

#include "MainMenu_UI.h"

MainMenu::MainMenu() {
    options = {"Spiel Starten", "Beenden"};
    selectedItem = 0;
    choice = -1;
}

void MainMenu::Update() {
    if (IsKeyPressed(KEY_DOWN)) selectedItem = (selectedItem + 1) % 2;
    if (IsKeyPressed(KEY_UP)) selectedItem = (selectedItem - 1 + 2) % 2;

    if (IsKeyPressed(KEY_ENTER)) {
        choice = selectedItem; // Setzt 0 für Start, 1 für Exit
    }
}

void MainMenu::Draw() {
    DrawText("Hauptmenü", 300, 150, 40, DARKGRAY);

    for (int i = 0; i < options.size(); i++) {
        Color color = (i == selectedItem) ? RED : BLACK;
        DrawText(options[i].c_str(), 100, 200 + (i * 50), 30, color);
    }
}