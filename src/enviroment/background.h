//
// Created by benco on 09.02.2026.
//

#ifndef RAYLIBSTARTER_BACKGROUND_H
#define RAYLIBSTARTER_BACKGROUND_H

#pragma once

#include "raylib.h"


struct  background {

    Texture2D texture;
  Vector2 pos;
    int frames;

    void init();

    void Unload();

    void draw();
};


#endif //RAYLIBSTARTER_BACKGROUND_H