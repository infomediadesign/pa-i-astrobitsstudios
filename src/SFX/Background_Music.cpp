#include "Background_Music.h"
#include <raylib.h>

Background_Music::~Background_Music() {
    Unload();
}

void Background_Music::Init(const char* filePath) {
    // Unload previous if loaded
    if (loaded) Unload();

    if (filePath == nullptr || !FileExists(filePath)) {
        loaded = false;
        TraceLog(LOG_WARNING, "Background_Music::Init - file not found: %s", filePath ? filePath : "(null)");
        return;
    }

    music = LoadMusicStream(filePath);

    // Check if raylib considers the music ready
    loaded = IsMusicReady(music);
    if (loaded) {
        // set default volume and make sure it loops
        SetMusicVolume(music, 1.0f);
        // raylib music streams loop if requested via PlayMusicStream repeatedly;
        // there's no direct SetLoop in older versions, PlayMusicStream handles streaming.
        TraceLog(LOG_INFO, "Background_Music::Init - loaded: %s", filePath);
    } else {
        TraceLog(LOG_WARNING, "Background_Music::Init - failed to load: %s", filePath);
    }
}

void Background_Music::Play() {
    if (!loaded) return;
    if (!IsAudioDeviceReady()) {
        TraceLog(LOG_WARNING, "Background_Music::Play - audio device not ready");
        return;
    }
    if (!IsMusicStreamPlaying(music)) {
        PlayMusicStream(music);
        // Ensure the stream starts immediately
        UpdateMusicStream(music);
        TraceLog(LOG_INFO, "Background_Music::Play - started playback");
    }
}

void Background_Music::Stop() {
    if (!loaded) return;
    StopMusicStream(music);
}

void Background_Music::Update() {
    if (!loaded) return;
    UpdateMusicStream(music);
    // Ensure playback continues (raylib samples often re-play if stopped on window restore)
    if (!IsMusicStreamPlaying(music)) PlayMusicStream(music);
}

void Background_Music::Unload() {
    if (!loaded) return;
    UnloadMusicStream(music);
    loaded = false;
    TraceLog(LOG_INFO, "Background_Music::Unload - music unloaded");
}

::Music& Background_Music::GetStream() {
    return music;
}
