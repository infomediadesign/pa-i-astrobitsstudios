//
// Created by justu on 26/01/2026.
//

#ifndef PA_I_ASTROBITSSTUDIOS_ATTACKJUMP_H
#define PA_I_ASTROBITSSTUDIOS_ATTACKJUMP_H
#include "raylib.h"
#include "../../../player/schaden/schaden.h"
#include "../BossController/boss.h"

class AttackJump {
public:
    Texture2D texture = LoadTexture("assets/graphics/BossJumpAttackRange");
    Vector2 pos;

    Rectangle dstH;
    Rectangle srcH;


    void startAttack(Vector2 playerPos);

    void attack(Vector2, Rectangle, float dt,Player &schadensSystem, Enemy &golem);

    void doAttack(Rectangle playerRect, Vector2 playerPos, Player &schadensSystem, Enemy &);

    void stopAttack();

    void setRange(float range);

    int getDamage();

    float getAttackRange();

    bool hitPlayer(Rectangle playerRect);

    Vector2 getPos();

    void setPos(Vector2 pos);

    void jumpAttackCD(float duration,float dt);

    void setActive(bool active);

    void startAttackDraw(Vector2 playerPos);

    void doAttackDraw(Vector2);

    bool isActive();

    bool isDoAttackActive();

    void setDoAttackActive(bool active);

    bool isStartAttackActive();

    void setStartAttackActive(bool active);

    void upadteAttackCD(float dt);

    void DrawCD();

private:
    int frames;
    int frameSpeed;
    int frameCount;

    float lifespan;
    float attackRange;
    float timer;
    float duration;

    int damage;
    bool doAttackActive;
    bool startAttackActive;
    bool active;
    bool hasHitPlayer;
};


#endif //PA_I_ASTROBITSSTUDIOS_ATTACKJUMP_H
