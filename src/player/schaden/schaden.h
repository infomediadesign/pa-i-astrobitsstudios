#ifndef PLAYER_H
#define PLAYER_H

#include "raylib.h"

class Player {
public:
    int Gethealth() const { return hp; }

    // Health
    int hp;
    int maxHp;
    bool takeDamage;
    // Damage cooldown
    float invincibleTimer;
    float invincibleDuration;
    float takeDamageTimer;
    float takeDamageDuration;

    // Basic functions
    void Init();

    void Draw(Rectangle);

    void Update(float deltaTime);

    void TakeDamage(int damage);

    const void DrawHealthBar(int x, int y, int width, int height, int hp, int maxHp);

    // State check
    bool IsDead() const;
};

#endif
