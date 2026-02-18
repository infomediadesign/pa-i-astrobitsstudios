#include "boss_Angriff.h"
#include "config.h"

static void StopAllAttacks(BossAngriff &b) {
    if (b.ringAttack.IsActive()) b.ringAttack.Init();
    if (b.swingAttack.IsActive()) b.swingAttack.Reset();
    if (b.meteorAttack.IsActive()) b.meteorAttack.Reset();

    b.jumpAttack.setDoAttackActive(false);
    b.jumpAttack.setStartAttackActive(false);
    b.jumpAttack.setActive(false);
    if (b.slamAttack.IsActive()) b.slamAttack.Init();
}

bool BossAngriff::AnyAttackActive() {
    return ringAttack.IsActive() || swingAttack.IsActive() || meteorAttack.IsActive() || jumpAttack.isActive() || slamAttack.IsActive();
}

void BossAngriff::Init() {
    mode = MODE_RING1_TELE;
    modeTimer = 0.0f;
    bossHP = 1.0f;
    bossMaxHP = 1.0f;
    ringAttack.Init();
    swingAttack.Init();
    meteorAttack.Init();

    slamAttack.Init();
    slamAttack.markDuration = slamTeleDuration;
    slamAttack.slamDuration = slamHitDuration;
    slamAttack.cooldownDuration = restAfterSlam;

    jumpAttack.setRange(150.0f);
    StopAllAttacks(*this);
    dmgTimer = 0.0f;
    meteorStormTriggered = false;
    meteorStormActive = false;
    lastPlayerPos = {(float) Game::ScreenWidth / 2, (float) Game::ScreenHeight / 2};
}

void BossAngriff::SetBossHP(float hp, float maxHp) {
    bossHP = hp;
    bossMaxHP = (maxHp <= 0.001f) ? 1.0f : maxHp;
}

bool BossAngriff::IsEnraged() const { return (bossHP / bossMaxHP) <= enragedPct; }
float BossAngriff::DamageMultiplier() const { return IsEnraged() ? 1.20f : 1.0f; }
float BossAngriff::SpeedMultiplier() const { return IsEnraged() ? 1.12f : 1.0f; }

float BossAngriff::ModifyIncomingBossDamage(float rawDamage) const {
    return (mode == MODE_METEOR_STORM) ? 1.0f : rawDamage;
}

void BossAngriff::StartRingTele(Vector2 bossPos, float teleInner, float teleOuter) {
    StopAllAttacks(*this);
    ringAttack.StartTele(bossPos, teleInner, teleOuter);
}

void BossAngriff::StartRing1Burst(Vector2 bossPos) {
    StopAllAttacks(*this);
    ringAttack.StartBurst(bossPos, ring1InnerStart, ring1OuterStart, ring1InnerEnd, ring1OuterEnd, ring1BurstDuration,
                          false);
}

void BossAngriff::StartSwing(Vector2 bossPos, Vector2 playerPos) {
    StopAllAttacks(*this);
    swingAttack.Start(bossPos, playerPos);
}

void BossAngriff::Update(float dt, Vector2 bossPos, Vector2 playerPos, Rectangle playerRect, Player &player,
                         Enemy &boss) {
    this->lastPlayerPos = playerPos;
    dmgTimer -= dt;
    if (dmgTimer < 0) dmgTimer = 0;

    //TryTriggerMeteorStorm(); für diesen Boss nicht vorgesehen
    if (mode == MODE_METEOR_STORM) {
        mode = MODE_RING1_TELE;
        return;
    }

    modeTimer += dt;
    float spd = SpeedMultiplier();

    switch (mode) {
        case MODE_RING1_TELE:
            if (!ringAttack.IsActive()) StartRingTele(bossPos, ring1InnerTele, ring1OuterTele);
            ringAttack.Update(dt, bossPos, playerPos);
            if (modeTimer >= ringTeleDuration / spd) {
                mode = MODE_RING1_BURST;
                modeTimer = 0.0f;
                StartRing1Burst(bossPos);
            }
            break;

        case MODE_RING1_BURST:
            ringAttack.Update(dt, bossPos, playerPos);
            if (modeTimer >= ring1BurstDuration / spd) {
                mode = MODE_WAIT_BETWEEN_RINGS;
                modeTimer = 0.0f;
            }
            break;

        case MODE_WAIT_BETWEEN_RINGS:
        case MODE_REST_AFTER_RINGS:
        case MODE_REST_AFTER_SWING:
            if (modeTimer >= waitBetweenRings / spd && !AnyAttackActive()) {
                int r = GetRandomValue(0, 99);
                if (r < 40) {
                    StopAllAttacks(*this);
                    jumpAttack.startAttack(playerPos);
                    mode = MODE_JUMP;
                }
                else if (r < 60) {
                    mode = MODE_RING1_TELE;
                }
                else if (r < 75) {
                    mode = MODE_SWING;
                    StartSwing(bossPos, playerPos);
                }
                else {
                    // Slam
                    StopAllAttacks(*this);
                    slamAttack.markDuration = slamTeleDuration;
                    slamAttack.slamDuration = slamHitDuration;
                    slamAttack.cooldownDuration = restAfterSlam;
                    slamAttack.Start(bossPos);
                    mode = MODE_SLAM_TELE;
                }

                modeTimer = 0.0f;
            }
            break;

        case MODE_JUMP:
            jumpAttack.updateAttackCD(dt);
            jumpAttack.attack(playerPos, playerRect, dt, player, boss);

            // Sobald die Attacke nicht mehr aktiv ist (stopAttack wurde gerufen)
            if (!jumpAttack.isActive()) {
                mode = MODE_REST_AFTER_RINGS; // Wechsel in die Pause
                modeTimer = 0.0f; // WICHTIG: Timer resetten!
            }
            break;

        case MODE_SWING:
            swingAttack.Update(dt, bossPos, playerPos);
            if (modeTimer >= swingDuration / spd) {
                mode = MODE_REST_AFTER_SWING;
                modeTimer = 0.0f;
            }
            break;

        case MODE_SLAM_TELE:
            slamAttack.Update(dt,bossPos, playerPos);
            if (modeTimer >= slamTeleDuration / spd) {
                mode = MODE_SLAM_HIT;
                modeTimer = 0.0f;
            }
            break;

        case MODE_SLAM_HIT:
            slamAttack.Update(dt,bossPos,playerPos);
            if (modeTimer >= slamHitDuration / spd) {
                mode = MODE_REST_AFTER_SLAM;
                modeTimer = 0.0f;
            }
            break;

        case MODE_REST_AFTER_SLAM:
            slamAttack.Update(dt,bossPos,playerPos);
            if (modeTimer >= restAfterSlam / spd && !AnyAttackActive()) {
                mode = MODE_WAIT_BETWEEN_RINGS;  // 或者直接回 MODE_RING1_TELE 都行
                modeTimer = 0.0f;
            }
            break;
        default: mode = MODE_RING1_TELE;
            break;
    }
}

void BossAngriff::Draw(Vector2 bossPos) const {
    if (ringAttack.IsActive()) ringAttack.Draw(bossPos);
    if (swingAttack.IsActive()) swingAttack.Draw(bossPos);
    if (meteorAttack.IsActive()) meteorAttack.Draw(bossPos);

    if (const_cast<AttackSlam&>(slamAttack).IsActive())
        const_cast<AttackSlam&>(slamAttack).Draw(bossPos);

    if (const_cast<AttackJump &>(jumpAttack).isActive()) {
        // Wir übergeben zwar lastPlayerPos, aber die Funktion in AttackJump
        // sollte intern "this->pos" zum Zeichnen benutzen!
        const_cast<AttackJump &>(jumpAttack).startAttackDraw(lastPlayerPos);
        const_cast<AttackJump &>(jumpAttack).doAttackDraw(lastPlayerPos);
    }
}

float BossAngriff::CheckDamage(float dt, Vector2 bossPos, Rectangle playerRect) {
    if (dmgTimer > 0.0f) return 0.0f;
    float mult = DamageMultiplier();

    if (ringAttack.CheckDamage(dt, bossPos, playerRect) > 0) {
        dmgTimer = 0.5f;
        return ring1Damage * mult;
    }
    if (swingAttack.CheckDamage(dt, bossPos, playerRect) > 0) {
        dmgTimer = 0.5f;
        return swingDamage * mult;
    }

    float slamD = slamAttack.CheckDamage(dt, bossPos, playerRect);
    if (slamD > 0.0f) {
        dmgTimer = 0.5f;
        return slamD;
    }

    if (const_cast<AttackJump &>(jumpAttack).isActive() && const_cast<AttackJump &>(jumpAttack).hitPlayer(playerRect)) {
        dmgTimer = 0.5f;
        return (float) const_cast<AttackJump &>(jumpAttack).getDamage() * mult;
    }
    return 0.0f;
}

void BossAngriff::TryTriggerMeteorStorm() {
    float pct = bossHP / bossMaxHP;
    if (!meteorStormTriggered && pct <= meteorTriggerPct) {
        meteorStormTriggered = true;
        meteorStormActive = true;
        StopAllAttacks(*this);
        meteorAttack.Start({0, 0}, lastPlayerPos);
        resumeMode = mode;
        mode = MODE_METEOR_STORM;
    }
}

void BossAngriff::UpdateMeteorStorm(float dt) {
    meteorAttack.Update(dt, {0, 0}, lastPlayerPos);
    if (!meteorAttack.IsActive()) {
        meteorStormActive = false;
        mode = resumeMode;
        modeTimer = 0.0f;
    }
}
