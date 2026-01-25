#ifndef WALLS_H
#define WALLS_H

#include "raylib.h"

class Wall {
public:
    // Die eigentliche Kollisionsbox der Wand
    Rectangle hitbox;
    // Die Farbe, in der die Wand gezeichnet wird
    Color color;

    // Konstruktor, um eine Wand schnell zu erstellen
    Wall(float x, float y, float width, float height, Color wallColor = GRAY);

    // Methode zum Zeichnen der Wand
    void Draw() const;
};

#endif

