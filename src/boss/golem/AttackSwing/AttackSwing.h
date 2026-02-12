#pragma once

#include "raylib.h"
#include "../../../player/schaden/schaden.h"
#include "../BossController/boss.h"

class AttackSwing {
public:
    AttackSwing();
    void Init();
    void Reset();
    void Start(Vector2 bossPos, Vector2 playerPos);
    void Update(float dt, Vector2 bossPos, Vector2 playerPos);
    void Draw(Vector2 bossPos) const;
    float CheckDamage(float dt, Vector2 bossPos, Rectangle playerRect);
    bool IsActive() const;

    // config
    float swingAngularSpeed = 5.0f;
    float swingArmLen = 200.0f;
    float swingBallRadius = 18.0f;
    float swingDuration = 2.0f;
    float swingDamage = 10.0f;

    Vector2 GetBallPos() const;

    Vector2 swingBallPos{0,0};

private:
    bool active = false;
    float swingAngle = 0.0f;
    float timer = 0.0f;
};

