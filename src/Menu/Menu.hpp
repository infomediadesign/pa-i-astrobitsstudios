
#ifndef RAYLIBSTARTER_MENU_HPP
#define RAYLIBSTARTER_MENU_HPP

#include "raylib.h"
#include <vector>
#include <string>

class Menu {
protected:
    int selectedItem = 0;
    std::vector<std::string> options;
    bool active = true;
    int choice = -1; // -1 = nothing selected

public:
    virtual ~Menu() = default;
    virtual void Update() = 0;
    virtual void Draw() = 0;

    int GetChoice() const { return choice; }
    void ResetChoice() { choice = -1; }

    bool IsActive() const { return active; }
    void SetActive(bool state) { active = state; }

};
#endif //RAYLIBSTARTER_MENU_HPP