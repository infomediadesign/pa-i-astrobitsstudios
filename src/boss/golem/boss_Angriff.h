//
// Created by yiin-n wang on 2026/2/9.
//

#ifndef RAYLIBSTARTER_BOSS_ANGRIFF_H
#define RAYLIBSTARTER_BOSS_ANGRIFF_H

#pragma once
#include "raylib.h"
#include <vector>

struct BossAngriff
{
    enum Mode {
        MODE_RING1_TELE = 0,
        MODE_RING1_BURST = 1,
        MODE_WAIT_BETWEEN_RINGS = 2,
        MODE_RING2_TELE = 3,
        MODE_RING2_BURST = 4,
        MODE_REST_AFTER_RINGS = 5,
        MODE_SWING = 6,
        MODE_REST_AFTER_SWING = 7,

        MODE_METEOR_STORM = 100
    };

    Mode mode = MODE_RING1_TELE;
    Mode resumeMode = MODE_RING1_TELE;
    float modeTimer = 0.0f;

    // === 由 main 每帧喂入 ===
    float bossHP = 1.0f;
    float bossMaxHP = 1.0f;

    // === 阈值 ===
    float meteorTriggerPct = 0.40f;   //  40%触发陨石雨
    float enragedPct       = 0.25f;   //  25%狂暴

    // ===== 时序（基础值）=====
    float ringTeleDuration = 0.80f;
    float ring1BurstDuration = 0.45f;
    float waitBetweenRings = 3.0f;
    float ring2BurstDuration = 0.35f;
    float restAfterRings = 2.2f;
    float swingDuration = 2.2f;
    float restAfterSwing = 2.2f;

    // ===== 圆环状态 =====
    bool ringTeleActive = false;
    bool ringBurstActive = false;

    Vector2 ringCenter{};

    float ring1InnerTele = 90.0f;
    float ring1OuterTele = 165.0f;
    float ring1InnerStart = 78.0f;
    float ring1OuterStart = 132.0f;
    float ring1InnerEnd   = 108.0f;
    float ring1OuterEnd   = 185.0f;

    float ring2InnerTele = 185.0f;
    float ring2OuterTele = 285.0f;
    float ring2InnerInstant = 205.0f;
    float ring2OuterInstant = 305.0f;

    float ringInnerR = 0.0f;
    float ringOuterR = 0.0f;
    float teleInnerR = 0.0f;
    float teleOuterR = 0.0f;

    // ===== 摆锤 =====
    bool  swingActive = false;
    float swingAngle = 0.0f;
    float swingAngularSpeed = 5.0f;
    float swingArmLen = 145.0f;
    float swingBallRadius = 18.0f;
    Vector2 swingBallPos{};

    // ===== 伤害（基础值）=====
    float ring1Damage = 5.0f;
    float ring2Damage = 7.0f;
    float swingDamage = 8.0f;

    float dmgCooldown = 0.35f;
    float dmgTimer = 0.0f;

    // =========================
    // 陨石雨
    // =========================
    struct Meteor {
        Vector2 targetPos{};
        float warnTime = 0.55f;
        float timer = 0.0f;
        float radius = 18.0f;
        bool falling = false;
        bool active = true;
    };

    std::vector<Meteor> meteors;

    bool meteorStormTriggered = false;
    bool meteorStormActive = false;
    float meteorStormDuration = 10.0f;
    float meteorStormTimer = 0.0f;

    float meteorSpawnTimer = 0.0f;
    float meteorSpawnMin = 0.08f;
    float meteorSpawnMax = 0.22f;

    // “更肉鸽”：落点偏向玩家
    float meteorNearPlayerChance = 0.70f; // 70%落在玩家附近
    float meteorNearRadius = 190.0f;      // 玩家附近散布半径
    float meteorDamage = 10.0f;

    // 保存最近玩家坐标（用于SpawnMeteor）
    Vector2 lastPlayerPos{};

    // =========================
    // API
    // =========================
    void Init();
    void SetBossHP(float hp, float maxHp);

    void Update(float dt, Vector2 bossPos, Vector2 playerPos);
    void Draw(Vector2 bossPos) const;

    float CheckDamage(float dt, Vector2 bossPos, Rectangle playerRect);

    // 陨石雨期间：Boss只吃1点伤害
    float ModifyIncomingBossDamage(float rawDamage) const;

    // 狂暴查询：main 用它把 boss “变红”
    bool IsEnraged() const;

private:
    void StartRingTele(Vector2 bossPos, float teleInner, float teleOuter);
    void StartRing1Burst(Vector2 bossPos);
    void StartRing2BurstInstant(Vector2 bossPos);
    void StartSwing(Vector2 bossPos, Vector2 playerPos);

    void TryTriggerMeteorStorm();
    void UpdateMeteorStorm(float dt);
    void SpawnMeteor();

    bool RectHitsRing(Rectangle r, Vector2 c, float innerR, float outerR) const;

    static float Clamp01(float x);
    static float Lerp(float a, float b, float t);
    static float EaseOut(float t);

    float DamageMultiplier() const;   // 狂暴后×1.2
    float SpeedMultiplier() const;    // 狂暴后节奏略快
};

#endif //RAYLIBSTARTER_BOSS_ANGRIFF_H