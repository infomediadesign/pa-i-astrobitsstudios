//
// Created by justu on 02/03/2026.
//

#ifndef RAYLIBSTARTER_ATTACKBIGDASH_H
#define RAYLIBSTARTER_ATTACKBIGDASH_H

#include "raylib.h"

class AttackBigDash {
public:
    AttackBigDash();

    void Init();

    void Reset();

    void Update(float dt, Vector2 bossPos, Vector2 playerPos);

    void Draw(Vector2 bossPos) const;

    bool isActive();
    // returns damage or 0.0f (or marker) when hit
    //float CheckDamage(float dt, Vector2 bossPos, Rectangle playerRect);

    void StartBigDash(Vector2 bossPos, Vector2 playerPos);

private:
    Rectangle attackArea;
    Vector2 origin;    // Drehpunkt (meist linke Mitte des Rechtecks)

    float rotation;
    bool charging;
    float speed;
    bool active;
};


#endif //RAYLIBSTARTER_ATTACKBIGDASH_H