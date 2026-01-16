//
// Created by Ben on 13.01.2026.
//

#pragma once
#include "raylib.h"



struct plattack{
    Texture2D texture;
    Vector2 pos;
    float damage;
    int frames;
    Rectangle player;
    Rectangle size;
    int frameSpeed;
    int frameCount;

    void Init();
    void Draw();
    void Animate(float dt);
    void Unload();
    void Update(float dt);
    void Damage();
};


