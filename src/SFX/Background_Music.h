#ifndef RAYLIBSTARTER_BACKGROUND_MUSIC_H
#define RAYLIBSTARTER_BACKGROUND_MUSIC_H

#include <raylib.h>
#include "Music.hpp"

// Use fully-qualified name to avoid depending on a global `Music` symbol
class Background_Music : public SFX::Music {
public:
    Background_Music() = default;
    ~Background_Music() override;

    // Lade die Musik-Datei und bereite den Stream vor.
    // Legt `loaded` intern fest (true wenn geladen), kein Rückgabewert mehr.
    void Init(const char* filePath);

    // Prüft, ob die Musik erfolgreich geladen wurde
    [[nodiscard]] bool IsLoaded() const { return loaded; }

    // Spielt den geladenen Music-Stream ab (falls geladen)
    void Play();

    // Stoppt den Stream (falls abgespielt)
    void Stop();

    // Muss in der Spiel-Loop aufgerufen werden, um den Musikstream zu aktualisieren
    void Update();

    // Unloadt den Stream und gibt Ressourcen frei
    void Unload();

    // Zugriff auf den internen raylib ::Music Stream
    ::Music& GetStream();

private:
    ::Music music{}; // raylib music stream (global namespace)
    bool loaded = false;
};


#endif //RAYLIBSTARTER_BACKGROUND_MUSIC_H