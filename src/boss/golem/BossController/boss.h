//
// Created by Ben on 04.12.2025.
//

#pragma once
#include "raylib.h"

class Enemy {
public:
    Texture2D texture;
    Vector2 pos;

    // Neue Variablen für Schaden und Lebensbalken
    float health;
    float maxHealth;
    float hitTimer; // Steuert das rote Aufblinken
    bool active; // Ist der Boss noch am Leben?
    bool wasHit;

    void Init();

    void Update(float dt);

    void Draw();

    void Unload();

    //set Position vom Boss
    void setPos(Vector2 position);

    // Funktion, um Schaden zu verursachen
    void TakeDamage(float amount);

    // Hitbox, damit Spieler Schaden erleiden bei Kollision
    Rectangle GetRect();

    // Hitbox, damit der Spieler schaden machen kann
    Rectangle GetDmgBox();
};
