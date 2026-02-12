//
// Created by Ben on 23.01.2026.
//

#include "Options.h"
#include "config.h"

Options::Options() {
    options={"Steuerung","Volume"};
}
void Options::Update() {

}
void Options::Draw() {
    DrawText("Optionen", Game::ScreenWidth/2.5, 0, 40, BLACK);

    for (int i = 0; i < options.size(); i++) {
        DrawText(options[i].c_str(), 100 + (i* Game::ScreenWidth/2) , Game::ScreenHeight/6.5, 30, BLACK);
    }
    int x = 100;
    int y = (int)(Game::ScreenHeight / 3.0f);

    DrawText("Steuerung:", x, y, 28, BLACK);
    DrawText("W/A/S/D", x, y + 45, 25, RED);
    DrawText("SPACE    - Angriff", x, y + 95, 25, RED);
    DrawText("SHIFT    - Rollen", x, y + 145, 25, RED);
}
