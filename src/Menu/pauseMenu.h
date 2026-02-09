#ifndef RAYLIBSTARTER_PAUSENMENU_H
#define RAYLIBSTARTER_PAUSENMENU_H
#pragma once

#include "Menu.hpp"
#include "raylib.h"
#include <vector>
#include <string>

class pauseMenu : public Menu {
public:
    pauseMenu();

    void Update() override;
    void Draw() override;
    void Open();

    // Hilfsfunktion, um zu wissen, was geklickt wurde
    int GetChoice() { return choice; }
    void ResetChoice() { choice = -1; }

private:
    // layout values
    int choice = -1; // -1 = nichts, 0 = Start, 1 = Exit




};

#endif // RAYLIBSTARTER_PAUSENMENU_H
