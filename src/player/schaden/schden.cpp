#include "raylib.h"
#include <algorithm> // for std::clamp

struct Player {
    Rectangle rect;      // position and size
    int hp;
    int maxHp;

    float invincibleTimer;      // time left (seconds)
    float invincibleDuration;   // how long after hit (seconds)

    void Init(float x, float y) {
        rect = { x, y, 40, 40 };
        maxHp = 100;
        hp = maxHp;

        invincibleDuration = 0.8f; // 0.8s hit cooldown
        invincibleTimer = 0.0f;
    }

    void Update(float dt) {
        // reduce invincibility timer
        if (invincibleTimer > 0.0f) {
            invincibleTimer -= dt;
            if (invincibleTimer < 0.0f) invincibleTimer = 0.0f;
        }

        // (Movement is already done by Ben, so you can skip here)
        // Example movement (optional):
        float speed = 200.0f;
        if (IsKeyDown(KEY_A)) rect.x -= speed * dt;
        if (IsKeyDown(KEY_D)) rect.x += speed * dt;
        if (IsKeyDown(KEY_W)) rect.y -= speed * dt;
        if (IsKeyDown(KEY_S)) rect.y += speed * dt;
    }

    void TakeDamage(int dmg) {
        // if still invincible, ignore hit
        if (invincibleTimer > 0.0f) return;

        hp -= dmg;
        hp = std::clamp(hp, 0, maxHp);

        // start invincibility
        invincibleTimer = invincibleDuration;
    }

    bool IsDead() const {
        return hp <= 0;
    }
};

static void DrawHealthBar(int x, int y, int width, int height, int hp, int maxHp) {
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

int main() {
    InitWindow(900, 600, "Player Damage + HP Bar (Simple)");
    SetTargetFPS(60);

    Player player;
    player.Init(200, 200);

    // Example "enemy" rectangle (just for testing)
    Rectangle enemy = { 500, 250, 60, 60 };

    while (!WindowShouldClose()) {
        float dt = GetFrameTime();

        // update
        player.Update(dt);

        // Collision check (player hit by enemy)
        if (CheckCollisionRecs(player.rect, enemy)) {
            player.TakeDamage(10); // lose 10 hp per hit (with cooldown)
        }

        // draw
        BeginDrawing();
        ClearBackground(RAYWHITE);

        // Draw player (blink effect when invincible)
        if (player.invincibleTimer > 0.0f) {
            // blink: draw only on some frames
            if (((int)(GetTime() * 10) % 2) == 0) {
                DrawRectangleRec(player.rect, BLUE);
            }
        } else {
            DrawRectangleRec(player.rect, BLUE);
        }

        DrawRectangleRec(enemy, ORANGE);

        // UI: health bar
        DrawHealthBar(20, 40, 250, 20, player.hp, player.maxHp);

        if (player.IsDead()) {
            DrawText("You Died!", 380, 280, 40, MAROON);
        }

        EndDrawing();
    }

    CloseWindow();
    return 0;
}