//
// Created by Ben on 04.12.2025.
//

#include "boss.h"
#include "raylib.h"
#include "config.h"

void Enemy::Init() {
    texture = LoadTexture("assets/graphics/Boss/Boss1_Base_Sprite.png");
    pos = {Game::ScreenWidth / 2, Game::ScreenHeight / 2};

    health = 200.0f;
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
    //DrawTextureV(texture, pos, tint);
    DrawTexture(texture, pos.x - (texture.width / 2), pos.y - (texture.width / 2), tint);

    // OPTIONAL: Lebensbalken zeichnen
    float barWidth = 400.0f;
    DrawText("Tomb Golem",(float) Game::ScreenWidth/2.2,(float) Game::ScreenHeight-45, 18,LIGHTGRAY);
    DrawRectangle((float) Game::ScreenWidth/2 - (barWidth / 2), (float) Game::ScreenHeight-25, barWidth, 20, DARKGRAY);
    DrawRectangle((float) Game::ScreenWidth/2 - (barWidth / 2), (float) Game::ScreenHeight-25, barWidth * (health / maxHealth), 20, GREEN);
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
    return {pos.x - 62.5f, pos.y - 25, 125, 75};
}

Rectangle Enemy::GetDmgBox() {
    return {pos.x - 75, pos.y - 75, 150, 150};
}

void Enemy::setPos(Vector2 position) {
    this->pos = position;

}