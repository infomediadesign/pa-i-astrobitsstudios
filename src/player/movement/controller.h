//
// Created by Ben on 13.01.2026.
//

#pragma once
#include "raylib.h"
#include <vector>

struct Wall;
struct controller
{
    Texture2D texture;
    Vector2 pos;
    float speed;
    float dashDistance;
    int frames;
    Rectangle plcollision;
    Rectangle size;
    Vector2 velocity = {0, 0};
    int frameSpeed;
    int frameCount;
    bool moving;
    float gehy;
    float gehx;
    float animTimer; // added: accumulation timer for dt-based animation
    bool animateOnlyMovement; // if true, pressing movement cycles frames but does not change pos
    bool facingRight; // true = looking right (top sprite row), false = looking left (bottom row)

    // Idle animation separate index so idle and run animations don't clobber each other
    int idleFrame;          // current idle frame index
    int idleFrameCount;     // how many frames are used for idle
    int idleFrameSpeed;     // frames-per-second for idle animation (smaller = slower)

    // Static idle texture (shown when player stands still)
    Texture2D idleTexture;

    void SetAnimateOnlyMovement(bool v){ animateOnlyMovement = v; }
    bool GetAnimateOnlyMovement() const { return animateOnlyMovement; }
    void SetFacingRight(bool v){ facingRight = v; }
    bool IsFacingRight() const { return facingRight; }
    Rectangle GetHitbox() const;
    bool getMoving(){return moving;}
    void setMoving(bool moving){this->moving = moving;}
    void Init();
    void DrawAnimation();
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
