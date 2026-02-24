#ifndef RAYLIBSTARTER_BOSS_ANGRIFF_H
#define RAYLIBSTARTER_BOSS_ANGRIFF_H

#pragma once
#include "raylib.h"
#include <vector>

#include "AttackJump/AttackJump.h"
#include "AttackRing/AttackRing.h"
#include "AttackSwing/AttackSwing.h"
#include "AttackMeteor/AttackMeteor.h"
#include "AttackSlam/AttackSlam.h"
#include "GolemController/GolemController.h"

struct BossAngriff {
    enum Mode {
        MODE_RING1_TELE = 0,
        MODE_RING1_BURST = 1,
        MODE_WAIT_BETWEEN_RINGS = 2,
        MODE_REST_AFTER_RINGS = 3,
        MODE_SWING = 4,
        MODE_REST_AFTER_SWING = 5,
        MODE_JUMP = 6,
        MODE_SLAM_TELE = 7,
        MODE_SLAM_HIT  = 8,
        MODE_REST_AFTER_SLAM = 9,
        MODE_METEOR_STORM = 100
    };

    Mode mode = MODE_RING1_TELE;
    Mode resumeMode = MODE_RING1_TELE;
    float modeTimer = 0.0f;

    float bossHP = 1.0f;
    float bossMaxHP = 1.0f;
    float meteorTriggerPct = 0.40f;
    float enragedPct = 0.25f;

    float ringTeleDuration = 0.80f;
    float ring1BurstDuration = 1.0f;
    float waitBetweenRings = 2.0f;
    float restAfterRings = 1.0f;
    float swingDuration = 2.2f;
    float restAfterSwing = 1.0f;

    float ring1InnerTele = 50.0f;
    float ring1OuterTele = 100.0f;
    float ring1InnerStart = 50.0f;
    float ring1OuterStart = 100.0f;
    float ring1InnerEnd = 300.0f;
    float ring1OuterEnd = 350.0f;

    float slamTeleDuration = 1.2f;
    float slamHitDuration  = 0.45f;
    float restAfterSlam    = 1.0f;

    float slamDamage = 12.0f;
    float ring1Damage = 5.0f;
    float swingDamage = 8.0f;
    float meteorDamage = 10.0f;
    float dmgTimer = 0.0f;

    // FIX FÜR SCREENSHOT: Diese Variablen müssen da sein
    bool meteorStormTriggered = false;
    bool meteorStormActive = false;

    Vector2 lastPlayerPos{};
    int lastRandomRoll = -1;
    bool debugOverlay = true;

    AttackRing ringAttack;
    AttackSwing swingAttack;
    AttackMeteor meteorAttack;
    AttackJump jumpAttack;
    AttackSlam slamAttack;

    void Update(float dt, Vector2 bossPos, Vector2 playerPos, Rectangle playerRect, Player &player, GolemController &boss);

    void Init();

    void SetBossHP(float hp, float maxHp);

    void Draw(Vector2 bossPos) const;

    float CheckDamage(float dt, Vector2 bossPos, Rectangle playerRect);

    float ModifyIncomingBossDamage(float rawDamage) const;

    bool IsEnraged() const;

private:
    void StartRingTele(Vector2 bossPos, float teleInner, float teleOuter);

    void StartRing1Burst(Vector2 bossPos);

    void StartSwing(Vector2 bossPos, Vector2 playerPos);

    void TryTriggerMeteorStorm();

    void UpdateMeteorStorm(float dt);

    bool AnyAttackActive();

    float DamageMultiplier() const;

    float SpeedMultiplier() const;
};

#endif
