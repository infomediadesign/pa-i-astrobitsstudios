//
// Created by justu on 02/03/2026.
//



#ifndef RAYLIBSTARTER_NIGHTMARE_ANGRIFF_H
#define RAYLIBSTARTER_NIGHTMARE_ANGRIFF_H

#pragma once

#include "raylib.h"
#include "config.h"

// 你的项目里 Player 与 NightmareController 的路径按实际情况调整
#include "player/schaden/schaden.h"
#include "boss/Nightmare/NightmareController/NightmareController.h"

// ✅ 你新增的最小攻击类
#include "boss/Nightmare/AttackSwing//AttackSwing.h"
#include "boss/Nightmare/AttackTripleDash//AttackTripleDash.h"
#include "boss/Nightmare/AttackFireDash/AttackFireDash.h"

// 你已有的 BigDash 攻击类（按你的项目真实路径 include）
#include "boss/Nightmare/AttackBigDash//AttackBigDash.h"

class nightmare_Angriff {
private:
    // start helpers
    void StartBigDash(Vector2 bossPos, Vector2 playerPos);
    void StartSwing(Vector2 bossPos, Vector2 playerPos);
    void StartTripleDash(Vector2 bossPos);
    void StartFireDash(Vector2 bossPos, Vector2 playerPos);

    bool AnyAttackActive();
    float DamageMultiplier() const;
    float SpeedMultiplier() const;

public:
    enum Mode {
        MODE_BIGDASH_TELE = 0,
        MODE_BIGDASH_DASH = 1,
        MODE_BIGDASH_SWING = 2, // I: dash后接 swing

        MODE_TRIPLE_DASH = 3,   // II
        MODE_FIRE_DASH = 4,     // III
        MODE_SWING = 5,         // IV

        MODE_WAIT = 6
    };

    Mode mode = MODE_BIGDASH_TELE;
    float modeTimer = 0.0f;

    float bossHP = 1.0f;
    float bossMaxHP = 1.0f;
    float enragedPct = 0.25f;

    // --- Tunables (像 Boss1 一样集中参数，后续你只调这里) ---
    float teleDuration = 0.35f;
    float waitBetween  = 0.60f;
    float swingDuration = 0.55f; // 同时会写给 swingAttack.duration

    float dmgTimer = 0.0f;
    Vector2 lastPlayerPos{};

    // --- Attacks ---
    AttackBigDash BigDashAttack;           // I: BigDash
    NM_AttackSwing swingAttack; // I/IV: Swing
    NM_AttackTripleDash tripleDashAttack;// II: 3x random dash
    NM_AttackFireDash fireDashAttack;            // III: 3x long dash + trail



    void Init();
    void SetBossHP(float hp, float maxHp);

    void Update(float dt, Vector2 bossPos, Vector2 playerPos, Rectangle playerRect, Player &player, NightmareController &boss);
    void Draw(Vector2 bossPos) const;

    float CheckDamage(float dt, Vector2 bossPos, Rectangle playerRect);

    bool IsEnraged() const;
};

#endif //RAYLIBSTARTER_NIGHTMARE_ANGRIFF_H


