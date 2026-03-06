//
// Created by yiin-n wang on 2026/3/5.
//
#include "AttackTripleDash.h"
#include <cmath>

static float Len(Vector2 v){ return std::sqrt(v.x*v.x + v.y*v.y); }
static Vector2 Sub(Vector2 a, Vector2 b){ return {a.x-b.x, a.y-b.y}; }

void NM_AttackTripleDash::Init() {
    active = false;
    phase = 0;
    startCenter = {0,0};
    target = {0,0};
}

void NM_AttackTripleDash::Reset() { Init(); }

bool NM_AttackTripleDash::IsActive() const { return active; }

Rectangle NM_AttackTripleDash::MakeHitBox(Vector2 c) const {
    return { c.x - hitBoxSize*0.5f, c.y - hitBoxSize*0.5f, hitBoxSize, hitBoxSize };
}

Vector2 NM_AttackTripleDash::MoveTowards(Vector2 from, Vector2 to, float maxDelta) {
    Vector2 d = Sub(to, from);
    float l = Len(d);
    if (l <= maxDelta || l < 0.0001f) return to;
    return { from.x + d.x / l * maxDelta, from.y + d.y / l * maxDelta };
}

void NM_AttackTripleDash::Start(Vector2 bossPosCenter) {
    active = true;
    phase = 0;
    startCenter = bossPosCenter;
    target = bossPosCenter;
}

void NM_AttackTripleDash::Update(float dt, Vector2 bossPosCenter, Vector2 playerPos, NightmareController &boss) {
    if (!active) return;

    // 当前 boss rect 用来 clamp
    Rectangle br = boss.GetRect();
    float halfW = br.width * 0.5f;
    float halfH = br.height * 0.5f;

    // 计算每一段目标点：符合 PDF II  [oai_citation:2‡NightmareAttackPattern.pdf](file-service://file_00000000ff8c71f49f4acd2422e42245)
    if (phase == 0) {
        // 水平对齐玩家 X： (playerX, bossY)
        target = { playerPos.x, bossPosCenter.y };
    } else if (phase == 1) {
        // 竖直向下：保持 x=playerX，y 往下到一个固定深度（用屏幕 75%）
        float downY = (float)Game::ScreenHeight * 0.75f;
        target = { playerPos.x, downY };
    } else {
        // 斜冲向玩家当前位置
        target = playerPos;
    }

    // 走向目标点
    Vector2 newC = MoveTowards(bossPosCenter, target, dashSpeed * dt);

    // clamp 防出界
    newC.x = std::clamp(newC.x, halfW, (float)Game::ScreenWidth  - halfW);
    newC.y = std::clamp(newC.y, halfH, (float)Game::ScreenHeight - halfH);

    boss.setPos(newC);

    // 判断到达进入下一段
    if (Len(Sub(target, newC)) <= arriveDist) {
        phase++;
        if (phase >= 3) {
            active = false;
        }
    }
}

void NM_AttackTripleDash::Draw(Vector2 bossPosCenter) const {
    if (!active) return;

    Rectangle r = MakeHitBox(bossPosCenter);
    DrawRectangleLinesEx(r, 3.0f, BLACK);

    // 画出当前目标点（调试）
    DrawCircleV(target, 6.0f, RED);
}

float NM_AttackTripleDash::CheckDamage(float dt, Vector2 bossPosCenter, Rectangle playerRect) {
    (void)dt;
    if (!active) return 0.0f;

    Rectangle r = MakeHitBox(bossPosCenter);
    return CheckCollisionRecs(r, playerRect) ? damage : 0.0f;
}