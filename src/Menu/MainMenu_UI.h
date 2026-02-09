//
// Created by benco on 20.01.2026.
//

#ifndef RAYLIBSTARTER_MENU_UI_H
#define RAYLIBSTARTER_MENU_UI_H
#pragma once

#include "Menu.hpp"

class MainMenu : public Menu {
public:
    MainMenu();
    void Update() override;
    void Draw() override;

    // Hilfsfunktion, um zu wissen, was geklickt wurde
    int GetChoice() { return choice; }
    void ResetChoice() { choice = -1; }

private:
    int choice = -1; // -1 = nichts, 0 = Start, 1 = Exit
};

#endif

 //RAYLIBSTARTER_MENU_UI_H