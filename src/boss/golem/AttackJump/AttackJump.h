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
    //Texture2D texture = LoadTexture("assets/graphics/BossJumpAttackRange");
    Vector2 pos;

    Rectangle dstH;
    Rectangle srcH;


    void startAttack(Vector2 playerPos);

    void attack(Vector2, Rectangle, float dt, Player &schadensSystem, Enemy &golem);

    void stopAttack();

    void setRange(float range);

    int getDamage();

    float getAttackRange();

    bool hitPlayer(Rectangle playerRect);

    Vector2 getPos();

    void setPos(Vector2 pos);

    void jumpAttackCD(float duration, float dt);

    void setActive(bool active);

    void startAttackDraw(Vector2 playerPos);

    void doAttack(Rectangle playerRect, Player &player, Enemy &boss);

    void doAttackDraw(Vector2);

    bool isActive();

    bool isDoAttackActive();

    void setDoAttackActive(bool active);

    bool isStartAttackActive();

    void setStartAttackActive(bool active);

    void updateAttackCD(float dt);

    void DrawCD();

private:
    int frames = 0;
    int frameSpeed = 0;
    int frameCount = 0;

    float lifespan = 0.0f;
    float attackRange = 0.0f;
    float timer = 0.0f;
    float duration = 0.0f;

    int damage = 0;
    bool doAttackActive = false;
    bool startAttackActive = false;
    bool active = false;
    bool hasHitPlayer = false;
};


#endif //PA_I_ASTROBITSSTUDIOS_ATTACKJUMP_H
