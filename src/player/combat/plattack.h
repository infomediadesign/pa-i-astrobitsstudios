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
    int frameSpeed;
    int frameCount;
    int rotation;
    Rectangle dstH;
    Rectangle srcH;
    bool active = false;
    float lifeTime = 0.0f;
    void Init();
    void Draw();
    void Animate(float dt);
    void Unload();
    void Update(float dt, Vector2 playerPos, Rectangle playerSize);
    void Damage();
    void Start(Vector2 playerPos,Rectangle playerSize);
    void Reset();
};


