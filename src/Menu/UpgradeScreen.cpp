#include "UpgradeScreen.h"
#include "config.h"

UpgradeScreen::UpgradeScreen() {
    options = {"HP+10%/ATK-10%", "HP-10%/SPEED+10%", "ATK+10%/SPEED-10%"};
    selectedItem = 0;
    choice = -1;
}

void UpgradeScreen::Open() {
    selectedItem = 0;
    choice = -1;
    active = true;
}

void UpgradeScreen::Update() {
    // einfache Navigation: W/S um zu wechseln, ENTER zum wählen
    int numOptions = static_cast<int>(options.size());
    if (IsKeyPressed(KEY_S)) selectedItem = (selectedItem + 1) % numOptions;
    if (IsKeyPressed(KEY_W)) selectedItem = (selectedItem - 1 + numOptions) % numOptions;
    if (IsKeyPressed(KEY_ENTER)) choice = selectedItem;
}

void UpgradeScreen::Draw() {
    DrawText("Upgrade Screen", 100, 100, 40, BLACK);
    DrawText("Hier kannst du deine Upgrades auswählen!", 100, 150, 30, DARKGRAY);
    for (int i = 0; i < options.size(); ++i) {
        Color color = (i == selectedItem) ? RED : BLACK;
        int textWidth = MeasureText(options[i].c_str(), 25);
        int x = Game::ScreenWidth/2 - textWidth/2;
        DrawText(options[i].c_str(), x, 250 + i*50, 25, color);
    }
}