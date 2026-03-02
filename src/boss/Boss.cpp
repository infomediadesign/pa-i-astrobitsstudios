#include "Boss.h"
#include "raylib.h"

Boss::Boss() = default;
Boss::~Boss() = default;

void Boss::Init() {
    // Basis-Initialisierung (nichts)
}

void Boss::update(float dt) {
    if (!alive) return;
    if (hitTimer > 0.0f) hitTimer -= dt;
}

void Boss::draw() const {
    if (!alive) return;
    // Basis: Zeichne Texture falls gesetzt
    if (texture.id != 0) {
        DrawTexture(texture, (int)(position.x - texture.width / 2), (int)(position.y - texture.height / 2), WHITE);
    }
}

void Boss::Unload() {
    if (texture.id != 0) UnloadTexture(texture);
}

void Boss::takeDamage(float amount) {
    if (!alive) return;
    health -= amount;
    hitTimer = 0.1f;
    wasHit = true;
    if (health <= 0.0f) {
        alive = false;
        onDeath();
    }
}

bool Boss::isAlive() const { return alive; }

float Boss::getHealth() const { return health; }
float Boss::getMaxHealth() const { return maxHealth; }

Vector2 Boss::getPosition() const { return position; }
void Boss::setPosition(const Vector2& pos) { position = pos; }
void Boss::setPos(const Vector2& pos) { setPosition(pos); }

Rectangle Boss::GetRect() const { return {position.x - 62.5f, position.y - 25, 125, 75}; }
Rectangle Boss::GetDmgBox() const { return {position.x - 75, position.y - 75, 150, 150}; }

void Boss::startRandomAttack() { if (attackActive) return; attackActive = true; }
void Boss::stopCurrentAttack() { attackActive = false; }

void Boss::onDeath() {
    // Default: nichts
}
