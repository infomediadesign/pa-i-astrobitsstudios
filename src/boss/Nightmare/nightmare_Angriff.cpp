//
// Created by justu on 02/03/2026.
//

#include "nightmare_Angriff.h"

// ------------------ internal helper ------------------

static void StopAllAttacks(nightmare_Angriff &b) {
    if (b.BigDashAttack.isActive()) b.BigDashAttack.Init();
    if (b.swingAttack.IsActive()) b.swingAttack.Reset();
    if (b.tripleDashAttack.IsActive()) b.tripleDashAttack.Reset();
    if (b.fireDashAttack.IsActive()) b.fireDashAttack.Reset();
}

// ------------------ core ------------------

bool nightmare_Angriff::AnyAttackActive() {
    return BigDashAttack.isActive()
        || swingAttack.IsActive()
        || tripleDashAttack.IsActive()
        || fireDashAttack.IsActive();
}

void nightmare_Angriff::Init() {
    mode = MODE_BIGDASH_TELE;
    modeTimer = 0.0f;

    bossHP = 1.0f;
    bossMaxHP = 1.0f;

    BigDashAttack.Init();
    swingAttack.Init();
    tripleDashAttack.Init();
    fireDashAttack.Init();

    // 对齐参数（可调）
    swingAttack.duration = swingDuration;

    StopAllAttacks(*this);
    dmgTimer = 0.0f;
    lastPlayerPos = {(float)Game::ScreenWidth / 2, (float)Game::ScreenHeight / 2};
}

void nightmare_Angriff::SetBossHP(float hp, float maxHp) {
    bossHP = hp;
    bossMaxHP = (maxHp <= 0.001f) ? 1.0f : maxHp;
}

bool nightmare_Angriff::IsEnraged() const { return (bossHP / bossMaxHP) <= enragedPct; }
float nightmare_Angriff::DamageMultiplier() const { return IsEnraged() ? 1.20f : 1.0f; }
float nightmare_Angriff::SpeedMultiplier() const { return IsEnraged() ? 1.12f : 1.0f; }

// ------------------ Start helpers ------------------

void nightmare_Angriff::StartBigDash(Vector2 bossPos, Vector2 playerPos) {
    StopAllAttacks(*this);
    BigDashAttack.StartBigDash(bossPos, playerPos);
    BigDashAttack.SetWantsToAttack(true); // 你原来就这么做
}

void nightmare_Angriff::StartSwing(Vector2 bossPos, Vector2 playerPos) {
    StopAllAttacks(*this);
    swingAttack.Start(bossPos, playerPos);
}

void nightmare_Angriff::StartTripleDash(Vector2 bossPos) {
    StopAllAttacks(*this);
    tripleDashAttack.Start(bossPos);
}

void nightmare_Angriff::StartFireDash(Vector2 bossPos, Vector2 playerPos) {
    StopAllAttacks(*this);
    fireDashAttack.Start(bossPos, playerPos);
}

// ------------------ Update / Draw / Damage ------------------

void nightmare_Angriff::Update(float dt,
                               Vector2 bossPos,
                               Vector2 playerPos,
                               Rectangle playerRect,
                               Player &player,
                               NightmareController &boss)
{
    (void)playerRect;
    (void)player;

    lastPlayerPos = playerPos;

    dmgTimer -= dt;
    if (dmgTimer < 0) dmgTimer = 0;

    modeTimer += dt;
    float spd = SpeedMultiplier();
    float scaleDt = dt * spd;

    auto pickNext = [&](){
        int r = GetRandomValue(0, 99);
        // 25% BigDash, 30% Triple, 25% Fire, 20% Swing
        if (r < 25) {
            mode = MODE_BIGDASH_TELE;
        } else if (r < 55) {
            StartTripleDash(bossPos);
            mode = MODE_TRIPLE_DASH;
        } else if (r < 80) {
            StartFireDash(bossPos, playerPos);
            mode = MODE_FIRE_DASH;
        } else {
            StartSwing(bossPos, playerPos);
            mode = MODE_SWING;
        }
        modeTimer = 0.0f;
    };

    switch (mode) {
        // I BigDash: tele -> dash -> swing
        case MODE_BIGDASH_TELE:
            if (modeTimer >= teleDuration / spd) {
                StartBigDash(bossPos, playerPos);
                mode = MODE_BIGDASH_DASH;
                modeTimer = 0.0f;
            }
            break;

        case MODE_BIGDASH_DASH:
            BigDashAttack.Update(scaleDt, boss, playerPos);
            if (!BigDashAttack.isActive()) {
                // ✅ Dash结束自动接Swing（你的 I 要求）
                StartSwing(bossPos, playerPos);
                mode = MODE_BIGDASH_SWING;
                modeTimer = 0.0f;
            }
            break;

        case MODE_BIGDASH_SWING:
            swingAttack.Update(scaleDt, bossPos, playerPos);
            if (modeTimer >= swingDuration / spd || !swingAttack.IsActive()) {
                mode = MODE_WAIT;
                modeTimer = 0.0f;
            }
            break;

        // II TripleDash（内部会移动 boss：boss.setPos）
        case MODE_TRIPLE_DASH:
            tripleDashAttack.Update(scaleDt, bossPos, playerPos, boss);
            if (!tripleDashAttack.IsActive()) {
                mode = MODE_WAIT;
                modeTimer = 0.0f;
            }
            break;

        // III FireDash（内部移动 boss + 留火 trail）
        case MODE_FIRE_DASH:
            fireDashAttack.Update(scaleDt, bossPos, playerPos, boss);
            if (!fireDashAttack.IsActive()) {
                mode = MODE_WAIT;
                modeTimer = 0.0f;
            }
            break;

        // IV Swing only
        case MODE_SWING:
            swingAttack.Update(scaleDt, bossPos, playerPos);
            if (modeTimer >= swingDuration / spd || !swingAttack.IsActive()) {
                mode = MODE_WAIT;
                modeTimer = 0.0f;
            }
            break;

        // Rest
        case MODE_WAIT:
            if (modeTimer >= waitBetween / spd && !AnyAttackActive()) {
                pickNext();
            }
            break;

        default:
            mode = MODE_WAIT;
            modeTimer = 0.0f;
            break;
    }
}

void nightmare_Angriff::Draw(Vector2 bossPos) const {
    if (const_cast<AttackBigDash&>(BigDashAttack).isActive())
        const_cast<AttackBigDash&>(BigDashAttack).Draw(bossPos);

    if (const_cast<NM_AttackSwing&>(swingAttack).IsActive())
        const_cast<NM_AttackSwing&>(swingAttack).Draw(bossPos);

    if (const_cast<NM_AttackTripleDash&>(tripleDashAttack).IsActive())
        const_cast<NM_AttackTripleDash&>(tripleDashAttack).Draw(bossPos);

    if (const_cast<NM_AttackFireDash&>(fireDashAttack).IsActive())
        const_cast<NM_AttackFireDash&>(fireDashAttack).Draw(bossPos);
}

float nightmare_Angriff::CheckDamage(float dt, Vector2 bossPos, Rectangle playerRect) {
    if (dmgTimer > 0.0f) return 0.0f;
    float mult = DamageMultiplier();

    float bd = BigDashAttack.CheckDamage(dt, bossPos, playerRect);
    if (bd > 0.0f) { dmgTimer = 0.45f; return bd * mult; }

    float sw = swingAttack.CheckDamage(dt, bossPos, playerRect);
    if (sw > 0.0f) { dmgTimer = 0.45f; return sw * mult; }

    float td = tripleDashAttack.CheckDamage(dt, bossPos, playerRect);
    if (td > 0.0f) { dmgTimer = 0.35f; return td * mult; }

    float fd = fireDashAttack.CheckDamage(dt, bossPos, playerRect);
    if (fd > 0.0f) { dmgTimer = 0.30f; return fd * mult; }

    return 0.0f;
}