#include "highscore.h"

#include <fstream>
#include <sstream>
#include <algorithm>
#include <cstdio>


// ================= TIMER =================

RunTimer::RunTimer()
{
    elapsedMs = 0;
    running = false;
}

void RunTimer::Reset()
{
    elapsedMs = 0;
    running = false;
}

void RunTimer::Start()
{
    running = true;
}

void RunTimer::Stop()
{
    running = false;
}

void RunTimer::Update(float dt)
{
    if (!running)return;

    elapsedMs += (long long)(dt * 1000.0f);
}

std::string RunTimer::FormatMinSecMs(long long ms)
{
    long long totalSec = ms / 1000;
    long long milli = ms % 1000;

    long long min = totalSec / 60;
    long long sec = totalSec % 60;

    char buffer[64];

    sprintf(buffer,
            "%02lld:%02lld:%03lld",
            min,
            sec,
            milli);

    return std::string(buffer);
}


// ================= BOARD =================

void HighscoreBoard::Load(const std::string& file)
{
    entries.clear();

    std::ifstream in(file);

    if (!in.is_open()) return;

    std::string line;

    while (std::getline(in, line))
    {
        std::stringstream ss(line);

        std::string name;
        std::string timeStr;

        if (!std::getline(ss, name, ',')) continue;
        if (!std::getline(ss, timeStr)) continue;

        HighscoreEntry entry;

        entry.name = name;
        entry.timeMs = std::stoll(timeStr);

        entries.push_back(entry);
    }

    SortAndTrim(10);
}


void HighscoreBoard::Save(const std::string& file)
{
    std::ofstream out(file, std::ios::trunc);

    for (auto& e : entries)
    {
        out << e.name << "," << e.timeMs << "\n";
    }
}


void HighscoreBoard::AddAndPersist(const std::string& file,
                                    const HighscoreEntry& entry,
                                    int maxKeep)
{
    entries.push_back(entry);

    SortAndTrim(maxKeep);

    Save(file);
}


void HighscoreBoard::SortAndTrim(int maxKeep)
{
    std::sort(entries.begin(),
              entries.end(),
              [](const HighscoreEntry& a,
                 const HighscoreEntry& b)
              {
                  return a.timeMs < b.timeMs;
              });

    if (entries.size() > maxKeep)
        entries.resize(maxKeep);
}

void HighscoreBoard::Reset(const std::string& file)
{
    // 1. Alle Einträge aus dem aktuellen Vektor löschen
    entries.clear();

    // 2. Die Datei auf der Festplatte leeren (überschreiben)
    // std::ios::trunc sorgt dafür, dass die Datei danach 0 Bytes groß ist
    std::ofstream out(file, std::ios::trunc);

    // Optional: Falls die Datei nicht geöffnet werden konnte,
    // ist das meist kein Problem, da wir ja ohnehin "nichts" haben wollen.
    out.close();
}

void HighscoreBoard::Draw(int x, int y)
{
    DrawText("HIGHSCORE BOARD",
             x,
             y,
             40,
             WHITE);

    int offsetY = y + 60;

    if (entries.empty())
    {
        DrawText("No scores yet",
                 x,
                 offsetY,
                 24,
                 GRAY);

        return;
    }

    for (int i = 0; i < entries.size(); i++)
    {
        const auto& e = entries[i];

        std::string line =
                std::to_string(i + 1)
                + ". "
                + e.name
                + "  "
                + RunTimer::FormatMinSecMs(e.timeMs);

        DrawText(line.c_str(),
                 x,
                 offsetY,
                 24,
                 RAYWHITE);

        offsetY += 30;
    }
}


// ================= NAME INPUT =================

NameInput::NameInput()
{
    minLength = 1;
    maxLength = 12;
}

void NameInput::Clear()
{
    text.clear();
}


void NameInput::Update()
{
    int key = GetCharPressed();

    while (key > 0)
    {
        if (text.size() < maxLength)
        {
            if (key >= 32 && key <= 126)
            {
                text.push_back((char)key);
            }
        }

        key = GetCharPressed();
    }

    if (IsKeyPressed(KEY_BACKSPACE))
    {
        if (!text.empty())
            text.pop_back();
    }
}


void NameInput::Draw(int x, int y)
{
    DrawText("ENTER YOUR NAME",
             x,
             y,
             30,
             WHITE);

    DrawRectangleLines(x,
                       y + 40,
                       300,
                       40,
                       WHITE);

    DrawText(text.c_str(),
             x + 10,
             y + 50,
             24,
             RAYWHITE);

    DrawText("Press ENTER to save",
             x,
             y + 100,
             20,
             GRAY);
}
