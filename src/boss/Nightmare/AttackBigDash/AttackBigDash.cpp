//
// Created by justu on 02/03/2026.
//
#include "AttackBigDash.h"
#include "raymath.h"
#include "raylib.h"
#include "config.h"

void AttackBigDash::Init() {
    attackArea = {0, 0, 0, 0,};
    Vector2 origin = {0, 0};
    rotation = 0;
    speed = 0;
    active = false;
    charging = false;
}

void AttackBigDash::Reset() {
    Init();
}

bool AttackBigDash::isActive() {
    return active || charging;
}

void AttackBigDash::StartBigDash(Vector2 bossPos, Vector2 playerPos) {
    charging = true;

    // Winkel zum Spieler berechnen (in Grad für Raylib)
    Vector2 dir = Vector2Subtract(playerPos, bossPos);
    rotation = atan2f(dir.y, dir.x) * RAD2DEG;

    // Rechteck definieren: x, y ist Startpunkt, width ist die Länge des Dashs
    attackArea = {bossPos.x, bossPos.y, 400.0f, 60.0f};
}

void AttackBigDash::Update(float dt, Vector2 bossPos, Vector2 playerPos) {
    if (wantsToAttack && !isActive) {
        Vector2 dir = Vector2Subtract(playerPos, bossPos);

        active = true;
        rotation = atan2f(dir.y, dir.x) * RAD2DEG;

        // Das Rechteck startet an der aktuellen Boss-Position
        // Breite (width) = Länge des Sturms, Höhe (height) = Breite des Pfads
        attackArea = {bossPos.x, bossPos.y, 500.0f, 80.0f};

        // Origin setzt den Drehpunkt auf die linke Mitte des Rechtecks
        origin = {0, attackArea.height / 2};
    }

    if (charging) {
        // Bewege den Boss entlang des Winkels
        bossPos.x += cosf(rotation * DEG2RAD) * speed * GetFrameTime();
        bossPos.y += sinf(rotation * DEG2RAD) * speed * GetFrameTime();

        // Optional: Angriff stoppen, wenn Ziel erreicht (vereinfacht)
        // if (Vector2Distance(pos, startPos) > range) isCharging = false;
    }
}

void AttackBigDash::Draw(Vector2 bossPos) const {
    if (active) {
        // Zeichne die feste "Warnzone" auf dem Boden
        DrawRectanglePro(attackArea, origin, rotation, Fade(RED, 0.3f));

        // Optional: Umrandung für mehr Deutlichkeit
        //DrawRectangleLinesEx(attackArea, 2, origin, attack.rotation, RED);
    }

    // Zeichne Boss
    DrawCircleV(bossPos, 25, MAROON);
}


/*float AttackBigDash::CheckDamage(float dt, Vector2 bossPos, Rectangle playerRect) {
    (void) dt;
    (void) bossPos;
    if (!burstActive) return 0.0f;
    // simple collision check: if rect intersects ring area
    float minD2 = 1e12f;
    float maxD2 = 0.0f;
    // approximate by testing rect corners
    Vector2 corners[4] = {
        {playerRect.x, playerRect.y}, {playerRect.x + playerRect.width, playerRect.y},
        {playerRect.x, playerRect.y + playerRect.height},
        {playerRect.x + playerRect.width, playerRect.y + playerRect.height}
    };
    for (int i = 0; i < 4; i++) {
        float dx = corners[i].x - center.x;
        float dy = corners[i].y - center.y;
        float d2 = dx * dx + dy * dy;
        if (d2 < minD2) minD2 = d2;
        if (d2 > maxD2) maxD2 = d2;
    }
    float inner2 = ringInnerR * ringInnerR;
    float outer2 = ringOuterR * ringOuterR;
    if (minD2 <= outer2 && maxD2 >= inner2) {
        burstActive = false;
        return 1.0f;
    }
    return 0.0f;
}*/