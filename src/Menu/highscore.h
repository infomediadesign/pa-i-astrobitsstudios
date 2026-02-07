

#ifndef RAYLIBSTARTER_HIGHSCORE_H
#define RAYLIBSTARTER_HIGHSCORE_H

#pragma once

#include "raylib.h"
#include <string>
#include <vector>

// ================= TIMER =================

struct RunTimer {

    long long elapsedMs;
    bool running;

    RunTimer();

    void Reset();

    void Start();

    void Stop();

    void Update(float dt);

    static std::string FormatMinSecMs(long long ms);

};


// ================= ENTRY =================

struct HighscoreEntry {

    std::string name;
    long long timeMs;

};


// ================= BOARD =================

struct HighscoreBoard {

    std::vector<HighscoreEntry> entries;

    void Load(const std::string& file);

    void Save(const std::string& file);

    void AddAndPersist(const std::string& file,
                       const HighscoreEntry& entry,
                       int maxKeep = 10);

    void SortAndTrim(int maxKeep);

    void Draw(int x, int y);

};


// ================= NAME INPUT =================

struct NameInput {

    std::string text;

    int maxLength;

    NameInput();

    void Clear();

    void Update();

    void Draw(int x, int y);

};



#endif //RAYLIBSTARTER_HIGHSCORE_H