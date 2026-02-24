#pragma once

#include "raylib.h"
#include <memory>

// Basisklasse für Bosse. Andere Boss-Typen (z.B. Golem) sollen von dieser Klasse erben.
class Boss {
public:
    Boss();
    virtual ~Boss();

    // Lebenszyklus
    virtual void Init();
    virtual void update(float dt);   // Spiel-Logik pro Frame
    virtual void draw() const;       // Zeichnen
    virtual void Unload();
    virtual void reset();
    // Backwards-compatible wrappers (some legacy code uses Update/Draw)
    virtual void Update(float dt) { update(dt); }
    virtual void Draw() { draw(); }

    // Schaden / Leben
    virtual void takeDamage(float amount);
    bool isAlive() const;

    // Getter für Kompatibilität
    float getHealth() const;
    float getMaxHealth() const;

    // Position
    Vector2 getPosition() const;
    void setPosition(const Vector2& pos);
    void setPos(const Vector2& pos); // Kompatibel mit älteren Namen

    // Hitboxes (müssen von abgeleiteten Klassen implentiert werden)
    virtual Rectangle GetRect() const;
    virtual Rectangle GetDmgBox() const;

    // Angriffskontrolle
    // Startet eine zufällige Attacke (implementierungsspezifisch in abgeleiteten Klassen)
    virtual void startRandomAttack();
    virtual void stopCurrentAttack();

protected:
    Vector2 position{0,0};
    Texture2D texture{};            // optionales Sprite
    float health{100.0f};
    float maxHealth{100.0f};
    bool alive{true};

    // Anzeige / Treffer
    float hitTimer{0.0f};
    bool wasHit{false};

    // Nur eine Attacke gleichzeitig
    bool attackActive{false};

    // Hilfsfunktionen
    virtual void onDeath();
};
