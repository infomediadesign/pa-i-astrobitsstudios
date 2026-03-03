//
// Created by Ben on 13.01.2026.
#include "raylib.h"
//
#include <vector>
#include "controller.h"
#include <cmath>
#include "../../enviroment/walls.h"

Rectangle controller::GetHitbox() const
{
    return Rectangle{
        pos.x,
        pos.y,
        size.width,
        size.height
    };
}
void controller::Update(float dt, const std::vector<Wall>& walls)
{
    velocity = {0, 0};
    bool input = false;
    bool leftInput = false;
    bool rightInput = false;
    if (IsKeyDown(KEY_W)) { velocity.y -= 1; input = true; }
    if (IsKeyDown(KEY_S)) { velocity.y += 1; input = true; }
    if (IsKeyDown(KEY_A)) { velocity.x -= 1; input = true; leftInput = true; }
    if (IsKeyDown(KEY_D)) { velocity.x += 1; input = true; rightInput = true; }
    float normal = std::sqrt(velocity.x*velocity.x + velocity.y*velocity.y);
    if (normal > 0) {
        velocity.x /= normal;
        velocity.y /= normal;
        setMoving(true);
    } else setMoving(false);
    gehx = velocity.x;
    gehy = velocity.y;

    // Update facing direction based on horizontal input
    if (rightInput) facingRight = true;
    else if (leftInput) facingRight = false;

    // Wenn nur die Animation ohne physische Bewegung gewünscht ist,
    // dann erhöhen wir nur die Animation und nicht die Position.
    if (animateOnlyMovement) {
        // simulate dt-based animation when there is input
        if (input) {
            // advance animation frames while input exists (discrete steps)
            Animate(dt);
        } else {
            // When input stops, reset to first frame (idle)
            frames = 0;
            animTimer = 0.0f;
            // update size to idle frame
            float fw = (texture.width > 0) ? (float)texture.width / 9.0f : 0.0f;
            float fh = (texture.height > 0) ? (float)texture.height / 2.0f : 0.0f;
            size.x = 0.0f;
            size.y = facingRight ? 0.0f : fh;
            size.width = fw;
            size.height = fh;
        }
        // plcollision bleibt unverändert oder kann an pos gebunden bleiben
        plcollision.x = pos.x;
        plcollision.y = pos.y;
        return;
    }

    velocity.x = velocity.x * speed * dt;
    velocity.y = velocity.y * speed * dt;
    // --- X ACHSE ---
    Rectangle nextX = {
        pos.x + velocity.x,
        pos.y,
        (float)texture.width / 9,
        (float)texture.height / 2
    };

    if (!Collides(nextX, walls))
        pos.x += velocity.x;

    // --- Y ACHSE ---
    Rectangle nextY = {
        pos.x,
        pos.y + velocity.y,
        (float)texture.width / 9,
        (float)texture.height / 2
    };

    if (!Collides(nextY, walls))
        pos.y += velocity.y;

    // Collisionbox nach Bewegung aktualisieren
    plcollision = {
        pos.x,
        pos.y,
        (float)texture.width / 9,
        (float)texture.height / 2
    };
}

void controller::Init()
{
    texture =LoadTexture("assets/graphics/Player/Ixirath_Sprites.png");
    Reset();
}
void controller::Draw()
{
    // Ensure size rect uses correct frame width and proper source rectangle
    float frameWidth = (texture.width > 0) ? (float)texture.width / 9.0f : 0.0f;
    float frameHeight = (texture.height > 0) ? (float)texture.height / 2.0f : 0.0f;

    // Ensure size contains correct width/height
    size.width = frameWidth;
    size.height = frameHeight;

    Rectangle src = { size.x, size.y, size.width, size.height };
    DrawTextureRec(texture, src, pos, WHITE);
}

void controller::Animate(float dt)
{
    // Guard: avoid division by zero and invalid texture
    if (frameSpeed <= 0 || texture.width <= 0) {
        // still update facing row so Draw has correct y
        float fh = (texture.height > 0) ? (float)texture.height / 2.0f : 0.0f;
        size.y = facingRight ? 0.0f : fh;
        return;
    }

    float frameWidth = (float)texture.width / 9.0f;
    float frameHeight = (float)texture.height / 2.0f;

    // If not moving, stay on idle frame (0)
    if (!moving) {
        frames = 0;
        animTimer = 0.0f;
        size.x = 0.0f;
        size.y = facingRight ? 0.0f : frameHeight;
        size.width = frameWidth;
        size.height = frameHeight;
        return;
    }

    // Use time-based animation to avoid frame sticking
    const float secondsPerFrame = 1.0f / (float)frameSpeed; // frameSpeed frames per second
    animTimer += dt;
    if (animTimer >= secondsPerFrame) {
        // Advance frames as many as needed (in case of lag)
        int advance = static_cast<int>(animTimer / secondsPerFrame);
        animTimer -= static_cast<float>(advance) * secondsPerFrame; // avoid narrowing warning
        frames += advance;
        const int maxFrames = 9; // total frames in row
        if (frames >= maxFrames) frames %= maxFrames;
        // update source x (discrete jump to frame index)
        size.x = (float)frames * frameWidth;
    }

    // Always set the row according to facing direction
    size.y = facingRight ? 0.0f : frameHeight;
    size.width = frameWidth;
    size.height = frameHeight;
}

void controller::Unload()
{
    UnloadTexture(texture);
}
void controller::Dash(const std::vector<Wall>& walls,float dt)
{
    if (!IsKeyPressed(KEY_LEFT_SHIFT))
        return;

    Rectangle testBox = plcollision;
    Vector2 dashDir = {0, 0};

    if (IsKeyDown(KEY_D)) dashDir.x += 1;
    if (IsKeyDown(KEY_A)) dashDir.x -= 1;
    if (IsKeyDown(KEY_W)) dashDir.y -= 1;
    if (IsKeyDown(KEY_S)) dashDir.y += 1;
    float norm = std::sqrt(dashDir.x*dashDir.x + dashDir.y*dashDir.y);
    if (norm>0) {
        dashDir.x /= norm;
        dashDir.y /= norm;
    }
    float dashDistance = 125;
    float step = 5;

    int stepsCount = static_cast<int>(dashDistance / step);
    for (int si = 0; si < stepsCount; ++si)
    {
        testBox.x += dashDir.x * step;
        testBox.y += dashDir.y * step;

        bool hit = false;
        for (const Wall& w : walls)
        {
            if (CheckCollisionRecs(testBox, w.hitbox))
            {
                hit = true;
                break;
            }
        }

        if (hit)
            break;

        pos.x += dashDir.x * step;
        pos.y += dashDir.y * step;
    }
    plcollision.x = pos.x;
    plcollision.y = pos.y;
}

bool controller::Collides(const Rectangle& box, const std::vector<Wall>& walls)
{
    for (const Wall& w : walls)
    {
        if (CheckCollisionRecs(box, w.hitbox))
            return true;
    }
    return false;
}
void controller::Reset() {
    pos = {10, 100};
    speed = 250; //minimum 200 aber das wäre sehr langsam. langsamer würde ich auf keinen fall empfehlen
    frames = 0;
    frameCount = 0;
    frameSpeed = 8;
    animTimer = 0.0f; // reset timer
    animateOnlyMovement = false; // default: physical movement enabled
    facingRight = true; // default facing right
    float fw = 0.0f;
    float fh = 0.0f;
    if (texture.width > 0) fw = (float)texture.width / 9.0f;
    if (texture.height > 0) fh = (float)texture.height / 2.0f;
    size = {0.0f, 0.0f, fw, fh};

}

Rectangle controller::GetCollision() {
    return plcollision;
}
Vector2 controller::GetPos() const {
    return pos;
}

Rectangle controller::GetSize() const {
    return size;
}

//test