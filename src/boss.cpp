//
// Created by Ben on 04.12.2025.
//

#include "boss.h"
#include <cstdlib>
#include <algorithm>

#include "raylib.h"

#include "config.h"
#include <math.h>
#include"cooldown.h"
#include <vector>
#include"Sprite.h"




    void Enemy::Init()
    {
        texture  = LoadTexture("assets/graphics/testimage.png");
        pos = {Game::ScreenWidth/2, Game::ScreenHeight/2};
    }

    void Enemy::Update(float dt)
    {

    }
    void Enemy::Draw() {
        DrawTextureV(texture,pos,WHITE);
    }
    void Enemy::Unload() {
        UnloadTexture(texture);
    }
