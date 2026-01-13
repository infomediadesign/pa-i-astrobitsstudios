//
// Created by Ben on 13.01.2026.
//

#pragma once
#include "raylib.h"


struct controller
{
    Texture2D texture;
    Vector2 pos;
    float speed;
    int frames;
    Rectangle size;
    int frameSpeed;
    int frameCount;


    void Update(float dt);

    void Init();
    void Draw();
    void Animate(float dt);
    void Unload();
    void Dash(float dt);
};



