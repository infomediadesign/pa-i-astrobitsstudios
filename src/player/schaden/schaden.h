#ifndef PLAYER_H
#define PLAYER_H

#include "raylib.h"

struct Player
{


    // Health
    int hp;
    int maxHp;

    // Damage cooldown
    float invincibleTimer;
    float invincibleDuration;

    // Basic functions
    void Init();
    void Update(float deltaTime);
    void TakeDamage(int damage);

    // State check
    bool IsDead() const;
};

#endif