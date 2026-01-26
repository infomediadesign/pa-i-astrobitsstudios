//
// Created by justu on 26/01/2026.
//

#ifndef PA_I_ASTROBITSSTUDIOS_ATTACKJUMP_H
#define PA_I_ASTROBITSSTUDIOS_ATTACKJUMP_H
#include "raylib.h"


class AttackJump {
public:
    Texture2D texture = LoadTexture("assets/graphics/BossJumpAttackRange");
    Vector2 pos;

    Rectangle dstH;
    Rectangle srcH;


    void startAttack(Vector2 playerPos);

    void attack(Vector2, Rectangle, float dt);

    void doAttack(Rectangle,Vector2);

    void stopAttack();

    void setRange(float range);

    int getDamage();

    float getAttackRange();

    bool hitPlayer(Rectangle playerRect);

    Vector2 getPos();

    void setPos(Vector2 pos);

    void jumpAttackCD(float duration,float dt);

private:
    int frames;
    int frameSpeed;
    int frameCount;

    float lifespan;
    float attackRange;
    float timer;
    float duration;

    int damage;
    bool active;
    bool hasHitPlayer;
};


#endif //PA_I_ASTROBITSSTUDIOS_ATTACKJUMP_H
