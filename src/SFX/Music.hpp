//
// Created by benco on 24.02.2026.
//

#ifndef RAYLIBSTARTER_MUSIC_HPP
#define RAYLIBSTARTER_MUSIC_HPP

// Minimal includes - avoid pulling in raylib's global `Music` symbol
// unless this header actually needs raylib types.

namespace SFX {

// Simple base class for project-specific music handling.
class Music {
public:
    Music() = default;
    virtual ~Music() = default;



protected:
};

} // namespace SFX


#endif //RAYLIBSTARTER_MUSIC_HPP