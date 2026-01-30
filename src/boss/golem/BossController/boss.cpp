//
// Created by Ben on 04.12.2025.
//

#include "boss.h"
#include "raylib.h"
#include "config.h"

void Enemy::Init() {
    texture = LoadTexture("assets/graphics/testimage.png");
    pos = {Game::ScreenWidth / 2, Game::ScreenHeight / 2};

    health = 100.0f;
    maxHealth = 100.0f;
    hitTimer = 0.0f;
    active = true;
    wasHit = false;
}

void Enemy::Update(float dt) {
    if (!active) return;

    //Wenn der Boss getroffen wurde, zählt der Timer runter
    if (hitTimer > 0) {
        hitTimer -= dt;
    }
}

void Enemy::Draw() {
    if (!active) return;

    // DrawTextureV(texture,pos,WHITE);


    // EFFEKT: Wenn hitTimer > 0, färbe die Textur Rot ein
    Color tint = (hitTimer > 0) ? RED : WHITE;
    DrawTextureV(texture, pos, tint);

    // OPTIONAL: Lebensbalken zeichnen
    float barWidth = 60.0f;
    DrawRectangle(pos.x + (texture.width / 2) - (barWidth / 2), pos.y - 15, barWidth, 6, DARKGRAY);
    DrawRectangle(pos.x + (texture.width / 2) - (barWidth / 2), pos.y - 15, barWidth * (health / maxHealth), 6, GREEN);
}

void Enemy::TakeDamage(float amount) {
    health -= amount;
    hitTimer = 0.1f; // Kurz aufblitzen lassen
    wasHit = true;
    if (health <= 0) {
        active = false; // Boss "stirbt"
    }
}


void Enemy::Unload() {
    UnloadTexture(texture);
}

Rectangle Enemy::GetRect() {
    return {pos.x, pos.y, (float) texture.width, (float) texture.height};
}

void Enemy::setPos(float x, float y) {
    pos.x = x;
    pos.y = y;
}