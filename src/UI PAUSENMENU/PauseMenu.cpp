#include "PauseMenu.h"

void PauseMenu::UpdateToggle()
{
    if (IsKeyPressed(KEY_ESCAPE)) {
        isPaused = !isPaused;
    }
}

PauseAction PauseMenu::UpdateInput()
{
    if (!isPaused) return PauseAction::None;

    if (IsKeyPressed(KEY_UP)) {
        selectedIndex--;
        if (selectedIndex < 0) selectedIndex = 2;
    }
    if (IsKeyPressed(KEY_DOWN)) {
        selectedIndex++;
        if (selectedIndex > 2) selectedIndex = 0;
    }

    if (IsKeyPressed(KEY_ENTER) || IsKeyPressed(KEY_KP_ENTER)) {
        if (selectedIndex == 0) return PauseAction::Resume;
        if (selectedIndex == 1) return PauseAction::Restart;
        if (selectedIndex == 2) return PauseAction::Quit;
    }

    return PauseAction::None;
}

void PauseMenu::Draw() const
{
    if (!isPaused) return;

    const int W = Game::ScreenWidth;   // canvas size
    const int H = Game::ScreenHeight;

    DrawRectangle(0, 0, W, H, Color{0, 0, 0, 180});

    int centerX = W / 2;

    const char* title = "PAUSED";
    int titleSize = 50;
    DrawText(title, centerX - MeasureText(title, titleSize) / 2, 140, titleSize, WHITE);

    const char* items[] = { "Continue", "Restart", "Quit" };
    int startY = 240;
    int fontSize = 28;

    for (int i = 0; i < 3; i++) {
        int y = startY + i * 50;
        if (i == selectedIndex) {
            const char* text = TextFormat("> %s <", items[i]);
            DrawText(text, centerX - MeasureText(text, fontSize)/2, y, fontSize, YELLOW);
        } else {
            DrawText(items[i], centerX - MeasureText(items[i], fontSize)/2, y, fontSize, RAYWHITE);
        }
    }

    const char* hint1 = "UP/DOWN to select, ENTER to confirm";
    const char* hint2 = "ESC to pause/resume";
    DrawText(hint1, centerX - MeasureText(hint1, 18)/2, startY + 180, 18, LIGHTGRAY);
    DrawText(hint2, centerX - MeasureText(hint2, 18)/2, startY + 205, 18, LIGHTGRAY);
}