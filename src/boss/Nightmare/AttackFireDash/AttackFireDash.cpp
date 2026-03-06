//
// Created by yiin-n wang on 2026/3/5.
//

#include "AttackFireDash.h"
#include <cmath>

static float Len(Vector2 v){ return std::sqrt(v.x*v.x + v.y*v.y); }
static Vector2 Sub(Vector2 a, Vector2 b){ return {a.x-b.x, a.y-b.y}; }

Vector2 NM_AttackFireDash::MoveTowards(Vector2 from, Vector2 to, float maxDelta) {
    Vector2 d = Sub(to, from);
    float l = Len(d);
    if (l <= maxDelta || l < 0.0001f) return to;
    return { from.x + d.x / l * maxDelta, from.y + d.y / l * maxDelta };
}

void NM_AttackFireDash::Init() {
    active = false;
    phase = 0;
    p0=p1=p2=p3=target={0,0};
    trails.clear();
}

void NM_AttackFireDash::Reset() { Init(); }

bool NM_AttackFireDash::IsActive() const { return active; }

void NM_AttackFireDash::Start(Vector2 bossPosCenter, Vector2 playerPos) {
    (void)playerPos;

    active = true;
    phase = 0;
    trails.clear();

    // 按 PDF III：大三角路线  [oai_citation:1‡NightmareAttackPattern.pdf](file-service://file_00000000ff8c71f49f4acd2422e42245)
    // 这里用屏幕边界的 margin 生成三个顶点：左上、右上、下中，再回左上
    float marginX = 70.0f;
    float marginY = 90.0f;

    p0 = bossPosCenter; // 从当前点起跑（更符合你战斗中触发）
    p1 = { (float)Game::ScreenWidth - marginX, marginY };                    // 右上
    p2 = { (float)Game::ScreenWidth * 0.50f, (float)Game::ScreenHeight - 90.0f }; // 下中
    p3 = { marginX, marginY };                                              // 左上

    target = p1;
}

void NM_AttackFireDash::AddTrail(Vector2 fromC, Vector2 toC) {
    // 用包围盒近似线段 trail（最小实现）
    float left   = std::min(fromC.x, toC.x) - trailWidth * 0.5f;
    float right  = std::max(fromC.x, toC.x) + trailWidth * 0.5f;
    float top    = std::min(fromC.y, toC.y) - trailWidth * 0.5f;
    float bottom = std::max(fromC.y, toC.y) + trailWidth * 0.5f;

    TrailSeg seg;
    seg.rect = { left, top, right-left, bottom-top };
    seg.ttl = trailLife;
    trails.push_back(seg);
}

void NM_AttackFireDash::Update(float dt, Vector2 bossPosCenter, Vector2 playerPos, NightmareController &boss) {
    (void)playerPos;
    if (!active) return;

    // 更新 trail 寿命
    for (auto &t : trails) t.ttl -= dt;
    trails.erase(std::remove_if(trails.begin(), trails.end(),
                                [](const TrailSeg& t){ return t.ttl <= 0.0f; }),
                 trails.end());

    Rectangle br = boss.GetRect();
    float halfW = br.width * 0.5f;
    float halfH = br.height * 0.5f;

    // 选择当前目标点：p1 -> p2 -> p3 -> 结束（形成三角）  [oai_citation:0‡NightmareAttackPattern.pdf](file-service://file_00000000ff8c71f49f4acd2422e42245)
    if (phase == 0) target = p1;
    else if (phase == 1) target = p2;
    else if (phase == 2) target = p3;
    else { active = false; return; }

    Vector2 newC = MoveTowards(bossPosCenter, target, dashSpeed * dt);

    // 记录火 trail（从 bossPosCenter 到 newC）
    AddTrail(bossPosCenter, newC);

    // clamp
    newC.x = std::clamp(newC.x, halfW, (float)Game::ScreenWidth  - halfW);
    newC.y = std::clamp(newC.y, halfH, (float)Game::ScreenHeight - halfH);

    boss.setPos(newC);

    // 到达则进入下一段
    if (Len(Sub(target, newC)) <= arriveDist) {
        phase++;
    }
}

void NM_AttackFireDash::Draw(Vector2 bossPosCenter) const {
    // 1. 先画所有残留火焰轨迹
    for (const auto &t : trails) {
        float a = std::clamp(t.ttl / trailLife, 0.0f, 1.0f);

        // 火焰主体
        DrawRectangleRec(t.rect, Fade(WHITE, 0.35f * a));

        // 外框让玩家更容易看见
        DrawRectangleLinesEx(t.rect, 2.0f, Fade(RED, 0.6f * a));
    }

    // 2. 如果攻击已经结束，就不要再画 boss dash 本体提示
    //if (!active) return;

    // 3. 当前 dash 本体可视化
    DrawCircleV(bossPosCenter, 10.0f,WHITE);

    // 4. 调试时可显示当前目标点
    DrawCircleV(target, 6.0f, RED);
}

float NM_AttackFireDash::CheckDamage(float dt, Vector2 bossPosCenter, Rectangle playerRect) {
    (void)dt;
    float dmg = 0.0f;

    if (active) {
        // dash 本体：用一个小圈做简单判定（最小）
        Rectangle dashRect = { bossPosCenter.x - 40, bossPosCenter.y - 40, 80, 80 };
        if (CheckCollisionRecs(dashRect, playerRect)) dmg += dashDamage;
    }

    // trail 伤害：踩到任何 trail 就给一次伤害（你外层有 dmgTimer，会自动限频）
    for (const auto &t : trails) {
        if (CheckCollisionRecs(t.rect, playerRect)) {
            dmg += trailDamage;
            break;
        }
    }
    return dmg;
}