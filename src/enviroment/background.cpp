//
// Created by benco on 09.02.2026.
//
#include <vector>
#include "background.h"
#include "config.h"

#include "raylib.h"

void background::init() {
    pos = {Game::ScreenWidth/2, Game::ScreenHeight/2};
    texture = LoadTexture("assets/graphics/backgrounds/Background1_Boss_Room.png");
    Vector2 pos;
    frames=0;
}


void background::Unload() {
    UnloadTexture(texture);
}

void background:: draw () {
    pos = {Game::ScreenWidth/2, Game::ScreenHeight/2};
    texture = LoadTexture("assets/graphics/backgrounds/Background1_Boss_Room.png");
    Vector2 pos;
    frames=0;
}

