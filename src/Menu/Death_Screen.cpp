#include "Death_Screen.h"
#include "config.h"
#include <iostream>

Death_Screen::Death_Screen() {
    options = {"Erneut versuchen", "Ins Hauptmenu"};
    choice = -1;
    selectedItem = 0;
}

void Death_Screen::Update() {
    // Dynamische Anpassung an die Anzahl der Optionen (hier 2)
    int numOptions = options.size();

    if (IsKeyPressed(KEY_S)) {
        selectedItem = (selectedItem + 1) % numOptions;
    }
    if (IsKeyPressed(KEY_W)) {
        selectedItem = (selectedItem - 1 + numOptions) % numOptions;
    }

    if (IsKeyReleased(KEY_ENTER)) {
        choice = selectedItem;
    }
}

void Death_Screen::Draw() {
    // 1. Hintergrund abdunkeln
    DrawRectangle(0, 0, Game::ScreenWidth, Game::ScreenHeight, Fade(BLACK, 0.8f));

    // 2. Titel anzeigen
    DrawText("DU BIST GESTORBEN!", Game::ScreenWidth / 2 - MeasureText("DU BIST GESTORBEN!", 40) / 2, 150, 40, RED);

    // 3. Optionen dynamisch zeichnen
    for (int i = 0; i < options.size(); i++) {
        // Text zentrieren
        int textWidth = MeasureText(options[i].c_str(), 30);
        int xPos = Game::ScreenWidth / 2 - textWidth / 2;
        int yPos = 300 + (i * 60);

        // Farbe wählen: Rot wenn ausgewählt, Weiß wenn nicht
        Color color = (i == selectedItem) ? RED : RAYWHITE;

        // Kleiner Indikator (Pfeil) vor dem ausgewählten Text
        if (i == selectedItem) {
            DrawText("> ", xPos - 30, yPos, 30, RED);
        }

        DrawText(options[i].c_str(), xPos, yPos, 30, color);
    }
}