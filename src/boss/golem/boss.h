//
// Created by Ben on 04.12.2025.
//

#pragma once
#include "raylib.h"

struct Enemy {

        Texture2D texture;
        Vector2 pos;

        void Init();
        void Update(float dt);
        void Draw();
        void Unload();
    };
Rectangle GetHitbox() const
{
    return Rectangle{
            pos.x,
            pos.y,
            (float)texture.width,
            (float)texture.height
    };
}


