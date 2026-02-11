#include "AttackMeteor.h"
#include "raylib.h"
#include <algorithm>

AttackMeteor::AttackMeteor() {}

void AttackMeteor::Init() { active = false; stormTimer = 0.0f; spawnTimer = 0.0f; meteors.clear(); }
void AttackMeteor::Reset() { Init(); }

void AttackMeteor::Start(Vector2 bossPos, Vector2 playerPos) {
    (void)bossPos;
    active = true;
    stormTimer = 0.0f;
    spawnTimer = 0.0f;
    lastPlayerPos = playerPos;
}

void AttackMeteor::Update(float dt, Vector2 bossPos, Vector2 playerPos) {
    (void)bossPos; (void)playerPos;
    if (!active) return;

    stormTimer += dt;
    spawnTimer -= dt;
    if (spawnTimer <= 0.0f) {
        SpawnMeteor();
        spawnTimer = (float)GetRandomValue((int)(meteorSpawnMin*1000), (int)(meteorSpawnMax*1000)) / 1000.0f;
    }

    for (auto &m : meteors) {
        if (!m.active) continue;
        m.timer += dt;
        if (!m.falling && m.timer >= m.warnTime) {
            m.falling = true;
            m.timer = 0.0f;
        }
        if (m.falling && m.timer >= 0.4f) {
            m.active = false;
        }
    }

    if (stormTimer >= meteorStormDuration) active = false;
}

void AttackMeteor::Draw(Vector2 bossPos) const {
    (void)bossPos;
    for (auto &m : meteors) {
        if (!m.active) continue;
        if (!m.falling) {
            DrawCircleLines((int)m.targetPos.x, (int)m.targetPos.y, m.radius, Fade(YELLOW, 0.85f));
        } else {
            DrawCircleV(m.targetPos, m.radius, MAROON);
            DrawCircleLines((int)m.targetPos.x, (int)m.targetPos.y, m.radius + 6.0f, Fade(ORANGE, 0.65f));
        }
    }
}

float AttackMeteor::CheckDamage(float dt, Vector2 bossPos, Rectangle playerRect) {
    (void)dt; (void)bossPos;
    for (auto &m : meteors) {
        if (!m.active) continue;
        if (m.falling && CheckCollisionCircleRec(m.targetPos, m.radius, playerRect)) {
            m.active = false;
            return meteorDamage;
        }
    }
    return 0.0f;
}

bool AttackMeteor::IsActive() const { return active || std::any_of(meteors.begin(), meteors.end(), [](const Meteor &m){ return m.active; }); }

void AttackMeteor::SpawnMeteor() {
    Meteor m;
    m.warnTime = 0.55f;
    m.timer = 0.0f;
    m.radius = 18.0f;
    m.falling = false;
    m.active = true;

    bool near = GetRandomValue(0,100) < int(meteorNearPlayerChance*100.0f);
    if (near) {
        float dx = (float)(GetRandomValue(-1000,1000))/1000.0f * meteorNearRadius;
        float dy = (float)(GetRandomValue(-1000,1000))/1000.0f * meteorNearRadius;
        m.targetPos = { lastPlayerPos.x + dx, lastPlayerPos.y + dy };
    } else {
        float x = (float)GetRandomValue(0, GetScreenWidth());
        float y = (float)GetRandomValue(0, GetScreenHeight());
        m.targetPos = { x, y };
    }

    meteors.push_back(m);
}

