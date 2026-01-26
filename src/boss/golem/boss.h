//
// Created by Ben on 04.12.2025.
//

#pragma once
#include "raylib.h"

struct Enemy {
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


    // Funktion, um Schaden zu verursachen
    void TakeDamage(float amount);

    // Hilfsfunktion für die Hitbox (für Kollisionen mit Schüssen)
    Rectangle GetRect();
};
