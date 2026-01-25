//
// Created by benco on 25.01.2026.
//

#ifndef RAYLIBSTARTER_DEATH_SCREEN_H
#define RAYLIBSTARTER_DEATH_SCREEN_H
#include "Menu.hpp"


class Death_Screen : public Menu {
public:
    Death_Screen();
    void Update() override ;
    void Draw() override;


};


#endif //RAYLIBSTARTER_DEATH_SCREEN_H