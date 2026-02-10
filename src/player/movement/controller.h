//
// Created by Ben on 13.01.2026.
//

#pragma once
#include "raylib.h"

struct Wall;
struct controller
{
    Texture2D texture;
    Vector2 pos;
    float speed;
    int frames;
    Rectangle plcollision;
    Rectangle size;
    int frameSpeed;
    int frameCount;
    bool moving;
    Rectangle GetHitbox() const;
    bool getMoving(){return moving;}
    void setMoving(bool moving){this->moving = moving;}
    void Init();
    void Draw();
    void Animate(float dt);
    void Unload();
    void Dash( const std::vector<Wall>&,float dt);
    Rectangle GetCollision();
    void Update(float dt, const std::vector<Wall>& walls);
    bool Collides(const Rectangle& box, const std::vector<Wall>& walls);
    void Reset();
    Vector2 GetPos() const;
    Rectangle GetSize() const;

};



