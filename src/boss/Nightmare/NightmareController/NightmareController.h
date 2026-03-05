#pragma once
#ifndef RAYLIBSTARTER_NIGHTMARECONTROLLER_H
#define RAYLIBSTARTER_NIGHTMARECONTROLLER_H

#include "../../Boss.h"
#include "raylib.h"

// Golem-spezifische Controller-Klasse (früher Enemy in BossController)
class NightmareController : public Boss {
public:
    NightmareController();

    ~NightmareController() override;

    void Init() override;

    void update(float dt) override;

    void draw() const override;

    void Unload() override;

    void Reset() override;

    void setPos(Vector2 position);

    void takeDamage(float amount) override;

    // expose health fields for compatibility (avoid direct access where possible)
    using Boss::getHealth;
    using Boss::getMaxHealth;
    using Boss::isAlive;
};

#endif //RAYLIBSTARTER_NIGHTMARECONTROLLER_H
