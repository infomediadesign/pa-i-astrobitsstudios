//
// Created by Ben on 23.01.2026.
//

#ifndef RAYLIBSTARTER_OPTIONS_H
#define RAYLIBSTARTER_OPTIONS_H

#include "Menu.hpp"
class Options : public Menu{
public:
    Options();
    void Update() override;
    void Draw() override;
};


#endif //RAYLIBSTARTER_OPTIONS_H