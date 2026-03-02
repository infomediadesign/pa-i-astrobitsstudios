//
// Created by justu on 2/24/2026.
//

#ifndef RAYLIBSTARTER_DIFFICULTY_H
#define RAYLIBSTARTER_DIFFICULTY_H
#include "../player/schaden/schaden.h"
#include "../boss/golem/GolemController/GolemController.h"


class difficulty {
    public:
    void setDifficultyEasy(Player &player, Boss &boss);
    void setDifficultyNormal(Player &player, Boss &boss);
    void setDifficultyHard(Player &player, Boss &boss);

};


#endif //RAYLIBSTARTER_DIFFICULTY_H