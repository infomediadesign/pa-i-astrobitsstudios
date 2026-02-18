

#ifndef RAYLIBSTARTER_ATTACKSLAM_H
#define RAYLIBSTARTER_ATTACKSLAM_H

#pragma once
#include "raylib.h"
#include <vector>

class AttackSlam
{
public:

    AttackSlam();

    void Init();

    void Reset();

    void Start(Vector2 bossPos);

    void Update(float dt, Vector2 bossPos, Vector2 playerPos);

    void Draw(Vector2 bossPos) const;

    float CheckDamage(float dt, Vector2 bossPos, Rectangle playerRect);

    bool IsActive() const;

    // config
    float markDuration = 1.5f;
    float slamDuration = 0.8f;
    float cooldownDuration = 1.5f;

    float length = 190.0f;
    float halfWidth = 115.0f;

    float spikeMaxHeight = 22.0f;
    int spikesPerArea = 16;

private:

    enum Phase
    {
        PHASE_IDLE,
        PHASE_MARK,
        PHASE_SLAM,
        PHASE_COOLDOWN
    };

    Phase phase = PHASE_IDLE;

    float timer = 0.0f;

    bool active = false;

    bool hitApplied = false;

    std::vector<Vector2> dirs;

private:

    void GetTriangle(Vector2 bossPos, Vector2 dir,
                     Vector2& p0, Vector2& p1, Vector2& p2) const;

    bool CircleHitsTriangle(Vector2 center, float radius,
                            Vector2 a, Vector2 b, Vector2 c) const;

    bool PointInTriangle(Vector2 p,
                         Vector2 a, Vector2 b, Vector2 c) const;

    float DistPointSegment(Vector2 p, Vector2 a, Vector2 b) const;

    void DrawTriangleAOE(Vector2 bossPos, Vector2 dir, Color c) const;

    void DrawSpikes(Vector2 bossPos, Vector2 dir, float t) const;
};


#endif //RAYLIBSTARTER_ATTACKSLAM_H