#include "GolemController.h"
#include "config.h"
#include <cstdio>

GolemController::GolemController() = default;
GolemController::~GolemController() = default;

void GolemController::Init() {
    texture = LoadTexture("assets/graphics/Boss/Boss1_Base_Sprite.png");
    Reset();
}

void GolemController::update(float dt) {
    if (!alive) return;
    if (hitTimer > 0.0f) hitTimer -= dt;
}
void GolemController::Reset() {
    Boss::Reset();
    position = {Game::ScreenWidth / 2.0f, Game::ScreenHeight / 2.0f};
    health=200;
    maxHealth=200;
    hitTimer = 0.0f;
    wasHit = false;
    alive = true;
}
void GolemController::draw() const {
    if (!alive) return;

    Color tint = (hitTimer > 0.0f) ? RED : WHITE;
    if (texture.id != 0) DrawTexture(texture, (int)(position.x - texture.width / 2), (int)(position.y - texture.height / 2), tint);

    float barWidth = 400.0f;
    DrawText("Tomb Golem", (int)(Game::ScreenWidth/2.2), (int)(Game::ScreenHeight - 45), 18, LIGHTGRAY);
    float barX = (float) Game::ScreenWidth/2 - (barWidth / 2);
    float barY = (float) Game::ScreenHeight-25;
    DrawRectangle(barX, barY, barWidth, 20, DARKGRAY);
    float ratio = (maxHealth > 0.0f) ? (health / maxHealth) : 0.0f;
    if (ratio < 0.0f) ratio = 0.0f;
    if (ratio > 1.0f) ratio = 1.0f;
    float filledWidth = barWidth * ratio;
    DrawRectangle(barX, barY, filledWidth, 20, GREEN);

    // Draw numeric HP / MaxHP to the right of the bar
    char buf[64];
    snprintf(buf, sizeof(buf), "%d / %d", (int)health, (int)maxHealth);
    DrawText(buf, (int)(barX + barWidth + 8), (int)(barY - 2), 16, WHITE);
}

void GolemController::takeDamage(float amount) {
    Boss::takeDamage(amount);
}

void GolemController::Unload() {
    if (texture.id != 0) UnloadTexture(texture);
}

void GolemController::setPos(Vector2 position) {
    this->position = position;
}
