//
// Created by yiin-n wang on 2026/3/5.
//

#ifndef RAYLIBSTARTER_ATTACKSWING_H
#define RAYLIBSTARTER_ATTACKSWING_H
#pragma once
#include "raylib.h"

class NM_AttackSwing {
public:
    void Init();
    void Reset();

    bool IsActive() const;

    // 朝玩家方向的半圆 swing（最小版：用半径圆判定，先跑起来）
    void Start(Vector2 bossPos, Vector2 playerPos);

    void Update(float dt, Vector2 bossPos, Vector2 playerPos);
    void Draw(Vector2 bossPos) const;

    float CheckDamage(float dt, Vector2 bossPos, Rectangle playerRect);

    // tunables
    float duration = 0.55f;
    float radius = 140.0f;
    float damage = 16.0f;

private:
    bool active = false;
    float timer = 0.0f;
    Vector2 dir{};
};



#endif //RAYLIBSTARTER_ATTACKSWING_H