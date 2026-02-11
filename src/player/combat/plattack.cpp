//
// Created by Ben on 13.01.2026.
//

#include "plattack.h"
#include "raylib.h"

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

    // 1. Das Zentrum des Spielers als Basis
    Vector2 pCenter = {
        playerPos.x + playerSize.width / 2.0f,
        playerPos.y + playerSize.height / 2.0f
    };

    // 2. Positionierung der Textur (dstH) und Hitbox
    // Wir schieben den Startpunkt an den Rand des Spieler-Rechtecks.
    if (rotation == 0) { // RECHTS
        dstH.x = playerPos.x + playerSize.width; // Direkt an die rechte Kante
        dstH.y = pCenter.y;
        hitBox = { dstH.x, dstH.y - (float)texture.height / 2.0f, (float)texture.width, (float)texture.height };
    }
    else if (rotation == 180) { // LINKS
        dstH.x = playerPos.x; // Direkt an die linke Kante
        dstH.y = pCenter.y;
        hitBox = { dstH.x - (float)texture.width, dstH.y - (float)texture.height / 2.0f, (float)texture.width, (float)texture.height };
    }
    else if (rotation == 270) { // OBEN
        dstH.x = pCenter.x;
        dstH.y = playerPos.y; // Direkt an die obere Kante
        // Hitbox ist hochkant: Breite = Textur-Höhe, Höhe = Textur-Breite
        hitBox = { dstH.x - (float)texture.height / 2.0f, dstH.y - (float)texture.width, (float)texture.height, (float)texture.width };
    }
    else if (rotation == 90) { // UNTEN
        dstH.x = pCenter.x;
        dstH.y = playerPos.y + playerSize.height; // Direkt an die untere Kante
        hitBox = { dstH.x - (float)texture.height / 2.0f, dstH.y, (float)texture.height, (float)texture.width };
    }

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

    // Der Pivot (origin) liegt bei {0, Höhe/2}.
    // Das bedeutet, das Schwert dreht sich um sein Heft.
    Vector2 origin = { 0, (float)texture.height / 2.0f };

    // Wir nutzen DrawTexturePro, damit die Rotation um das Heft (origin) funktioniert.
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

