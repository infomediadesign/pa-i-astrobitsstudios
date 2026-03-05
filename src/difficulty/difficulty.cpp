//
// Created by justu on 2/24/2026.
//

#include "difficulty.h"


difficulty::difficulty() {
    // Constructor can be empty or used for initialization if needed
}
void difficulty::setDifficultyEasy(Player &player, Boss &boss) {
    player.maxHp = 150;
    player.hp = player.maxHp;
    boss.setMaxHealth(150);
    player.setInvincibleDuration(1.0f);
}

void difficulty::setDifficultyNormal(Player &player, Boss &boss) {
    player.maxHp = 100;
    player.hp = player.maxHp;
    boss.setMaxHealth(200);
    player.setInvincibleDuration(0.8f);
}

void difficulty::setDifficultyHard(Player &player, Boss &boss) {
    player.maxHp = 70;
    player.hp = player.maxHp;
    boss.setMaxHealth(250);
    player.setInvincibleDuration(0.5f);
}

void difficulty::Draw() {
    // Optional: Draw current difficulty on screen for debugging
    // This could be removed or moved to a more appropriate place in the UI
    DrawText("Difficulty: ", 10, 10, 20, BLACK);
}