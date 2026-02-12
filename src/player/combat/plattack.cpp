//
// Created by Ben on 13.01.2026.
//

#include "plattack.h"
#include "raylib.h"
#include <cmath>

#include "config.h"

void plattack::Init() {
     texture =LoadTexture("assets/graphics/Player/shot.png");
      srcH = {0,0,(float)texture.width,(float)texture.height};
     Reset();
}
void plattack::Unload() {
     UnloadTexture(texture);
}
void plattack::Update(float dt, Vector2 playerPos, Rectangle playerSize)
{
    if (!active) return;

    lifeTime -= dt;
    if (lifeTime <= 0.0f) {
        active = false;
        return;
    }

    // FIX: Wir nehmen den absoluten Wert der Breite und Höhe,
    // falls diese durch Spiegelung negativ sind.
    float pW = fabsf(playerSize.width);
    float pH = fabsf(playerSize.height);

    Vector2 attackStart;
    if (rotation == 0) { // Rechts
        attackStart = { playerPos.x + pW, playerPos.y + pH / 2.0f };
        hitBox = { attackStart.x, attackStart.y - (float)texture.height / 2.0f, (float)texture.width, (float)texture.height };
    }
    else if (rotation == 180) { // Links
        attackStart = { playerPos.x, playerPos.y + pH / 2.0f };
        hitBox = { attackStart.x - (float)texture.width, attackStart.y - (float)texture.height / 2.0f, (float)texture.width, (float)texture.height };
    }
    else if (rotation == 270) { // Oben
        attackStart = { playerPos.x + pW / 2.0f, playerPos.y };
        hitBox = { attackStart.x - (float)texture.height / 2.0f, attackStart.y - (float)texture.width, (float)texture.height, (float)texture.width };
    }
    else if (rotation == 90) { // Unten
        attackStart = { playerPos.x + pW / 2.0f, playerPos.y + pH };
        hitBox = { attackStart.x - (float)texture.height / 2.0f, attackStart.y, (float)texture.height, (float)texture.width };
    }

    dstH.x = attackStart.x;
    dstH.y = attackStart.y;
    dstH.width = (float)texture.width;
    dstH.height = (float)texture.height;
}

void plattack::UpdateDirection() {
     if (IsKeyDown(KEY_A)) rotation = 180;
     else if (IsKeyDown(KEY_D)) rotation = 0;
     else if (IsKeyDown(KEY_W)) rotation = 270;
     else if (IsKeyDown(KEY_S)) rotation = 90;
}
void plattack::Start(Vector2 playerPos, Rectangle playerSize)
{
     active = true;
     lifeTime = 0.1f;
     if (IsKeyDown(KEY_A)) rotation = 180;
     else if (IsKeyDown(KEY_D))rotation = 0;
     else if (IsKeyDown(KEY_W)) rotation = 270;
     else if (IsKeyDown(KEY_S)) rotation = 90;
     dstH.width = (float)texture.width;
     dstH.height = (float)texture.height;
     Update(0, playerPos, playerSize);
}


void plattack::Draw() {
    if (!active) return;

    // Origin ist die Mitte der linken Seite der Textur (das "Heft" des Schwerts)
    Vector2 origin = { 0, (float)texture.height / 2.0f };

    // DrawTexturePro nutzt dstH.x/y als Drehpunkt
    DrawTexturePro(texture, srcH, dstH, origin, (float)rotation, WHITE);
}
void plattack::Reset() {
     pos = {0,0};
     damage=10;
     active = false;
     int frames;
     float frameSpeed;
     int frameCount;
     rotation =0;
     dstH = {0,0,(float)texture.width,(float)texture.height};
     hitBox={0,0,0,0};
}

