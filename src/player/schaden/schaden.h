#ifndef PLAYER_H
#define PLAYER_H

#include "raylib.h"

struct Player
{
    // Position and size
    Rectangle rect;

    // Health
    int hp;
    int maxHp;

    // Damage cooldown
    float invincibleTimer;
    float invincibleDuration;

    // Basic functions
    void Init(float x, float y);
    void Update(float deltaTime);
    void TakeDamage(int damage);

    // State check
    bool IsDead() const;
};

#endif