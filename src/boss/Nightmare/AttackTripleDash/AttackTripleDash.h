//
// Created by yiin-n wang on 2026/3/5.
//

#ifndef RAYLIBSTARTER_ATTACKTRIPLEDASH_H
#define RAYLIBSTARTER_ATTACKTRIPLEDASH_H


#pragma once
#include "raylib.h"
#include <algorithm>
#include <config.h>
#include "boss/Nightmare/NightmareController/NightmareController.h"

class NM_AttackTripleDash {
public:
    void Init();
    void Reset();

    bool IsActive() const;

    // bossPosCenter 传入 boss 中心点
    void Start(Vector2 bossPosCenter);

    // 需要 boss 来移动本体；playerPos 用于 II 的“对齐 + 斜冲”
    void Update(float dt, Vector2 bossPosCenter, Vector2 playerPos, NightmareController &boss);

    void Draw(Vector2 bossPosCenter) const;

    float CheckDamage(float dt, Vector2 bossPosCenter, Rectangle playerRect);

    // ---- tunables ----
    float dashSpeed = 1100.0f;
    float arriveDist = 18.0f;     // 到达目标点的判定距离
    float hitBoxSize = 120.0f;
    float damage = 14.0f;

private:
    bool active = false;
    int phase = 0;               // 0: horizontal, 1: vertical down, 2: diagonal to player
    Vector2 startCenter{};
    Vector2 target{};

    Rectangle MakeHitBox(Vector2 center) const;
    static Vector2 MoveTowards(Vector2 from, Vector2 to, float maxDelta);
};


#endif //RAYLIBSTARTER_ATTACKTRIPLEDASH_H