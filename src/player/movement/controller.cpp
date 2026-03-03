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

    // Precompute integer frame sizes to avoid fractional source rects
    int fwInt = (texture.width > 0) ? (texture.width / 8) : 0; // integer pixels per frame
    int fhInt = (texture.height > 0) ? (texture.height / 2) : 0; // integer pixels per row

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
            // update size to idle frame using integer sizes
            size.x = 0.0f;
            size.y = facingRight ? 0.0f : (float)fhInt;
            size.width = (float)fwInt;
            size.height = (float)fhInt;
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
        (float)fwInt,
        (float)fhInt
    };

    if (!Collides(nextX, walls))
        pos.x += velocity.x;

    // --- Y ACHSE ---
    Rectangle nextY = {
        pos.x,
        pos.y + velocity.y,
        (float)fwInt,
        (float)fhInt
    };

    if (!Collides(nextY, walls))
        pos.y += velocity.y;

    // Collisionbox nach Bewegung aktualisieren (ganze Pixel)
    plcollision = {
        pos.x,
        pos.y,
        (float)fwInt,
        (float)fhInt
    };
}

void controller::Init()
{
    texture =LoadTexture("assets/graphics/Player/Ixirath_Sprites.png");
    Reset();
}
void controller::Draw()
{
    // Use integer frame size to avoid fractional source coords
    int fwInt = (texture.width > 0) ? (texture.width / 8) : 0;
    int fhInt = (texture.height > 0) ? (texture.height / 2) : 0;

    // Ensure size contains correct width/height
    size.width = (float)fwInt;
    size.height = (float)fhInt;

    Rectangle src = { size.x, size.y, size.width, size.height };
    DrawTextureRec(texture, src, pos, WHITE);
}

void controller::Animate(float dt)
{
    // Guard: avoid division by zero and invalid texture
    if (frameSpeed <= 0 || texture.width <= 0) {
        // still update facing row so Draw has correct y
        int fhInt = (texture.height > 0) ? (texture.height / 2) : 0;
        size.y = facingRight ? 0.0f : (float)fhInt;
        return;
    }

    int fwInt = (texture.width > 0) ? (texture.width / 8) : 0;
    int fhInt = (texture.height > 0) ? (texture.height / 2) : 0;

    // If not moving, stay on idle frame (0)
    if (!moving) {
        frames = 0;
        animTimer = 0.0f;
        size.x = 0.0f;
        size.y = facingRight ? 0.0f : (float)fhInt;
        size.width = (float)fwInt;
        size.height = (float)fhInt;
        return;
    }

    // When input is present, advance a frame immediately on key press to get a crisp jump
    if (IsKeyPressed(KEY_W) || IsKeyPressed(KEY_S) || IsKeyPressed(KEY_A) || IsKeyPressed(KEY_D)) {
        const int maxFramesImmediate = 8;
        frames = (frames + 1) % maxFramesImmediate;
        // update size.x using integer frame width
        size.x = (float)(frames * fwInt);
        size.width = (float)fwInt;
        size.y = facingRight ? 0.0f : (float)fhInt;
        size.height = (float)fhInt;
    }

    // Use time-based animation to avoid frame sticking when holding keys (still discrete steps)
    const float secondsPerFrame = 1.0f / (float)frameSpeed; // frameSpeed frames per second
    animTimer += dt;
    if (animTimer >= secondsPerFrame) {
        int advance = static_cast<int>(animTimer / secondsPerFrame);
        animTimer -= static_cast<float>(advance) * secondsPerFrame;
        frames += advance;
        const int maxFrames = 8; // total frames in row
        if (frames >= maxFrames) frames %= maxFrames;
        // use integer frame width to avoid fractional source x
        size.x = (float)(frames * fwInt);
        size.width = (float)fwInt;
    }

    // Always set the row according to facing direction using integer frame height
    size.y = facingRight ? 0.0f : (float)fhInt;
    size.height = (float)fhInt;
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
    int fwInt = (texture.width > 0) ? (texture.width / 8) : 0;
    int fhInt = (texture.height > 0) ? (texture.height / 2) : 0;
    size = {0.0f, 0.0f, (float)fwInt, (float)fhInt};

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