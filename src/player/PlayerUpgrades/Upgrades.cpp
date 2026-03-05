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
    schadensSystem.maxHp = (int) round(schadensSystem.maxHp * 1.45);
    schadensSystem.hp = schadensSystem.maxHp;
    plattack.damage = (int) round(plattack.damage * 0.9);
}
void Upgrades:: Upgrade2(Player &schadensSystem, controller &movement){
    schadensSystem.maxHp = (int) round(schadensSystem.maxHp * 0.8);
    schadensSystem.hp = schadensSystem.maxHp;
    movement.speed = (int) round(movement.speed * 1.3);
    movement.dashDistance = (int) round(movement.dashDistance * 1.3);
}
void Upgrades:: Upgrade3(plattack &plattack, controller &movement) {
    plattack.damage = (int) round(plattack.damage * 1.5);
    movement.speed = (int) round(movement.speed * 0.8);
    movement.dashDistance = (int) round(movement.dashDistance * 0.8);

}