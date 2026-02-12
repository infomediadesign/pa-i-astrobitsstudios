#include "AttackRing.h"
#include "raylib.h"

AttackRing::AttackRing() {
}

void AttackRing::Init() {
    teleActive = false;
    burstActive = false;
    center = {0, 0};
    teleInnerR = teleOuterR = 0.0f;
    burstInnerStart = burstOuterStart = burstInnerEnd = burstOuterEnd = 0.0f;
    burstDuration = 0.0f;
    burstTimer = teleTimer = 0.0f;
}

void AttackRing::Reset() { Init(); }

void AttackRing::StartTele(Vector2 bossPos, float teleInner, float teleOuter) {
    center = bossPos;
    teleActive = true;
    burstActive = false;
    teleInnerR = teleInner;
    teleOuterR = teleOuter;
    teleTimer = 0.0f;
}

void AttackRing::StartBurst(Vector2 bossPos, float innerStart, float outerStart, float innerEnd, float outerEnd,
                            float duration, bool instant) {
    center = bossPos;
    teleActive = false;
    burstActive = true;
    burstInnerStart = innerStart;
    burstOuterStart = outerStart;
    burstInnerEnd = innerEnd;
    burstOuterEnd = outerEnd;
    burstDuration = duration;
    burstTimer = instant ? duration : 0.0f;
    ringInnerR = innerStart;
    ringOuterR = outerStart;
}

void AttackRing::Update(float dt, Vector2 bossPos, Vector2 playerPos) {
    (void) playerPos;
    center = bossPos;

    if (teleActive) {
        teleTimer += dt;
    }

    if (burstActive) {
        burstTimer += dt;
        float t = 1.0f - (1.0f - (burstTimer / burstDuration)) * (1.0f - (burstTimer / burstDuration));
        ringOuterR = burstOuterStart + (burstOuterEnd - burstOuterStart) * t;
        ringInnerR = burstInnerStart + (burstInnerEnd - burstInnerStart) * t;

        float minThickness = 30.0f;
        if (ringOuterR < ringInnerR + minThickness)
            ringOuterR = ringInnerR + minThickness;

        if (burstTimer >= burstDuration) {
            burstActive = false;
        }
    }
}

void AttackRing::Draw(Vector2 bossPos) const {
    (void) bossPos;
    if (teleActive) {
        DrawRingLines(center, teleInnerR, teleOuterR, 0.0f, 360.0f, 72, Fade(RED, 0.8f));
        DrawCircleLines((int) center.x, (int) center.y, teleOuterR, Fade(RED, 0.8f));
    }

    if (burstActive) {
        DrawRing(center, ringInnerR, ringOuterR, 0.0f, 360.0f, 72, Fade(RED, 0.18f));
        DrawCircleLines((int) center.x, (int) center.y, ringOuterR, Fade(RED, 0.95f));
    }
}

float AttackRing::CheckDamage(float dt, Vector2 bossPos, Rectangle playerRect) {
    (void) dt;
    (void) bossPos;
    if (!burstActive) return 0.0f;
    // simple collision check: if rect intersects ring area
    float minD2 = 1e12f;
    float maxD2 = 0.0f;
    // approximate by testing rect corners
    Vector2 corners[4] = {
        {playerRect.x, playerRect.y}, {playerRect.x + playerRect.width, playerRect.y},
        {playerRect.x, playerRect.y + playerRect.height},
        {playerRect.x + playerRect.width, playerRect.y + playerRect.height}
    };
    for (int i = 0; i < 4; i++) {
        float dx = corners[i].x - center.x;
        float dy = corners[i].y - center.y;
        float d2 = dx * dx + dy * dy;
        if (d2 < minD2) minD2 = d2;
        if (d2 > maxD2) maxD2 = d2;
    }
    float inner2 = ringInnerR * ringInnerR;
    float outer2 = ringOuterR * ringOuterR;
    if (minD2 <= outer2 && maxD2 >= inner2) {
        burstActive = false;
        return 1.0f;
    }
    return 0.0f;
}

bool AttackRing::IsActive() const { return teleActive || burstActive; }
