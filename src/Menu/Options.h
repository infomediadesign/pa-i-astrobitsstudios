//
// Created by Ben on 23.01.2026.
//

#ifndef RAYLIBSTARTER_OPTIONS_H
#define RAYLIBSTARTER_OPTIONS_H

#include "Menu.hpp"
#include <algorithm>
#include <vector>
#include <string>

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

    // Difficulty access
    [[nodiscard]] int GetDifficultyIndex() const { return difficultyIndex; }
    [[nodiscard]] std::string GetDifficultyName() const { return difficulties.at(difficultyIndex); }

    // Persistence
    bool LoadSettings(const std::string &path);
    bool SaveSettings(const std::string &path) const;

private:
    float masterVolume = 1.0f; // 0..1
    bool muted = false;
    // Difficulty options
    std::vector<std::string> difficulties = {"Easy", "Normal", "Hard"};
    int difficultyIndex = 1; // default: Normal
};


#endif //RAYLIBSTARTER_OPTIONS_H

