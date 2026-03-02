//
// Created by benco on 25.02.2026.
//

#ifndef RAYLIBSTARTER_UPGRADES_H
#define RAYLIBSTARTER_UPGRADES_H
class Player;
class plattack;
struct controller;
class Upgrades {
public:
    void Upgrade1(Player &schadensSystem, plattack &plattack);
    void Upgrade2(Player &schadensSystem, controller &movement);
    void Upgrade3(plattack &plattack, controller &movement);
private:
    double prozentWert = 10;

};


#endif //RAYLIBSTARTER_UPGRADES_H