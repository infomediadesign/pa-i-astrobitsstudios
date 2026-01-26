#ifndef RAYLIBSTARTER_PAUSENMENU_H
#define RAYLIBSTARTER_PAUSENMENU_H

#include "Menu.hpp"   //
#include "raylib.h"
#include <vector>
#include <string>

class pauseMenu : public Menu {
public:
    pauseMenu();

    void Update() override;
    void Draw() override;

private:
    // layout values
    int titleFontSize = 50;
    int itemFontSize  = 28;

    // overlay
    Color overlayColor = {0, 0, 0, 180};

    // helpers
    void MoveUp();
    void MoveDown();
};

#endif // RAYLIBSTARTER_PAUSENMENU_H
