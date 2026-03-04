#ifndef PLAYER_H
#define PLAYER_H

#include "raylib.h"

class Player {
public:
    int Gethealth() const { return hp; }

    // Health
    float hp;
    float maxHp;
    bool takeDamage;
    // Damage cooldown
    float invincibleTimer;
    float invincibleDuration;
    float takeDamageTimer;
    float takeDamageDuration;

    // Sound: hit SFX
    Sound hitSound;
    bool hitSoundLoaded = false;

    // Basic functions
    void Init();

    void Draw(Rectangle);

    void Update(float deltaTime);

    void TakeDamage(int damage);

    void setInvincibleDuration(float duration);

    const void DrawHealthBar(int x, int y, int width, int height, int hp, int maxHp);

    // Resource cleanup
    void Unload();

    // State check
    bool IsDead() const;
};

#endif
