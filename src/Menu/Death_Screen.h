#ifndef RAYLIBSTARTER_DEATH_SCREEN_H
#define RAYLIBSTARTER_DEATH_SCREEN_H

#include "Menu.hpp"
#include <vector>
#include <string>

class Death_Screen : public Menu {
public:
    Death_Screen();

    // Überschreibt die Funktionen der Basisklasse
    void Update() override;

    void Draw() override;

    // Gibt die aktuelle Wahl zurück (0, 1, etc.)
    int GetChoice() const { return choice; }

    // Setzt die Wahl zurück.
    // WICHTIG: Auf -1 setzen, sonst denkt das Spiel,
    // du hättest sofort wieder "Erneut versuchen" (0) oder "Menu" (1) gedrückt.
    void ResetChoice() { choice = -1; }
};

#endif //RAYLIBSTARTER_DEATH_SCREEN_H