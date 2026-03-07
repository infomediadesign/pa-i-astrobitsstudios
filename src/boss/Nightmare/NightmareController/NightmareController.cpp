//
// Created by justu on 02/03/2026.
//

#include "NightmareController.h"
#include "config.h"
#include <cstdio>

NightmareController::NightmareController() = default;
NightmareController::~NightmareController() = default;

void NightmareController::Init() {
    // Stelle sicher, dass der richtige Dateiname geladen wird (kleinschreibung auf Windows ist tolerant, aber sauberer so)
    texture = LoadTexture("assets/graphics/Boss/Boss2_Base_Sprite.png");
    Reset();
}

void NightmareController::update(float dt) {
    if (!alive) return;
    if (hitTimer > 0.0f) hitTimer -= dt;
}
void NightmareController::Reset() {
    Boss::Reset();
    position = {Game::ScreenWidth / 2.0f, Game::ScreenHeight / 2.0f};
    health=100;
    maxHealth=100;
    hitTimer = 0.0f;
    wasHit = false;
    alive = true;
}
void NightmareController::draw() const {
    if (!alive) return;

    Color tint = (hitTimer > 0.0f) ? RED : WHITE;
    if (texture.id != 0) DrawTexture(texture, (int)(position.x - texture.width / 2), (int)(position.y - texture.height / 2), tint);

    //Healthbar
    float barWidth = 400.0f;
    DrawText("Nightmare", (int)(Game::ScreenWidth/2.2), (int)(Game::ScreenHeight - 45), 18, LIGHTGRAY);
    float barX = (float) Game::ScreenWidth/2 - (barWidth / 2);
    float barY = (float) Game::ScreenHeight-25;
    DrawRectangle(barX, barY, barWidth, 20, DARKGRAY);

    // Compute clamped ratio so the filled part never exceeds the bar
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

void NightmareController::takeDamage(float amount) {
    TraceLog(LOG_INFO, "NightmareController::takeDamage called: amount=%.2f, health before=%.2f", amount, health);
    Boss::takeDamage(amount);
    TraceLog(LOG_INFO, "NightmareController::takeDamage applied: health after=%.2f", health);
}

void NightmareController::Unload() {
    if (texture.id != 0) UnloadTexture(texture);
}

void NightmareController::setPos(Vector2 position) {
    this->position = position;
}