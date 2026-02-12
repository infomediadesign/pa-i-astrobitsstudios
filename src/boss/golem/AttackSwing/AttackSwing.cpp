#include "AttackSwing.h"
#include "raylib.h"
#include <cmath>
#include "config.h"

AttackSwing::AttackSwing() {
}

void AttackSwing::Init() {
    active = false;
    swingAngle = 0.0f;
    swingBallPos = {0, 0};
    timer = 0.0f;
}

void AttackSwing::Reset() { Init(); }

void AttackSwing::Start(Vector2 bossPos, Vector2 playerPos) {
    active = true;
    Vector2 dir = {playerPos.x - bossPos.x, playerPos.y - bossPos.y};
    float len = sqrtf(dir.x * dir.x + dir.y * dir.y);
    if (len > 0.0001f) {
        dir.x /= len;
        dir.y /= len;
    }
    swingAngle = atan2f(dir.y, dir.x);
    swingBallPos = {bossPos.x + cosf(swingAngle) * swingArmLen, bossPos.y + sinf(swingAngle) * swingArmLen};
    timer = 0.0f;
}

void AttackSwing::Update(float dt, Vector2 bossPos, Vector2 playerPos) {
    (void) playerPos;
    if (!active) return;
    swingAngle += swingAngularSpeed * dt;
    swingBallPos = {bossPos.x + cosf(swingAngle) * swingArmLen, bossPos.y + sinf(swingAngle) * swingArmLen};
    timer += dt;
    if (timer >= swingDuration) active = false;
}

void AttackSwing::Draw(Vector2 bossPos) const {
    if (!active) return;
    DrawLineEx(bossPos, swingBallPos, 10.0f, GRAY);
    DrawCircleV(swingBallPos, swingBallRadius, MAROON);
}

float AttackSwing::CheckDamage(float dt, Vector2 bossPos, Rectangle playerRect) {
    (void) dt;
    (void) bossPos;
    if (!active) return 0.0f;
    if (CheckCollisionCircleRec(swingBallPos, swingBallRadius, playerRect)) {
        active = false;
        return swingDamage;
    }
    return 0.0f;
}

bool AttackSwing::IsActive() const { return active; }

Vector2 AttackSwing::GetBallPos() const { return swingBallPos; }
