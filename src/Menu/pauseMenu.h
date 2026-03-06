#ifndef RAYLIBSTARTER_PAUSENMENU_H
#define RAYLIBSTARTER_PAUSENMENU_H
#pragma once

#include "Menu.hpp"
#include "raylib.h"

class pauseMenu : public Menu {
public:
    pauseMenu();
    ~pauseMenu() override;

    void Update() override;
    void Draw() override;
    void Open();

private:
    Texture2D background; // Pause background texture
};

#endif // RAYLIBSTARTER_PAUSENMENU_H
