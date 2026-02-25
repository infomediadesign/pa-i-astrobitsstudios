//
// Created by Ben on 23.01.2026.
//

#ifndef RAYLIBSTARTER_OPTIONS_H
#define RAYLIBSTARTER_OPTIONS_H

#include "Menu.hpp"
#include <algorithm>

class Options : public Menu{
public:
    Options();
    void Update() override;
    void Draw() override;

    // Lautstärke-API
    [[nodiscard]] float GetMasterVolume() const { return masterVolume; }
    void SetMasterVolume(float v); // implemented in .cpp (adds logging)
    [[nodiscard]] bool IsMuted() const { return muted; }
    void ToggleMute(); // implemented in .cpp (adds logging)

private:
    float masterVolume = 1.0f; // 0..1
    bool muted = false;
};


#endif //RAYLIBSTARTER_OPTIONS_H