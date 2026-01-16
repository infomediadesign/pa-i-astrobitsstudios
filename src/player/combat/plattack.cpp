//
// Created by Ben on 13.01.2026.
//

#include "plattack.h"
#include "raylib.h"

#include "config.h"

void plattack::Init() {
     texture =LoadTexture("assets/graphics/shot.png");
     pos = {0,0};
     damage=10;
     active = false;
     frames;
     frameSpeed;
     frameCount;
     rotation =0;
      dstH = {pos.x+texture.width / 2.0f,pos.y+texture.height/2.0f,(float)texture.width,(float)texture.height};
      srcH = {0,0,(float)texture.width,(float)texture.height};
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
     if (rotation == 180) { // Links
          pos.x = playerPos.x;
          pos.y = playerPos.y + playerSize.height / 2;
     } else if (rotation == 0) { // Rechts
          pos.x = playerPos.x + playerSize.width;
          pos.y = playerPos.y + playerSize.height / 2;
     } else if (rotation == 270) { // Oben
          pos.x = playerPos.x + playerSize.width / 2;
          pos.y = playerPos.y;
     } else if (rotation == 90) { // Unten
          pos.x = playerPos.x + playerSize.width / 2;
          pos.y = playerPos.y + playerSize.height;
     }

     dstH.x = pos.x;
     dstH.y = pos.y;
}


void plattack::Start(Vector2 playerPos, Rectangle playerSize)
{
     active = true;
     lifeTime = 0.15f;
     if (IsKeyDown(KEY_A)) rotation = 180;
     else if (IsKeyDown(KEY_W)) rotation = 270;
     else if (IsKeyDown(KEY_S)) rotation = 90;
     else if (IsKeyDown(KEY_D))rotation = 0; // Standard nach Rechts
     dstH.width = (float)texture.width;
     dstH.height = (float)texture.height;
     Update(0, playerPos, playerSize);
}


void plattack::Draw() {
     if (!active) return;
     Vector2 origin = { 0, (float)texture.height / 2.0f };
     DrawTexturePro(texture,srcH, dstH, origin,rotation, WHITE);
}

