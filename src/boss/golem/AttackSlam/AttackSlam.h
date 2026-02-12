

#ifndef RAYLIBSTARTER_ATTACKSLAM_H
#define RAYLIBSTARTER_ATTACKSLAM_H

#include "raylib.h"

class AttackSlam {
public:
    AttackSlam();

    void init();
    void reset();
    void Update(float dt, Vector2 bossPos, Vector2 playerPos);
    void Draw(float dt, Vector2 bossPos, Vector2 playerPos);

    struct Slam {};
};


#endif //RAYLIBSTARTER_ATTACKSLAM_H