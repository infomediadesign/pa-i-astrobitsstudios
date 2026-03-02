#pragma once

#include "../../Boss.h"
#include "raylib.h"

// Golem-spezifische Controller-Klasse (früher Enemy in BossController)
class GolemController : public Boss {
public:
    GolemController();
    ~GolemController() override;

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
