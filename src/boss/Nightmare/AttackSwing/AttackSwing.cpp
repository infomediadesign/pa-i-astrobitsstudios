//
// Created by yiin-n wang on 2026/3/5.
//

#include "AttackSwing.h"
#include <cmath>

static float Len(Vector2 v){ return std::sqrt(v.x*v.x + v.y*v.y); }
static Vector2 Norm(Vector2 v){
    float l = Len(v);
    if (l < 0.0001f) return {0,0};
    return {v.x/l, v.y/l};
}

void NM_AttackSwing::Init() {
    active = false;
    timer = 0.0f;
    dir = {0,0};
}

void NM_AttackSwing::Reset() { Init(); }

bool NM_AttackSwing::IsActive() const { return active; }

void NM_AttackSwing::Start(Vector2 bossPos, Vector2 playerPos) {
    active = true;
    timer = 0.0f;
    dir = Norm({playerPos.x - bossPos.x, playerPos.y - bossPos.y});
}

void NM_AttackSwing::Update(float dt, Vector2 bossPos, Vector2 playerPos) {
    if (!active) return;
    (void)bossPos;
    // 允许轻微追方向：让 swing 面向玩家
    dir = Norm({playerPos.x - bossPos.x, playerPos.y - bossPos.y});
    timer += dt;
    if (timer >= duration) active = false;
}

void NM_AttackSwing::Draw(Vector2 bossPos) const {
    if (!active) return;
    DrawCircleLines((int)bossPos.x, (int)bossPos.y, radius, RED);
    // 简单画一条方向线
    DrawLineEx(bossPos, {bossPos.x + dir.x * radius, bossPos.y + dir.y * radius}, 5.0f, Fade(RED, 0.7f));
}

float NM_AttackSwing::CheckDamage(float dt, Vector2 bossPos, Rectangle playerRect) {
    (void)dt;
    if (!active) return 0.0f;

    Vector2 pc = {playerRect.x + playerRect.width/2.0f, playerRect.y + playerRect.height/2.0f};
    float d = Len({pc.x - bossPos.x, pc.y - bossPos.y});
    return (d <= radius) ? damage : 0.0f;
}