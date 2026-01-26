//
// Created by justu on 26/01/2026.
//

#include "AttackJump.h"

void AttackJump::setRange(float range) {
    this->attackRange = range;
}

int AttackJump::getDamage() {
    return this->damage;
}

float AttackJump::getAttackRange() {
    return this->attackRange;
}

Vector2 AttackJump::getPos() {
    return this->pos;
}

void AttackJump::setPos(Vector2 pos) {
    this->pos = pos;
}

bool AttackJump::hitPlayer(Rectangle playerRect) {
    if (CheckCollisionCircleRec(this->pos, this->getAttackRange(),  playerRect))
        return true;
    else
    {
        return false;
    }
}

void AttackJump::attack(Vector2 playerPos, Rectangle playerRect, float dt) {
    startAttack(playerPos);
    jumpAttackCD(2.0f, dt);
    doAttack(playerRect, playerPos);
    //stopAttack();
}

void AttackJump::startAttack(Vector2 playerPos) {
    setRange(100.0f);
    DrawCircleLines(playerPos.x +30,playerPos.y+35, this->getAttackRange(),RED);
    setPos(playerPos);
}

void AttackJump::doAttack(Rectangle playerRect, Vector2 playerPos) {
    DrawCircle(playerPos.x +30,playerPos.y+35, this->getAttackRange(),RED);

    if (CheckCollisionCircleRec(this->pos, this->getAttackRange(),  playerRect)) {
        //TakeDamage(30);
    }
}

void AttackJump::stopAttack() {
    //unload texture;
}

void AttackJump::jumpAttackCD(float duration, float dt) {

    if (timer >0) timer -dt;
    DrawText(TextFormat("Cooldown %.2f", timer), 500, 600, 24,BLUE);
    if (timer == 0) timer == duration;

}

