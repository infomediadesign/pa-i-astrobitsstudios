//
// Created by justu on 26/01/2026.
//

#include "AttackJump.h"
#include "../../../cooldown.h"
#include "../../../player/schaden/schaden.h"
#include "../BossController/boss.h"



Cooldown startAttackCD(1.0f);
Cooldown doAttackCD(1.0f);
Cooldown stopAttackCD(2.0f);

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
    if (CheckCollisionCircleRec(this->pos, this->getAttackRange(), playerRect))
        return true;
    else {
        return false;
    }
}

void AttackJump::attack(Vector2 playerPos, Rectangle playerRect, float dt, Player &schadensSystem, Enemy &golem) {
    if (startAttackCD.Ready() && !this->isStartAttackActive()) {
        startAttack(playerPos);
        startAttackCD.Trigger(); // Verhindert sofortigen Neustart
    }

    if (this->isStartAttackActive()) {
        if (doAttackCD.Ready())
            if (!isDoAttackActive()) {
                setDoAttackActive(true);
                doAttack(playerRect, playerPos, schadensSystem, golem);
            }

        if (stopAttackCD.Ready())
            stopAttack();
    }
}

void AttackJump::startAttack(Vector2 playerPos) {
    setRange(100.0f);
    setPos(playerPos);
    doAttackCD.Trigger();
    stopAttackCD.Trigger();
    this->setStartAttackActive(true);
}

void AttackJump::startAttackDraw(Vector2 playerPos) {
    if (startAttackActive)
        DrawCircleLines(playerPos.x + 30, playerPos.y + 35, this->getAttackRange(),RED);
}

void AttackJump::upadteAttackCD(float dt) {
    startAttackCD.Update(dt);
    doAttackCD.Update(dt);
    stopAttackCD.Update(dt);
}

// Füge einen Parameter vom Typ deiner Schadens-Klasse hinzu (z.B. Player)
void AttackJump::doAttack(Rectangle playerRect, Vector2 playerPos, Player &schadensSystem, Enemy &golem) {

    golem.setPos(getPos());

    if (CheckCollisionCircleRec(this->pos, this->getAttackRange(), playerRect)) {
        schadensSystem.TakeDamage(20); // Jetzt klappt der Zugriff!
    }

}

void AttackJump::doAttackDraw(Vector2 playerPos) {
    if (doAttackActive && doAttackCD.Ready()) {

        DrawCircle(playerPos.x + 30, playerPos.y + 35, this->getAttackRange(),RED);
    }
    if (stopAttackCD.Ready())
        stopAttack();
}

void AttackJump::stopAttack() {
    this->setDoAttackActive(false);
    this->setStartAttackActive(false);
}

void AttackJump::jumpAttackCD(float duration, float dt) {
    timer = duration;
    if (timer > 0) timer - dt;
    DrawText(TextFormat("Cooldown %.2f", timer), 500, 600, 24,BLUE);
    if (timer == 0) timer == duration;
}

void AttackJump::setActive(bool active) {
    this->active = active;
}

bool AttackJump::isActive() {
    return active;
}

bool AttackJump::isDoAttackActive() {
    return doAttackActive;
}

bool AttackJump::isStartAttackActive() {
    return startAttackActive;
}

void AttackJump::setDoAttackActive(bool active) {
    this->doAttackActive = active;
}

void AttackJump::setStartAttackActive(bool active) {
    this->startAttackActive = active;
}

void AttackJump::DrawCD() {
    if (doAttackCD.Ready())
        DrawText("Ready", 300, 20, 10, GREEN);
    else DrawText(TextFormat("do: Cooldown %.2f", doAttackCD.Remaining()), 300, 20, 10, GREEN);
    if (startAttackCD.Ready())
        DrawText("Ready", 450, 20, 10, GREEN);
    else DrawText(TextFormat("start; Cooldown %.2f", startAttackCD.Remaining()), 450, 20, 10, GREEN);
    if (stopAttackCD.Ready())
        DrawText("Ready", 500, 20, 10, GREEN);
    else DrawText(TextFormat("stop; Cooldown %.2f", stopAttackCD.Remaining()), 500, 20, 10, GREEN);
}

