//
// Created by yiin-n wang on 2026/2/9.
//
#include "boss_Angriff.h"
#include "config.h"
#include <cmath>
#include <algorithm>

static Vector2 NormalizeSafe(Vector2 v) {
    float len = sqrtf(v.x*v.x + v.y*v.y);
    if (len < 0.0001f) return {0,0};
    return { v.x/len, v.y/len };
}

float BossAngriff::Clamp01(float x){
    if (x < 0) return 0;
    if (x > 1) return 1;
    return x;
}

float BossAngriff::Lerp(float a, float b, float t){
    return a + (b - a) * t;
}

float BossAngriff::EaseOut(float t){
    t = Clamp01(t);
    return 1.0f - (1.0f - t) * (1.0f - t);
}

void BossAngriff::Init()
{
    mode = MODE_RING1_TELE;
    resumeMode = MODE_RING1_TELE;
    modeTimer = 0.0f;

    bossHP = 1.0f;
    bossMaxHP = 1.0f;

    ringTeleActive = false;
    ringBurstActive = false;
    ringCenter = {0,0};
    teleInnerR = teleOuterR = 0.0f;
    ringInnerR = ringOuterR = 0.0f;

    swingActive = false;
    swingAngle = 0.0f;
    swingBallPos = {0,0};

    dmgTimer = 0.0f;

    meteors.clear();
    meteorStormTriggered = false;
    meteorStormActive = false;
    meteorStormTimer = 0.0f;
    meteorSpawnTimer = 0.0f;

    lastPlayerPos = { (float)Game::ScreenWidth/2, (float)Game::ScreenHeight/2 };
}

void BossAngriff::SetBossHP(float hp, float maxHp)
{
    bossHP = hp;
    bossMaxHP = (maxHp <= 0.001f) ? 1.0f : maxHp;
}

bool BossAngriff::IsEnraged() const
{
    float pct = bossHP / bossMaxHP;
    return pct <= enragedPct;
}

// Leicht erhöhter Schaden nach Eintritt in den Berserk-Modus (nicht signifikant)
float BossAngriff::DamageMultiplier() const
{
    return IsEnraged() ? 1.20f : 1.0f; // ✅ nach25%×1.2
}

// Nach der Raserei beschleunigt sich das Tempo leicht (das Pendel schwingt schneller, mit kürzeren Intervallen zwischen den Bewegungen).
float BossAngriff::SpeedMultiplier() const
{
    return IsEnraged() ? 1.12f : 1.0f; // Leicht erhöht
}

float BossAngriff::ModifyIncomingBossDamage(float rawDamage) const
{
    if (meteorStormActive) return 1.0f; // ✅ Während des Meteoritenschauers kann der Boss nur einen Treffer einstecken.
    return rawDamage;
}

void BossAngriff::StartRingTele(Vector2 bossPos, float teleInner, float teleOuter)
{
    ringCenter = bossPos;
    ringTeleActive = true;
    ringBurstActive = false;

    teleInnerR = teleInner;
    teleOuterR = teleOuter;
}

void BossAngriff::StartRing1Burst(Vector2 bossPos)
{
    ringCenter = bossPos;
    ringTeleActive = false;
    ringBurstActive = true;

    ringInnerR = ring1InnerStart;
    ringOuterR = ring1OuterStart;
}

void BossAngriff::StartRing2BurstInstant(Vector2 bossPos)
{
    ringCenter = bossPos;
    ringTeleActive = false;
    ringBurstActive = true;

    ringInnerR = ring2InnerInstant;
    ringOuterR = ring2OuterInstant;
}

void BossAngriff::StartSwing(Vector2 bossPos, Vector2 playerPos)
{
    swingActive = true;

    Vector2 dir = NormalizeSafe({ playerPos.x - bossPos.x, playerPos.y - bossPos.y });
    swingAngle = atan2f(dir.y, dir.x);

    swingBallPos = {
        bossPos.x + cosf(swingAngle) * swingArmLen,
        bossPos.y + sinf(swingAngle) * swingArmLen
    };
}

void BossAngriff::TryTriggerMeteorStorm()
{
    float pct = bossHP / bossMaxHP;

    // ✅ Auf 40 % ändern, um einmal auszulösen
    if (!meteorStormTriggered && pct <= meteorTriggerPct)
    {
        meteorStormTriggered = true;
        meteorStormActive = true;
        meteorStormTimer = meteorStormDuration;

        resumeMode = mode;
        mode = MODE_METEOR_STORM;
        modeTimer = 0.0f;

        meteors.clear();
        meteorSpawnTimer = 0.0f;
    }
}

void BossAngriff::SpawnMeteor()
{
    Meteor m;

    // === Mehr Fleischtaube: 70 % Wahrscheinlichkeit, in der Nähe des Spielers zu landen (mit Versatz),
    // 30 % Wahrscheinlichkeit, zufällig auf dem gesamten Bildschirm zu landen. ===
    float roll = (float)GetRandomValue(0, 1000) / 1000.0f;

    float x, y;

    if (roll < meteorNearPlayerChance)
    {
        float ang = (float)GetRandomValue(0, 628) / 100.0f; // 0~6.28
        float rad = (float)GetRandomValue((int)(meteorNearRadius*30), (int)(meteorNearRadius*100)) / 100.0f; // 0.3R~1.0R

        x = lastPlayerPos.x + cosf(ang) * rad;
        y = lastPlayerPos.y + sinf(ang) * rad;
    }
    else
    {
        x = (float)GetRandomValue(40, Game::ScreenWidth  - 40);
        y = (float)GetRandomValue(80, Game::ScreenHeight - 80);
    }

    // Klemme am Bildschirm
    if (x < 40) x = 40;
    if (x > Game::ScreenWidth - 40) x = (float)Game::ScreenWidth - 40;
    if (y < 80) y = 80;
    if (y > Game::ScreenHeight - 80) y = (float)Game::ScreenHeight - 80;

    m.targetPos = { x, y };

    // Unregelmäßig: Zufällige Vorwarnzeit und Radius
    m.warnTime = 0.45f + (float)GetRandomValue(0, 25) / 100.0f; // 0.45~0.70
    m.radius   = 16.0f + (float)GetRandomValue(0, 12);          // 16~28

    m.timer = 0.0f;
    m.falling = false;
    m.active = true;

    meteors.push_back(m);
}

void BossAngriff::UpdateMeteorStorm(float dt)
{
    meteorStormTimer -= dt;
    if (meteorStormTimer <= 0.0f)
    {
        meteorStormActive = false;

        mode = resumeMode;
        modeTimer = 0.0f;

        meteors.clear();
        return;
    }

    // Generationsrhythmus: Unregelmäßig + gelegentlich doppelt
    meteorSpawnTimer -= dt;
    if (meteorSpawnTimer <= 0.0f)
    {
        int count = (GetRandomValue(0, 100) < 38) ? 2 : 1;
        for (int i = 0; i < count; i++) SpawnMeteor();

        float next = (float)GetRandomValue((int)(meteorSpawnMin*1000), (int)(meteorSpawnMax*1000)) / 1000.0f;
        meteorSpawnTimer = next;
    }

    for (auto &m : meteors)
    {
        if (!m.active) continue;

        m.timer += dt;

        if (!m.falling)
        {
            if (m.timer >= m.warnTime)
            {
                m.falling = true;
                m.timer = 0.0f;
            }
        }
        else
        {
            // Das Trefferfenster ist etwas kürzer, wodurch ein intensiveres Gefühl von Druck entsteht,
            // eine Ausweichmöglichkeit bleibt jedoch weiterhin bestehen.
            if (m.timer >= 0.18f)
            {
                m.active = false;
            }
        }
    }

    meteors.erase(
        std::remove_if(meteors.begin(), meteors.end(), [](const Meteor& m){ return !m.active; }),
        meteors.end()
    );
}

void BossAngriff::Update(float dt, Vector2 bossPos, Vector2 playerPos)
{
    lastPlayerPos = playerPos;

    dmgTimer -= dt;
    if (dmgTimer < 0) dmgTimer = 0;

    TryTriggerMeteorStorm();

    if (mode == MODE_METEOR_STORM)
    {
        ringTeleActive = false;
        ringBurstActive = false;
        swingActive = false;

        UpdateMeteorStorm(dt);
        return;
    }

    modeTimer += dt;

    // Leichte Beschleunigung (Rhythmus)
    float spd = SpeedMultiplier();

    switch(mode)
    {
        case MODE_RING1_TELE:
        {
            if (!ringTeleActive) StartRingTele(bossPos, ring1InnerTele, ring1OuterTele);
            ringCenter = bossPos;

            if (modeTimer >= ringTeleDuration / spd) {
                mode = MODE_RING1_BURST;
                modeTimer = 0.0f;
                StartRing1Burst(bossPos);
            }
        } break;

        case MODE_RING1_BURST:
        {
            ringCenter = bossPos;
            ringTeleActive = false;
            ringBurstActive = true;

            float t = EaseOut(modeTimer / (ring1BurstDuration / spd));
            ringOuterR = Lerp(ring1OuterStart, ring1OuterEnd, t);
            ringInnerR = Lerp(ring1InnerStart, ring1InnerEnd, t);

            float minThickness = 30.0f;

            if (ringOuterR < ringInnerR + minThickness)
                ringOuterR = ringInnerR + minThickness;

            if (modeTimer >= ring1BurstDuration / spd) {
                ringBurstActive = false;
                mode = MODE_WAIT_BETWEEN_RINGS;
                modeTimer = 0.0f;
            }
        } break;

        case MODE_WAIT_BETWEEN_RINGS:
        {
            ringTeleActive = false;
            ringBurstActive = false;
            swingActive = false;

            if (modeTimer >= waitBetweenRings / spd) {
                mode = MODE_RING2_TELE;
                modeTimer = 0.0f;
                StartRingTele(bossPos, ring2InnerTele, ring2OuterTele);
            }
        } break;

        case MODE_RING2_TELE:
        {
            ringCenter = bossPos;
            if (!ringTeleActive) StartRingTele(bossPos, ring2InnerTele, ring2OuterTele);

            if (modeTimer >= ringTeleDuration / spd) {
                mode = MODE_RING2_BURST;
                modeTimer = 0.0f;
                StartRing2BurstInstant(bossPos);
            }
        } break;

        case MODE_RING2_BURST:
        {
            ringCenter = bossPos;
            ringTeleActive = false;
            ringBurstActive = true;

            if (modeTimer >= ring2BurstDuration / spd) {
                ringBurstActive = false;
                mode = MODE_REST_AFTER_RINGS;
                modeTimer = 0.0f;
            }
        } break;

        case MODE_REST_AFTER_RINGS:
        {
            ringTeleActive = false;
            ringBurstActive = false;
            swingActive = false;

            float rest = restAfterRings / spd;
            if (modeTimer >= rest) {
                mode = MODE_SWING;
                modeTimer = 0.0f;
                StartSwing(bossPos, playerPos);
            }
        } break;

        case MODE_SWING:
        {
            ringTeleActive = false;
            ringBurstActive = false;
            swingActive = true;

            float swingSpd = swingAngularSpeed * spd; // Wütend und etwas schneller
            swingAngle += swingSpd * dt;

            swingBallPos = {
                bossPos.x + cosf(swingAngle) * swingArmLen,
                bossPos.y + sinf(swingAngle) * swingArmLen
            };

            if (modeTimer >= swingDuration / spd) {
                swingActive = false;
                mode = MODE_REST_AFTER_SWING;
                modeTimer = 0.0f;
            }
        } break;

        case MODE_REST_AFTER_SWING:
        {
            ringTeleActive = false;
            ringBurstActive = false;
            swingActive = false;

            float rest = restAfterSwing / spd;
            if (modeTimer >= rest) {
                mode = MODE_RING1_TELE;
                modeTimer = 0.0f;
                StartRingTele(bossPos, ring1InnerTele, ring1OuterTele);
            }
        } break;

        default:
            mode = MODE_RING1_TELE;
            modeTimer = 0.0f;
            break;
    }
}

void BossAngriff::Draw(Vector2 bossPos) const
{
    // Rundumwarnblinkleuchte (Warnblinker)
    if (ringTeleActive)
    {
        float u = Clamp01(modeTimer / ringTeleDuration);
        bool bright = (u < 0.25f) || (u >= 0.5f && u < 0.75f);
        float alpha = bright ? 0.95f : 0.35f;

        DrawRingLines(ringCenter, teleInnerR, teleOuterR, 0.0f, 360.0f, 72, Fade(YELLOW, alpha));
        DrawCircleLines((int)ringCenter.x, (int)ringCenter.y, teleOuterR, Fade(YELLOW, alpha * 0.85f));
        DrawCircleLines((int)ringCenter.x, (int)ringCenter.y, teleInnerR, Fade(YELLOW, alpha * 0.85f));
    }

    // Kreisförmiger Ausbruch
    if (ringBurstActive)
    {
        DrawRing(ringCenter, ringInnerR, ringOuterR, 0.0f, 360.0f, 72, Fade(RED, 0.18f));
        DrawCircleLines((int)ringCenter.x, (int)ringCenter.y, ringOuterR, Fade(RED, 0.95f));
        DrawCircleLines((int)ringCenter.x, (int)ringCenter.y, ringInnerR, Fade(RED, 0.95f));
    }

    // Pendel
    if (swingActive)
    {
        DrawLineEx(bossPos, swingBallPos, 10.0f, GRAY);
        DrawCircleV(swingBallPos, swingBallRadius, MAROON);
        DrawCircleLines((int)swingBallPos.x, (int)swingBallPos.y, swingBallRadius + 6.0f, Fade(ORANGE, 0.55f));
    }

    // Meteorschauer
    if (meteorStormActive)
    {
        for (const auto &m : meteors)
        {
            if (!m.active) continue;

            if (!m.falling)
            {
                float blink = (fmodf(m.timer * 8.0f, 1.0f) < 0.5f) ? 0.9f : 0.35f;
                DrawCircleLines((int)m.targetPos.x, (int)m.targetPos.y, m.radius + 10.0f, Fade(ORANGE, blink));
                DrawCircleV(m.targetPos, m.radius, Fade(ORANGE, 0.12f));
            }
            else
            {
                Vector2 top = { m.targetPos.x, 0.0f };
                DrawLineEx(top, m.targetPos, 6.0f, Fade(GRAY, 0.75f));
                DrawCircleV(m.targetPos, m.radius, Fade(DARKGRAY, 0.85f));
            }
        }
    }
}

// ---- rect vs ring ----
static float PointRectMinDistSq(Vector2 p, Rectangle r){
    float cx = (p.x < r.x) ? r.x : (p.x > r.x + r.width) ? (r.x + r.width) : p.x;
    float cy = (p.y < r.y) ? r.y : (p.y > r.y + r.height) ? (r.y + r.height) : p.y;
    float dx = p.x - cx;
    float dy = p.y - cy;
    return dx*dx + dy*dy;
}
static float PointRectMaxDistSq(Vector2 p, Rectangle r){
    Vector2 c1{r.x, r.y};
    Vector2 c2{r.x + r.width, r.y};
    Vector2 c3{r.x, r.y + r.height};
    Vector2 c4{r.x + r.width, r.y + r.height};

    auto dsq = [&](Vector2 c){
        float dx = p.x - c.x;
        float dy = p.y - c.y;
        return dx*dx + dy*dy;
    };

    float d1 = dsq(c1), d2 = dsq(c2), d3 = dsq(c3), d4 = dsq(c4);
    float m = d1;
    if (d2 > m) m = d2;
    if (d3 > m) m = d3;
    if (d4 > m) m = d4;
    return m;
}
bool BossAngriff::RectHitsRing(Rectangle r, Vector2 c, float innerR, float outerR) const
{
    float minD2 = PointRectMinDistSq(c, r);
    float maxD2 = PointRectMaxDistSq(c, r);
    float inner2 = innerR * innerR;
    float outer2 = outerR * outerR;
    return (minD2 <= outer2) && (maxD2 >= inner2);
}

float BossAngriff::CheckDamage(float dt, Vector2 bossPos, Rectangle playerRect)
{
    (void)dt;
    (void)bossPos;

    if (dmgTimer > 0.0f) return 0.0f;

    float mult = DamageMultiplier();

    if (ringBurstActive)
    {
        if (RectHitsRing(playerRect, ringCenter, ringInnerR, ringOuterR))
        {
            dmgTimer = dmgCooldown;
            float base = (mode == MODE_RING2_BURST) ? ring2Damage : ring1Damage;
            return base * mult;
        }
    }

    if (swingActive)
    {
        if (CheckCollisionCircleRec(swingBallPos, swingBallRadius, playerRect))
        {
            dmgTimer = dmgCooldown;
            return swingDamage * mult;
        }
    }

    if (meteorStormActive)
    {
        for (auto &m : meteors)
        {
            if (!m.active || !m.falling) continue;

            if (CheckCollisionCircleRec(m.targetPos, m.radius, playerRect))
            {
                dmgTimer = dmgCooldown;
                return meteorDamage * mult;
            }
        }
    }

    return 0.0f;
}