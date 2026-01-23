//
// Created by benco on 20.01.2026.
//

#include "MainMenu_UI.h"

#include <config.h>

MainMenu::MainMenu() {
    options = {"Spiel Starten","Optionen", "Beenden"};
    selectedItem = 0;
    choice = -1;
}

void MainMenu::Update() {
    if (IsKeyPressed(KEY_DOWN)) selectedItem = (selectedItem + 1) % 3;
    if (IsKeyPressed(KEY_UP)) selectedItem = (selectedItem - 1 + 3) % 3;

    if (IsKeyPressed(KEY_ENTER)) {
        choice = selectedItem; // Setzt 0 für Start, 1 für Exit
    }
}

void MainMenu::Draw() {
    DrawText("Hauptmenü", Game::ScreenWidth/2.5, Game::ScreenHeight/6.5, 40, BLACK);

    for (int i = 0; i < options.size(); i++) {
        Color color = (i == selectedItem) ? RED : BLACK;
        DrawText(options[i].c_str(), 100, 200 + (i * 50), 30, color);
    }
}