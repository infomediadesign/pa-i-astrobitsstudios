//
// Created by yiin-n wang on 2026/3/5.
//

#ifndef RAYLIBSTARTER_ATTACKFIREDASH_H
#define RAYLIBSTARTER_ATTACKFIREDASH_H


#pragma once
#include "raylib.h"
#include <algorithm>
#include <vector>
#include <config.h>
#include "boss/Nightmare/NightmareController/NightmareController.h"

class NM_AttackFireDash {
public:
    void Init();
    void Reset();

    bool IsActive() const;

    void Start(Vector2 bossPosCenter, Vector2 playerPos);

    // 移动 boss 本体；同时记录火 trail
    void Update(float dt, Vector2 bossPosCenter, Vector2 playerPos, NightmareController &boss);

    void Draw(Vector2 bossPosCenter) const;

    float CheckDamage(float dt, Vector2 bossPosCenter, Rectangle playerRect);

    // ---- tunables ----
    float dashSpeed = 900.0f;         // 火 dash 稍慢一点
    float arriveDist = 22.0f;
    float dashDamage = 10.0f;         // dash 本体碰撞伤害
    float trailDamage = 6.0f;         // 踩火伤害
    float trailWidth = 46.0f;
    float trailLife = 2.2f;           // 火持续时间（秒）

private:
    struct TrailSeg {
        Rectangle rect;
        float ttl;
    };

    bool active = false;
    int phase = 0;               // 0->1->2->3(结束)
    Vector2 p0{}, p1{}, p2{}, p3{};
    Vector2 target{};

    std::vector<TrailSeg> trails;

    static Vector2 MoveTowards(Vector2 from, Vector2 to, float maxDelta);
    void AddTrail(Vector2 fromC, Vector2 toC);
};

#endif //RAYLIBSTARTER_ATTACKFIREDASH_H