//
// Created by Ben on 02.12.2025.
//

#pragma once


class Cooldown {
    public:
        Cooldown(float duration) : duration(duration), timer(0) {}

        void Update(float dt) {
            if (timer > 0) timer -= dt;
        }

        bool Ready() const {
            return timer <= 0;
        }

        void Trigger() {
            timer = duration;
        }
        float Duration() const {
            return duration;
        }
        float Remaining() const {
            return timer;
        }

    private:
        float duration;
        float timer;
    };


