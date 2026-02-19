#pragma once

#include "raylib.h"
#include <vector>
#include "../../../player/schaden/schaden.h"
#include "../../golem/GolemController/GolemController.h"

class AttackMeteor {
public:
    AttackMeteor();
    void Init();
    void Reset();
    void Start(Vector2 bossPos, Vector2 playerPos);
    void Update(float dt, Vector2 bossPos, Vector2 playerPos);
    void Draw(Vector2 bossPos) const;
    float CheckDamage(float dt, Vector2 bossPos, Rectangle playerRect);
    bool IsActive() const;

    struct Meteor {
        Vector2 targetPos{0,0};
        float warnTime = 0.55f;
        float timer = 0.0f;
        float radius = 18.0f;
        bool falling = false;
        bool active = true;
    };

    float meteorStormDuration = 10.0f;
    float meteorSpawnMin = 0.08f;
    float meteorSpawnMax = 0.22f;
    float meteorNearPlayerChance = 0.70f;
    float meteorNearRadius = 190.0f;
    float meteorDamage = 10.0f;

private:
    bool active = false;
    float stormTimer = 0.0f;
    float spawnTimer = 0.0f;
    Vector2 lastPlayerPos{0,0};
    std::vector<Meteor> meteors;

    void SpawnMeteor();
};
