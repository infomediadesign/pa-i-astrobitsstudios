#include "Death_Screen.h"
#include "config.h"
#include <iostream>

Death_Screen::Death_Screen() {
    choice = -1; // Initialisiere die Wahl auf "nichts gewählt"
}

void Death_Screen::Update() {
    // Debug-Test: Wenn du hier ein printf einfügst, siehst du in der Konsole,
    // ob die Funktion überhaupt erreicht wird.
    std::cout << "Death Screen Update läuft..." << std::endl;

    if (IsKeyPressed(KEY_R)) {
        choice = 0;
    }
    if (IsKeyPressed(KEY_M)) {
        choice = 1;
    }
}

void Death_Screen::Draw() {
    // Optional: Ein halbtransparenter Hintergrund macht es schicker
    DrawRectangle(0, 0, Game::ScreenWidth, Game::ScreenHeight, Fade(BLACK, 0.7f));

    DrawText("DU BIST GESTORBEN!", Game::ScreenWidth/2 - 150, 200, 40, RED);

    DrawText("Drücke [R], um erneut zu versuchen", Game::ScreenWidth/2 - 180, 350, 20, WHITE);
    DrawText("Drücke [M], um ins Hauptmenü zu gelangen", Game::ScreenWidth/2 - 180, 390, 20, LIGHTGRAY);
}