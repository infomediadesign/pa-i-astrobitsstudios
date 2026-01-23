#include "schaden.h"
#include "raylib.h"
#include <algorithm> // for std::clamp


    void Player::Init() {

        maxHp = 100;
        hp = maxHp;

        invincibleDuration = 0.8f; // 0.8s hit cooldown
        invincibleTimer = 0.0f;
    }

    void Player::Update(float dt) {

        // reduce invincibility timer
        if (invincibleTimer > 0.0f) {
            invincibleTimer -= dt;
            if (invincibleTimer < 0.0f) invincibleTimer = 0.0f;
        }

    }

    void Player::TakeDamage(int dmg) {
        // if still invincible, ignore hit
        if (invincibleTimer > 0.0f) return;

        hp -= dmg;
        hp = std::clamp(hp, 0, maxHp);

        // start invincibility
        invincibleTimer = invincibleDuration;
    }

    bool Player::IsDead() const{
        return hp <= 0;
    }
    void Player::Draw(Rectangle box) {
        if (invincibleTimer > 0.0f) {
            // blink: draw only on some frames
            if (((int)(GetTime() * 10) % 2) == 0) {
                DrawRectangleRec(box, BLUE);
            }
        } else {
            // DrawRectangleRec(player.GetCollision(), BLUE);
        }

        // UI: health bar
        DrawHealthBar(20, 40, 250, 20, hp, maxHp);

        if (IsDead()) {
            DrawText("You Died!", 380, 280, 40, MAROON);
        }


    }

 const void Player:: DrawHealthBar(int x, int y, int width, int height, int hp, int maxHp) {
    // background
    DrawRectangle(x, y, width, height, DARKGRAY);

    // hp ratio
    float ratio = (maxHp > 0) ? (float)hp / (float)maxHp : 0.0f;
    ratio = std::clamp(ratio, 0.0f, 1.0f);

    // current hp bar
    int currentWidth = (int)(width * ratio);
    DrawRectangle(x, y, currentWidth, height, RED);

    // border
    DrawRectangleLines(x, y, width, height, BLACK);

    // simple text
    DrawText(TextFormat("HP: %d / %d", hp, maxHp), x, y - 20, 16, WHITE);
}