//
// Created by justu on 02/03/2026.
//

#include "nightmare_Angriff.h"
#include "config.h"

static void StopAllAttacks(nightmare_Angriff &b) {
    if (b.BigDashAttack.isActive()) b.BigDashAttack.Init();
}

bool nightmare_Angriff::AnyAttackActive() {
    return BigDashAttack.isActive();
}

void nightmare_Angriff::Init() {
    mode = Mode_BigDashAttack_Tele;
    modeTimer = 0.0f;
    bossHP = 1.0f;
    bossMaxHP = 1.0f;
    BigDashAttack.Init();

    StopAllAttacks(*this);
    dmgTimer = 0.0f;
    meteorStormTriggered = false;
    meteorStormActive = false;
    lastPlayerPos = {(float) Game::ScreenWidth / 2, (float) Game::ScreenHeight / 2};
}

void nightmare_Angriff::SetBossHP(float hp, float maxHp) {
    bossHP = hp;
    bossMaxHP = (maxHp <= 0.001f) ? 1.0f : maxHp;
}

bool nightmare_Angriff::IsEnraged() const { return (bossHP / bossMaxHP) <= enragedPct; }
float nightmare_Angriff::DamageMultiplier() const { return IsEnraged() ? 1.20f : 1.0f; }
float nightmare_Angriff::SpeedMultiplier() const { return IsEnraged() ? 1.12f : 1.0f; }

float nightmare_Angriff::ModifyIncomingBossDamage(float rawDamage) const {
    return (mode == MODE_METEOR_STORM) ? 1.0f : rawDamage;
}

void nightmare_Angriff::StartBigDash(Vector2 bossPos, Vector2 playerPos) {
    StopAllAttacks(*this);
    BigDashAttack.StartBigDash(bossPos, playerPos);
    BigDashAttack.SetWantsToAttack(true);
}

void nightmare_Angriff::Draw(Vector2 BossPos) const {
    switch (mode) {
        case Mode_BigDashAttack_Tele:
            // Tele-visuals could go here
            break;
        case MODE_BigDashAttack_Dash:
            BigDashAttack.Draw(BossPos);
            break;
        default:
            break;
    }
}

void nightmare_Angriff::Update(float dt, Vector2 bossPos, Vector2 playerPos, Rectangle playerRect, Player &player,
                               NightmareController &boss) {
    (void) player; (void) boss; (void) playerRect;
    this->lastPlayerPos = playerPos;

    // dmg cooldown
    dmgTimer -= dt;
    if (dmgTimer < 0) dmgTimer = 0;

    modeTimer += dt;
    float spd = SpeedMultiplier();
    float scaleDt = dt * spd;

    switch (mode) {
        case Mode_BigDashAttack_Tele: {
            if (modeTimer >= ringTeleDuration / spd) {
                StartBigDash(bossPos, playerPos);
                mode = MODE_BigDashAttack_Dash;
                modeTimer = 0.0f;
            }
            break;
        }

        case MODE_BigDashAttack_Dash: {
            BigDashAttack.Update(scaleDt, bossPos, playerPos);
            if (!BigDashAttack.isActive()) {
                mode = MODE_WAIT_BETWEEN_RINGS;
                modeTimer = 0.0f;
            }
            break;
        }

        case MODE_WAIT_BETWEEN_RINGS: {
            if (modeTimer >= waitBetweenRings / spd && !AnyAttackActive()) {
                mode = Mode_BigDashAttack_Tele;
                modeTimer = 0.0f;
            }
            break;
        }

        default:
            // other modes not implemented for Nightmare
            break;
    }
}

float nightmare_Angriff::CheckDamage(float dt, Vector2 bossPos, Rectangle playerRect) {
    if (dmgTimer > 0.0f) return 0.0f;
    float mult = DamageMultiplier();

    float bd = BigDashAttack.CheckDamage(dt, bossPos, playerRect);
    if (bd > 0.0f) {
        dmgTimer = 0.5f;
        return bd * mult;
    }

    return 0.0f;
}

void nightmare_Angriff::TryTriggerMeteorStorm() {
    // Not implemented for this boss; left as placeholder
    (void) meteorTriggerPct;
}

void nightmare_Angriff::UpdateMeteorStorm(float dt) {
    (void) dt;
    // placeholder: no meteor logic implemented for Nightmare
}
