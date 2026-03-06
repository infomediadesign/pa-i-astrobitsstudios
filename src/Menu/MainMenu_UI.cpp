//
// Created by benco on 20.01.2026.
//

#include "MainMenu_UI.h"

#include <config.h>

MainMenu::MainMenu() : menuBackground{} {
    options = {"PLAY GAME","OPTIONS","HIGHSCORES","EXIT"};
    selectedItem = 0;
    // Load background
    menuBackground = LoadTexture("assets/graphics/backgrounds/Main_Menu_unbeschriftet.png");
}

MainMenu::~MainMenu() {
    if (menuBackground.id > 0) UnloadTexture(menuBackground);
}

void MainMenu::Update() {
    if (IsKeyPressed(KEY_S)) selectedItem = (selectedItem + 1) % (int)options.size();
    if (IsKeyPressed(KEY_W)) selectedItem = (selectedItem - 1 + (int)options.size()) % (int)options.size();

    if (IsKeyPressed(KEY_ENTER)) {
        choice = selectedItem;
    }
}

void MainMenu::Draw() {
    if (menuBackground.id > 0) {
        DrawTexturePro(menuBackground,
                       Rectangle{0, 0, (float)menuBackground.width, (float)menuBackground.height},
                       Rectangle{0, 0, (float)Game::ScreenWidth, (float)Game::ScreenHeight},
                       Vector2{0,0}, 0.0f, WHITE);
    }



    int optionFontSize = 30;
    // move options slightly (10px down from previous position)
    int yStart = (int)(Game::ScreenHeight * 0.48f) + 6;
    int spacing = 62;
    for (int i = 0; i < options.size(); i++) {
        int textWidth = MeasureText(options[i].c_str(), optionFontSize);
        int x = (Game::ScreenWidth - textWidth) / 2;
        int y = yStart + (i * spacing);
        Color color = (i == selectedItem) ? RED : WHITE;
        DrawText(options[i].c_str(), x, y, optionFontSize, color);
    }
}