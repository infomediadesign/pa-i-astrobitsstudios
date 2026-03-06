

#ifndef RAYLIBSTARTER_ATTACKBIGDASH_H
#define RAYLIBSTARTER_ATTACKBIGDASH_H

#pragma once
#include "raylib.h"
#include <algorithm>     // std::clamp
#include <config.h>      // Game::ScreenWidth/Height
#include "boss/Nightmare/NightmareController/NightmareController.h"

class AttackBigDash {
public:
    void Init();
    bool isActive() const;

    void SetWantsToAttack(bool v);

    void StartBigDash(Vector2 bossPos, Vector2 playerPos);

    // ✅ 改：需要 boss 引用，才能移动 boss 本体
    void Update(float dt, NightmareController &boss, Vector2 playerPos);

    void Draw(Vector2 bossPos) const;

    float CheckDamage(float dt, Vector2 bossPos, Rectangle playerRect);

    // Tunables
    float dashDuration = 0.28f;
    float dashSpeed = 1100.0f;
    float hitBoxSize = 130.0f;
    float damage = 18.0f;

private:
    bool active = false;
    bool wantsToAttack = false;
    float timer = 0.0f;
    Vector2 dir{0, 0};

    Rectangle MakeHitBox(Vector2 center) const;
};


#endif //RAYLIBSTARTER_ATTACKBIGDASH_H