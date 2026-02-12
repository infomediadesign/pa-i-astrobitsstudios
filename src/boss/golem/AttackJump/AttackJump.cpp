//
// Created by justu on 26/01/2026.
//
#include "raylib.h"
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

void AttackJump::setPos(Vector2 playerPos) {
    // Wir speichern EXAKT die Mitte des Spielers (ohne +30 Schätzung)
    this->pos = playerPos;
}

bool AttackJump::hitPlayer(Rectangle playerRect) {
    if (CheckCollisionCircleRec(this->pos, this->getAttackRange(), playerRect))
        return true;
    else {
        return false;
    }
}

void AttackJump::attack(Vector2 playerPos, Rectangle playerRect, float dt, Player &schadensSystem, Enemy &golem) {
    // 1. Phase: Start (Warnkreis erscheint)
    if (startAttackCD.Ready() && !this->isActive()) {
        startAttack(playerPos);
        startAttackCD.Trigger();
    }

    if (this->isActive()) {
        // 2. Phase: Einschlag (Boss springt)
        if (doAttackCD.Ready() && !isDoAttackActive()) {
            setDoAttackActive(true);
            setStartAttackActive(false); // Warnkreis aus, wenn Boss landet
            doAttack(playerRect, schadensSystem, golem);
        }

        // 3. Phase: Ende (Boss macht sich bereit für nächste Aktion)
        if (stopAttackCD.Ready()) {
            stopAttack();
        }
    }
}

void AttackJump::startAttack(Vector2 playerPos) {
    this->setActive(true);             // <--- Sicherstellen, dass das hier steht!
    this->setStartAttackActive(true);
    setPos(playerPos);
    doAttackCD.Trigger();
    stopAttackCD.Trigger();
}

void AttackJump::startAttackDraw(Vector2 playerPos) {
    if (startAttackActive) {
        // Falls du den Kreis am Ziel zeichnest (da wo er landen wird):
        DrawCircleLines((int)this->pos.x, (int)this->pos.y, this->getAttackRange(), RED);
        DrawCircle((int)this->pos.x, (int)this->pos.y, this->getAttackRange(), Fade(RED, 0.4f));
    }
}

void AttackJump::doAttack(Rectangle playerRect, Player &player, Enemy &boss) {
    Rectangle bossRect = boss.GetRect();

    // Wir zentrieren den Boss auf den Punkt, wo der Spieler stand
    // Wir ziehen die HALBE Breite des Bosses ab
    boss.pos.x = this->pos.x - (0.0f);
    boss.pos.y = this->pos.y - (0.0f);

    if (hitPlayer(playerRect)) {
        player.TakeDamage(20);
    }
}

void AttackJump::doAttackDraw(Vector2 playerPos) {
    if (doAttackActive) {
        // Der eigentliche Einschlag (Solider Kreis)
        DrawCircle((int)this->pos.x, (int)this->pos.y, this->getAttackRange(), RED);
    }
}

void AttackJump::stopAttack() {
    this->setDoAttackActive(false);
    this->setStartAttackActive(false);
    this->setActive(false); // <--- DAS hier verhindert den Dauer-Sprung!
}

void AttackJump::updateAttackCD(float dt) {
    // Die drei Cooldown-Objekte brauchen das dt, um abzulaufen
    startAttackCD.Update(dt);
    doAttackCD.Update(dt);
    stopAttackCD.Update(dt);
}

void AttackJump::setActive(bool val) {
    this->active = val;
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

void AttackJump::setDoAttackActive(bool val) {
    this->doAttackActive = val;
}

void AttackJump::setStartAttackActive(bool val) {
    this->startAttackActive = val;
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

