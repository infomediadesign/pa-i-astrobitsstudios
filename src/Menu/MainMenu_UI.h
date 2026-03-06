//
// Created by benco on 20.01.2026.
//

#ifndef RAYLIBSTARTER_MENU_UI_H
#define RAYLIBSTARTER_MENU_UI_H
#pragma once

#include "Menu.hpp"
#include "raylib.h"

class MainMenu : public Menu {
public:
    MainMenu();
    ~MainMenu() override;

    void Update() override;
    void Draw() override;

private:
    Texture2D menuBackground; // Hintergrundbild für das Hauptmenü
};

#endif

 //RAYLIBSTARTER_MENU_UI_H