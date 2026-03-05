#include "AttackBigDash.h"
#include <cmath>

static float LenV(Vector2 v) { return std::sqrt(v.x*v.x + v.y*v.y); }
static Vector2 NormV(Vector2 v) {
    float l = LenV(v);
    if (l < 0.0001f) return {0, 0};
    return {v.x / l, v.y / l};
}

void AttackBigDash::Init() {
    active = false;
    wantsToAttack = false;
    timer = 0.0f;
    dir = {0, 0};
}

bool AttackBigDash::isActive() const { return active; }

void AttackBigDash::SetWantsToAttack(bool v) { wantsToAttack = v; }

void AttackBigDash::StartBigDash(Vector2 bossPos, Vector2 playerPos) {
    (void)bossPos;
    active = true;
    timer = 0.0f;

    dir = NormV({playerPos.x - bossPos.x, playerPos.y - bossPos.y});
    if (dir.x == 0 && dir.y == 0) dir = {1, 0};
    wantsToAttack = true;
}

Rectangle AttackBigDash::MakeHitBox(Vector2 center) const {
    return Rectangle{center.x - hitBoxSize*0.5f, center.y - hitBoxSize*0.5f, hitBoxSize, hitBoxSize};
}

void AttackBigDash::Update(float dt, NightmareController &boss, Vector2 playerPos) {
    (void)playerPos;
    if (!active) return;

    // 当前 boss 中心
    Rectangle br = boss.GetRect();
    Vector2 c = { br.x + br.width*0.5f, br.y + br.height*0.5f };

    // 移动
    Vector2 newC = { c.x + dir.x * dashSpeed * dt, c.y + dir.y * dashSpeed * dt };

    // 防出界（按 boss 尺寸）
    float halfW = br.width * 0.5f;
    float halfH = br.height * 0.5f;
    newC.x = std::clamp(newC.x, halfW, (float)Game::ScreenWidth  - halfW);
    newC.y = std::clamp(newC.y, halfH, (float)Game::ScreenHeight - halfH);

    // 设置 boss 位置（这里假设 setPos 接收的是“中心点”）
    boss.setPos(newC);

    timer += dt;
    if (timer >= dashDuration) {
        active = false;
        wantsToAttack = false;
    }
}

void AttackBigDash::Draw(Vector2 bossPos) const {
    if (!active) return;
    Rectangle r = MakeHitBox(bossPos);
    DrawRectangleLinesEx(r, 3.0f, RED);
}

float AttackBigDash::CheckDamage(float dt, Vector2 bossPos, Rectangle playerRect) {
    (void)dt;
    if (!active) return 0.0f;
    Rectangle r = MakeHitBox(bossPos);
    return CheckCollisionRecs(r, playerRect) ? damage : 0.0f;
}