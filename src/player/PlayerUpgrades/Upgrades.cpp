//
// Created by benco on 25.02.2026.
//

#include "Upgrades.h"
#include "raylib.h"
#include <cmath>
#include "player/combat/plattack.h"
#include "player/schaden/schaden.h"
#include "player/movement/controller.h"

void Upgrades:: Upgrade1(Player &schadensSystem, plattack &plattack) {
    schadensSystem.maxHp = (int) round(schadensSystem.maxHp * (1 + prozentWert / 100));
    schadensSystem.hp = schadensSystem.maxHp;
    plattack.damage = (int) round(plattack.damage * (1 - prozentWert / 100));
}
void Upgrades:: Upgrade2(Player &schadensSystem, controller &movement){
    schadensSystem.maxHp = (int) round(schadensSystem.maxHp * (1 - prozentWert / 100));
    schadensSystem.hp = schadensSystem.maxHp;
    movement.speed = (int) round(movement.speed * (1 + prozentWert / 100));
}
void Upgrades:: Upgrade3() {

}