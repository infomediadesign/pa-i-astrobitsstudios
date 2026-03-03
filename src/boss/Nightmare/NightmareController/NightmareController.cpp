//
// Created by justu on 02/03/2026.
//

#include "NightmareController.h"
#include "config.h"

NightmareController::NightmareController() = default;
NightmareController::~NightmareController() = default;

void NightmareController::Init() {
    texture = LoadTexture("assets/graphics/Boss/Testimage1.png");
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
    DrawText("Tomb Golem", (int)(Game::ScreenWidth/2.2), (int)(Game::ScreenHeight - 45), 18, LIGHTGRAY);
    DrawRectangle((float) Game::ScreenWidth/2 - (barWidth / 2), (float) Game::ScreenHeight-25, barWidth, 20, DARKGRAY);
    DrawRectangle((float) Game::ScreenWidth/2 - (barWidth / 2), (float) Game::ScreenHeight-25, barWidth * (health / maxHealth), 20, GREEN);
}

void NightmareController::takeDamage(float amount) {
    Boss::takeDamage(amount);
}

void NightmareController::Unload() {
    if (texture.id != 0) UnloadTexture(texture);
}

void NightmareController::setPos(Vector2 position) {
    this->position = position;
}