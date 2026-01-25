#include "walls.h"

// Konstruktor: Wir weisen die Werte der Hitbox zu
Wall::Wall(float x, float y, float width, float height, Color wallColor) {
    hitbox = { x, y, width, height };
    color = wallColor;
}

// Zeichnet die Wand auf den Bildschirm
void Wall::Draw() const {
    DrawRectangleRec(hitbox, color);
}