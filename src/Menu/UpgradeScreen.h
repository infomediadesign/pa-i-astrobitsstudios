//
// Created by benco on 23.02.2026.
//

#ifndef RAYLIBSTARTER_UPGRADESCREEN_H
#define RAYLIBSTARTER_UPGRADESCREEN_H

#include "Menu.hpp"

class UpgradeScreen : public Menu {
public:
    UpgradeScreen();

    void Update() override; // implementiert die pure-virtual from Menu
    void Draw() override;   // implementiert die pure-virtual from Menu
    void Open();

protected:
    // Hinweis: 'choice', 'selectedItem' und 'options' werden von Menu bereitgestellt.
};


#endif //RAYLIBSTARTER_UPGRADESCREEN_H