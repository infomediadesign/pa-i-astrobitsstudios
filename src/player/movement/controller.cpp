//
// Created by Ben on 13.01.2026.
#include "raylib.h"

#include "config.h"
//
#include <vector>
#include "controller.h"

#include "../../enviroment/walls.h"


void controller::Update(float dt, const std::vector<Wall>& walls)
{
    Vector2 velocity = {0, 0};

    if (IsKeyDown(KEY_W)) velocity.y -= speed;
    if (IsKeyDown(KEY_S)) velocity.y += speed;
    if (IsKeyDown(KEY_A)) velocity.x -= speed;
    if (IsKeyDown(KEY_D)) velocity.x += speed;

    // --- X ACHSE ---
    Rectangle nextX = {
        pos.x + velocity.x,
        pos.y,
        (float)texture.width / 9,
        (float)texture.height / 2
    };

    if (!Collides(nextX, walls))
        pos.x += velocity.x;

    // --- Y ACHSE ---
    Rectangle nextY = {
        pos.x,
        pos.y + velocity.y,
        (float)texture.width / 9,
        (float)texture.height / 2
    };

    if (!Collides(nextY, walls))
        pos.y += velocity.y;

    // Collisionbox nach Bewegung aktualisieren
    plcollision = {
        pos.x,
        pos.y,
        (float)texture.width / 9,
        (float)texture.height / 2
    };
}

void controller::Init()
{
    texture =LoadTexture("assets/graphics/player.png");
    pos = {10, 100};
    speed = 7;
    frames=0;
    frameCount=0;
    frameSpeed = 8;
    size={0.0f,0.0f, (float)texture.width/8,(float)texture.height/2};

}
void controller::Draw()
{
    if (IsKeyDown(KEY_A))
    {
        size ={size.x,0.0f, (float)-texture.width/8,(float)texture.height/2};
    }else if (IsKeyDown(KEY_D)) size ={size.x,0.0f, (float)texture.width/8,(float)texture.height/2};

    DrawTextureRec(texture, size, pos, WHITE);
}
void controller::Animate(float dt)
{
    frameCount++;
    if (frameCount >=(60/frameSpeed)) {
        frameCount =0;
        frames++;
        if (frames>8) frames=0;
        size.x = (float) frames*(float)texture.width/8;
    }

}
void controller::Unload()
{
    UnloadTexture(texture);
}
void controller::Dash(const std::vector<Wall>& walls)
{
    if (!IsKeyPressed(KEY_Q))
        return;

    Rectangle testBox = plcollision;
    Vector2 dashDir = {0, 0};

    if (IsKeyDown(KEY_D)) dashDir = {1, 0};
    if (IsKeyDown(KEY_A)) dashDir = {-1, 0};
    if (IsKeyDown(KEY_W)) dashDir = {0, -1};
    if (IsKeyDown(KEY_S)) dashDir = {0, 1};

    float dashDistance = 350;
    float step = 10;

    for (float i = 0; i < dashDistance; i += step)
    {
        testBox.x += dashDir.x * step;
        testBox.y += dashDir.y * step;

        bool hit = false;
        for (const Wall& w : walls)
        {
            if (CheckCollisionRecs(testBox, w.hitbox))
            {
                hit = true;
                break;
            }
        }

        if (hit)
            break;

        pos.x += dashDir.x * step;
        pos.y += dashDir.y * step;
    }
    plcollision.x = pos.x;
    plcollision.y = pos.y;
}

bool controller::Collides(const Rectangle& box, const std::vector<Wall>& walls)
{
    for (const Wall& w : walls)
    {
        if (CheckCollisionRecs(box, w.hitbox))
            return true;
    }
    return false;
}

Rectangle controller::GetCollision() {
    return plcollision;
}
Vector2 controller::GetPos() const {
    return pos;
}

Rectangle controller::GetSize() const {
    return size;
}