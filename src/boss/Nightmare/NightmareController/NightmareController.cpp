//
// Created by justu on 02/03/2026.
//

#include "NightmareController.h"
#include "config.h"

NightmareController::NightmareController() = default;
NightmareController::~NightmareController() = default;

void NightmareController::Init() {
    // Stelle sicher, dass der richtige Dateiname geladen wird (kleinschreibung auf Windows ist tolerant, aber sauberer so)
    texture = LoadTexture("assets/graphics/Boss/testimage1.png");
    Reset();
}

void NightmareController::update(float dt) {
    if (!alive) return;
    if (hitTimer > 0.0f) hitTimer -= dt;
}
void NightmareController::Reset() {
    Boss::Reset();
    position = {Game::ScreenWidth / 2.0f, Game::ScreenHeight / 2.0f};
    health=200;
    maxHealth=200;
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
    DrawRectangle((float) Game::ScreenWidth/2 - (barWidth / 2), (float) Game::ScreenHeight-25, barWidth, 20, DARKGRAY);
    DrawRectangle((float) Game::ScreenWidth/2 - (barWidth / 2), (float) Game::ScreenHeight-25, barWidth * (health / maxHealth), 20, GREEN);
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