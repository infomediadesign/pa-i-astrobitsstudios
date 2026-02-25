//
// Created by benco on 25.02.2026.
//

#ifndef RAYLIBSTARTER_UPGRADES_H
#define RAYLIBSTARTER_UPGRADES_H
#include "player/schaden/schaden.h"
#include "player/combat/plattack.h"
class Upgrades {
public:
    void Upgrade1(Player &schadensSystem, plattack &plattack);
    void Upgrade2();
    void Upgrade3();
private:
    double prozentWert = 10;

};


#endif //RAYLIBSTARTER_UPGRADES_H