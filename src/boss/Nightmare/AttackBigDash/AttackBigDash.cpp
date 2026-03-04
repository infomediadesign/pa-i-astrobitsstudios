//
// Created by justu on 02/03/2026.
//
#include "AttackBigDash.h"
#include "raymath.h"
#include "raylib.h"
#include "config.h"

AttackBigDash::AttackBigDash()
    : attackArea{0,0,0,0}, origin{0,0}, rotation(0.0f), charging(false), speed(0.0f), active(false), wantsToAttack(false)
{
    // Optional: Init() kann weiterhin genutzt werden, aber die Initializer-Liste reicht aus
}

void AttackBigDash::Init() {
    attackArea = {0, 0, 0, 0,};
    origin = {0, 0};
    rotation = 0;
    speed = 0;
    active = false;
    charging = false;
    wantsToAttack = false; // initialisieren
}

void AttackBigDash::Reset() {
    Init();
}

bool AttackBigDash::isActive() const {
    return active || charging;
}

void AttackBigDash::StartBigDash(Vector2 bossPos, Vector2 playerPos) {
    // charging = true;  // removed: StartBigDash darf charging nicht sofort setzen

    // Winkel zum Spieler berechnen (in Grad für Raylib)
    Vector2 dir = Vector2Subtract(playerPos, bossPos);
    rotation = atan2f(dir.y, dir.x) * RAD2DEG;

    // Rechteck definieren: x, y ist Startpunkt, width ist die Länge des Dashs
    attackArea = {bossPos.x, bossPos.y, 400.0f, 60.0f};
}

void AttackBigDash::Update(float dt, Vector2 bossPos, Vector2 playerPos) {
    if (wantsToAttack && !isActive()) {
        Vector2 dir = Vector2Subtract(playerPos, bossPos);

        active = true;
        hitApplied = false; // Rücksetzen beim Start
        rotation = atan2f(dir.y, dir.x) * RAD2DEG;

        // Das Rechteck startet an der aktuellen Boss-Position
        // Breite (width) = Länge des Sturms, Höhe (height) = Breite des Pfads
        attackArea = {bossPos.x, bossPos.y, 500.0f, 80.0f};

        // Origin setzt den Drehpunkt auf die linke Mitte des Rechtecks
        origin = {0, attackArea.height / 2};

        // Angriff ausgeführt, Anfrage zurücksetzen
        wantsToAttack = false;

        // Debug log
        TraceLog(LOG_INFO, "AttackBigDash: activated. rot=%.2f area=(%.1f,%.1f,%.1f,%.1f)", rotation, attackArea.x, attackArea.y, attackArea.width, attackArea.height);
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

// Helper: project polygon onto axis and return min/max
static void projectPolygon(const Vector2 *pts, int count, const Vector2 &axis, float &outMin, float &outMax) {
    outMin = outMax = (pts[0].x * axis.x + pts[0].y * axis.y);
    for (int i = 1; i < count; ++i) {
        float proj = (pts[i].x * axis.x + pts[i].y * axis.y);
        if (proj < outMin) outMin = proj;
        if (proj > outMax) outMax = proj;
    }
}

// Helper: check overlapping intervals
static bool overlapIntervals(float minA, float maxA, float minB, float maxB) {
    return !(maxA < minB || maxB < minA);
}

float AttackBigDash::CheckDamage(float dt, Vector2 bossPos, Rectangle playerRect) {
    (void) dt;
    if (!active) return 0.0f;

    // rotation center
    Vector2 rotCenter = { attackArea.x + origin.x, attackArea.y + origin.y };

    // Local corners of attack rectangle relative to rotation center
    Vector2 local[4] = {
        { -origin.x, -origin.y },
        { -origin.x + attackArea.width, -origin.y },
        { -origin.x + attackArea.width, -origin.y + attackArea.height },
        { -origin.x, -origin.y + attackArea.height }
    };

    // World-space corners of attack OBB
    Vector2 attackPts[4];
    float rad = rotation * DEG2RAD;
    float c = cosf(rad);
    float s = sinf(rad);
    for (int i = 0; i < 4; ++i) {
        attackPts[i].x = rotCenter.x + (c * local[i].x - s * local[i].y);
        attackPts[i].y = rotCenter.y + (s * local[i].x + c * local[i].y);
    }

    // Player AABB corners
    Vector2 playerPts[4] = {
        { playerRect.x, playerRect.y },
        { playerRect.x + playerRect.width, playerRect.y },
        { playerRect.x + playerRect.width, playerRect.y + playerRect.height },
        { playerRect.x, playerRect.y + playerRect.height }
    };

    // Axes to test: normals of both rectangle edges (two from attack OBB, two from player AABB)
    Vector2 axes[4];
    // attack edge 0->1
    Vector2 e0 = { attackPts[1].x - attackPts[0].x, attackPts[1].y - attackPts[0].y };
    Vector2 e1 = { attackPts[3].x - attackPts[0].x, attackPts[3].y - attackPts[0].y };
    // normals
    axes[0] = { -e0.y, e0.x };
    axes[1] = { -e1.y, e1.x };
    // player axes (AABB): x and y axes
    axes[2] = { 1.0f, 0.0f };
    axes[3] = { 0.0f, 1.0f };

    // Normalize axes
    for (int i = 0; i < 4; ++i) {
        float len = sqrtf(axes[i].x * axes[i].x + axes[i].y * axes[i].y);
        if (len > 0.00001f) {
            axes[i].x /= len; axes[i].y /= len;
        }
    }

    // SAT test
    for (int i = 0; i < 4; ++i) {
        float minA, maxA, minB, maxB;
        projectPolygon(attackPts, 4, axes[i], minA, maxA);
        projectPolygon(playerPts, 4, axes[i], minB, maxB);
        if (!overlapIntervals(minA, maxA, minB, maxB)) {
            return 0.0f; // separating axis found -> no collision
        }
    }

    // collision detected
    if (!hitApplied) {
        hitApplied = true;
        active = false;
        charging = false;
        TraceLog(LOG_INFO, "AttackBigDash: hit detected at rotCenter=(%.1f,%.1f) player=(%.1f,%.1f,%.1f,%.1f)", rotCenter.x, rotCenter.y,
                 playerRect.x, playerRect.y, playerRect.width, playerRect.height);
        return damage;
    }
    return 0.0f;
}

// Implementierung des Setters
void AttackBigDash::SetWantsToAttack(bool val) {
    wantsToAttack = val;
    TraceLog(LOG_INFO, "AttackBigDash: wantsToAttack set to %d", (int)val);
}
