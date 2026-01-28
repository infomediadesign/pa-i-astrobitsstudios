#include "pauseMenu.h"

// ---------------- Constructor ----------------
pauseMenu::pauseMenu()
{
    // Menu options
    options = { "Continue", "Restart", "Quit" };

    selectedItem = 0;
    choice = -1;      // nothing selected
    active = false;   // pause menu starts inactive
}

// ---------------- Helpers ----------------
void pauseMenu::MoveUp()
{
    selectedItem--;
    if (selectedItem < 0)
        selectedItem = (int)options.size() - 1;
}

void pauseMenu::MoveDown()
{
    selectedItem++;
    if (selectedItem >= (int)options.size())
        selectedItem = 0;
}

// ---------------- Update ----------------
void pauseMenu::Update()
{
    // ESC toggles pause menu
    if (IsKeyPressed(KEY_ESCAPE)) {
        active = !active;
        choice = -1; // reset choice when toggling
        return;
    }

    if (!active) return;

    // navigation
    if (IsKeyPressed(KEY_W)) {
        MoveUp();
    }

    if (IsKeyPressed(KEY_S)) {
        MoveDown();
    }

    // confirm
    if (IsKeyPressed(KEY_ENTER) || IsKeyPressed(KEY_KP_ENTER)) {
        choice = selectedItem;   // store result
        active = false;          // close menu after choice
    }
}

// ---------------- Draw ----------------
void pauseMenu::Draw()
{
    if (!active) return;

    const int W = GetScreenWidth();
    const int H = GetScreenHeight();

    // dark overlay
    DrawRectangle(0, 0, W, H, overlayColor);

    // title
    const char* title = "PAUSED";
    int titleWidth = MeasureText(title, titleFontSize);
    DrawText(title, (W - titleWidth) / 2, 140, titleFontSize,BLACK);

    // menu items
    int startY = 240;
    for (int i = 0; i < (int)options.size(); i++) {
        const std::string& textStr = options[i];
        int y = startY + i * 50;

        if (i == selectedItem) {
            const char* text = TextFormat("> %s <", textStr.c_str());
            int w = MeasureText(text, itemFontSize);
            DrawText(text, (W - w) / 2, y, itemFontSize, YELLOW);
        } else {
            int w = MeasureText(textStr.c_str(), itemFontSize);
            DrawText(textStr.c_str(), (W - w) / 2, y, itemFontSize, RAYWHITE);
        }
    }

    // hints
    const char* hint1 = "UP/DOWN to select, ENTER to confirm";
    const char* hint2 = "ESC to pause/resume";

    DrawText(hint1, (W - MeasureText(hint1, 18)) / 2, startY + 180, 18, LIGHTGRAY);
    DrawText(hint2, (W - MeasureText(hint2, 18)) / 2, startY + 205, 18, LIGHTGRAY);
}