#include "schaden.h"
#include "raylib.h"
#include <algorithm> // for std::clamp
#include "config.h"
#include <cstdio>
#include "../../SFX/audio_globals.h"


void Player::Init() {
    maxHp = 100;
    hp = maxHp;

    invincibleDuration = 0.8f; // 0.8s hit cooldown
    invincibleTimer = 0.0f;
    takeDamage = false;
    takeDamageDuration = 0.2f;
    takeDamageTimer = 0.0f;

    // Load hit sound (adjust path to actual asset)
    hitSoundLoaded = false;
    if (IsFileDropped()) {
        // noop - keep signature
    }
    // Try to load a hit SFX if present
    const char* hitPath = "assets/audio/sfx/player-hit.wav";
    if (FileExists(hitPath) && IsAudioDeviceReady()) {
        hitSound = LoadSound(hitPath);
        hitSoundLoaded = true;
    }
}

void Player::Unload() {
    if (hitSoundLoaded) {
        UnloadSound(hitSound);
        hitSoundLoaded = false;
    }
}

void Player::Update(float dt) {
    // reduce invincibility timer
    if (invincibleTimer > 0.0f) {
        invincibleTimer -= dt;
        if (invincibleTimer < 0.0f){
            invincibleTimer = 0.0f;
        }
    }
    if (takeDamageTimer > 0.0f) {
        takeDamageTimer -= dt;
        if (takeDamageTimer < 0.0f){
            takeDamageTimer = 0.0f;
            takeDamage = false;
        }
    }
}

void Player::TakeDamage(int dmg) {
    // if still invincible, ignore hit
    if (invincibleTimer > 0.0f) return;
    takeDamage = true;
    hp -= dmg;
    hp = std::clamp(hp, 0.0f, maxHp);

    // start invincibility
    invincibleTimer = invincibleDuration;
    takeDamageTimer = takeDamageDuration;

    // Play hit sound if loaded - force it to be audible by temporarily overriding master volume
    if (hitSoundLoaded && IsAudioDeviceReady()) {
        // Save previous global audio state and set a short restore timer so the sound plays audibly
        g_forcePrevMasterVolume = g_gameMasterVolume;
        g_forcePrevMuted = g_gameMuted;
        // Force master volume to full and unmute (main loop will respect this and restore later)
        SetMasterVolume(1.0f);
        g_gameMuted = false;
        PlaySound(hitSound);
        // request main loop to restore previous state after short duration
        g_forceAudioTimer = 0.25f; // seconds
    }
}

bool Player::IsDead() const {
    return hp <= 0;
}

void Player::Draw(Rectangle box) {
    if (invincibleTimer > 0.0f) {
        // blink: draw only on some frames
        if (((int) (GetTime() * 10) % 2) == 0) {
            DrawRectangleRec(box, Fade(BLUE,0.6));
        }
    } else {
        // DrawRectangleRec(player.GetCollision(), BLUE);
    }

    // UI: health bar
    DrawHealthBar(20, 40, 250, 20, hp, maxHp);

}

const void Player::DrawHealthBar(int x, int y, int width, int height, int hp, int maxHp) {
    // background
    DrawRectangle(x, y, width, height, DARKGRAY);

    // hp ratio
    float ratio = (maxHp > 0) ? (float) hp / (float) maxHp : 0.0f;
    ratio = std::clamp(ratio, 0.0f, 1.0f);

    // current hp bar
    int currentWidth = (int) (width * ratio);
    DrawRectangle(x, y, currentWidth, height, RED);

    // border
    DrawRectangleLines(x, y, width, height, BLACK);
}

void Player::setInvincibleDuration(float duration) {
    invincibleDuration = duration;
}
