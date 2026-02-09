//
// Created by Ben on 04.12.2025.
//

#include "boss.h"


#include "raylib.h"

#include "config.h"




Rectangle Enemy::GetHitbox() const
{
    return Rectangle{
        pos.x,
        pos.y,
        (float)texture.width,
        (float)texture.height
    };; // or bodyRect; depends on your code
}

    void Enemy::Init()
    {
        texture  = LoadTexture("assets/graphics/Boss/testimage1.png");
        pos = {Game::ScreenWidth/2, Game::ScreenHeight/2};

        health = 100.0f;
        maxHealth = 100.0f;
        hitTimer = 0.0f;
        active = true;
        wasHit= false;
    }

    void Enemy::Update(float dt) {
        if (!active) return;

        //Wenn der Boss getroffen wurde, zählt der Timer runter
        if (hitTimer >0) {
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
        float barWidth = 480.0f;
        DrawText("Titty Frutti der Zweite", Game::ScreenWidth/2.5 +10,Game::ScreenHeight -50, 16,WHITE);
        DrawRectangle((float)Game::ScreenWidth/4, (float)Game::ScreenHeight - 30, barWidth, 20, GRAY);
        DrawRectangle((float)Game::ScreenWidth/4, (float)Game::ScreenHeight - 30, barWidth * (health/maxHealth), 20, GREEN);
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
        return { pos.x, pos.y, (float)texture.width, (float)texture.height };
    }