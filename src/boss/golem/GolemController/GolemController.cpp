#include "GolemController.h"
#include "config.h"

GolemController::GolemController() = default;
GolemController::~GolemController() = default;

void GolemController::Init() {
    texture = LoadTexture("assets/graphics/Boss/Boss1_Base_Sprite.png");
    position = {Game::ScreenWidth / 2.0f, Game::ScreenHeight / 2.0f};

    health = 200.0f;
    maxHealth = 200.0f;
    hitTimer = 0.0f;
    alive = true;
    wasHit = false;
}

void GolemController::update(float dt) {
    if (!alive) return;
    if (hitTimer > 0.0f) hitTimer -= dt;
}

void GolemController::draw() const {
    if (!alive) return;

    Color tint = (hitTimer > 0.0f) ? RED : WHITE;
    if (texture.id != 0) DrawTexture(texture, (int)(position.x - texture.width / 2), (int)(position.y - texture.height / 2), tint);

    float barWidth = 400.0f;
    DrawText("Tomb Golem", (int)(Game::ScreenWidth/2.2), (int)(Game::ScreenHeight - 45), 18, LIGHTGRAY);
    DrawRectangle((float) Game::ScreenWidth/2 - (barWidth / 2), (float) Game::ScreenHeight-25, barWidth, 20, DARKGRAY);
    DrawRectangle((float) Game::ScreenWidth/2 - (barWidth / 2), (float) Game::ScreenHeight-25, barWidth * (health / maxHealth), 20, GREEN);
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
