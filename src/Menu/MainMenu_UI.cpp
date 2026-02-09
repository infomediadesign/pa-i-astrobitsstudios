//
// Created by benco on 20.01.2026.
//

#include "MainMenu_UI.h"

#include <config.h>

MainMenu::MainMenu() {
    options = {"Spiel Starten","Optionen","Highscores","Beenden"};
    selectedItem = 0;
    choice = -1;
}

void MainMenu::Update() {
    if (IsKeyPressed(KEY_S)) selectedItem = (selectedItem + 1) % 4;
    if (IsKeyPressed(KEY_W)) selectedItem = (selectedItem - 1 + 4) % 4;

    if (IsKeyPressed(KEY_ENTER)) {
        choice = selectedItem; // Setzt 0 für Start, 1 für Exit
    }
}

void MainMenu::Draw() {
    DrawText("Hauptmenu", Game::ScreenWidth/2.5, Game::ScreenHeight/6.5, 40, BLACK);

    for (int i = 0; i < options.size(); i++) {
        Color color = (i == selectedItem) ? RED : BLACK;
        DrawText(options[i].c_str(), 100, 200 + (i * 50), 30, color);
/*
           if (i == selectedItem) {
            DrawText("> ", - 30,300 + (i * 60) , 30, RED);
        }*/
    }
}