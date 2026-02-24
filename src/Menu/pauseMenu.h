#ifndef RAYLIBSTARTER_PAUSENMENU_H
#define RAYLIBSTARTER_PAUSENMENU_H
#pragma once

#include "Menu.hpp"

class pauseMenu : public Menu {
public:
    pauseMenu();

    void Update() override;
    void Draw() override;
    void Open();

private:
};

#endif // RAYLIBSTARTER_PAUSENMENU_H
