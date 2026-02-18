#pragma once

#include "raylib.h"
#include "../../../player/schaden/schaden.h"
#include "../../golem/GolemController/GolemController.h"

class AttackRing {
public:
    AttackRing();

    void Init();

    void Reset();

    // tele and burst
    void StartTele(Vector2 bossPos, float teleInner, float teleOuter);

    void StartBurst(Vector2 bossPos, float innerStart, float outerStart, float innerEnd, float outerEnd, float duration,
                    bool instant = false);

    void Update(float dt, Vector2 bossPos, Vector2 playerPos);

    void Draw(Vector2 bossPos) const;

    // returns damage or 0.0f (or marker) when hit
    float CheckDamage(float dt, Vector2 bossPos, Rectangle playerRect);

    bool IsActive() const;

    // exposed radii for syncing if needed
    float ringInnerR = 0.0f;
    float ringOuterR = 0.0f;

private:
    bool teleActive = false;
    bool burstActive = false;
    Vector2 center{0, 0};

    float teleInnerR = 0.0f;
    float teleOuterR = 0.0f;

    float burstInnerStart = 0.0f;
    float burstOuterStart = 0.0f;
    float burstInnerEnd = 0.0f;
    float burstOuterEnd = 0.0f;
    float burstDuration = 0.0f;
    float burstTimer = 0.0f;
    float teleTimer = 0.0f;
};
