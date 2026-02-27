#include <cstdlib>
#include <algorithm>
#include "raylib.h"
#include "config.h"
#include <cmath>
#include"cooldown.h"
#include <vector>
#include "boss/golem/GolemController/GolemController.h"


#include "player/movement/controller.h"
#include "enviroment/walls.h"
#include "player/combat/plattack.h"
#include "player/schaden/schaden.h"
#include "Menu/MainMenu_UI.h"
#include "Menu/Options.h"
#include "Menu/Death_Screen.h"
#include "Menu/pauseMenu.h"
#include "Menu/highscore.h"
#include "boss/golem/boss_Angriff.h"
#include "Menu/UpgradeScreen.h"
#include "SFX/Background_Music.h"
#include "player/PlayerUpgrades/Upgrades.h"


int main() {
    Cooldown attackCD(0.5f);
    Cooldown dashCD(3.0f);
    // Jumpattack is now handled by the boss system; removed standalone cooldown
    //Cooldown jumpAttackCD(1.75f);
    // Raylib initialization
    // Project name, screen size, fullscreen mode etc. can be specified in the config.h file
    SetConfigFlags(FLAG_WINDOW_RESIZABLE | FLAG_VSYNC_HINT | FLAG_WINDOW_UNDECORATED);
    InitWindow(GetMonitorWidth(0), GetMonitorHeight(0), Game::PROJECT_NAME);

    // Initialize audio device for raylib audio playback
    InitAudioDevice();
    if (!IsAudioDeviceReady()) {
        TraceLog(LOG_WARNING, "Audio device not ready - background music will not play.");
    } else {
        // Optional: set master volume to full
        SetMasterVolume(1.0f);
    }

    SetExitKey(KEY_NULL);
    SetTargetFPS(60);

#ifdef GAME_START_FULLSCREEN
    ToggleFullscreen();
#endif
    enum GameState {
        STATE_MENU = -1,
        STATE_BOSS_1 = 0,
        STATE_OPTIONS = 1,
        STATE_EXIT = 2,
        STATE_DEATH = 3,
        STATE_PAUSE = 4,
        STATE_VICTORY = 5,
        STATE_NAME_ENTRY = 6,
        STATE_HIGHSCORES = 7,
        STATE_UPGRADES = 8,
        STATE_BOSS_2 = 9
    };

    MainMenu mainMenu;
    Options options;
    pauseMenu pauseMenu;
    GameState currentState = STATE_MENU;
    GameState previousState = STATE_BOSS_1;
    Death_Screen deathScreen;
    RunTimer runTimer;
    HighscoreBoard board;
    NameInput nameInput;
    UpgradeScreen upgradeScreen;

    const std::string SCORE_FILE = "highscores.csv";
    board.Load(SCORE_FILE);
    Player hp{}; // value-initialize to remove uninitialized warnings
    GolemController golem;
    controller player;
    plattack melee;
    melee.Init();
    golem.Init();
    player.Init();
    hp.Init();
    BossAngriff bossAtk;
    bossAtk.Init();
    Upgrades Upgrades;

    // Background music instance (loads a file and plays it)
    Background_Music bgm;
    bool bgmStarted = false;
    // NOTE: Passe den Pfad zur vorhandenen Audiodatei in deinem Projekt an.
    bgm.Init("assets/audio/sfx/Thememusic.mp3");
    if (IsAudioDeviceReady() && bgm.IsLoaded()) {
        bgm.Play();
        bgmStarted = true;
        TraceLog(LOG_INFO, "Background music started successfully");
    } else if (!bgm.IsLoaded()) {
        TraceLog(LOG_WARNING, "Background music could not be loaded: %s", "assets/audio/sfx/Thememusic.mp3");
    } else {
        TraceLog(LOG_WARNING, "Audio device not ready - music will not play immediately");
    }

    std::vector<Wall> walls = {
        {0, 0, 1, (float) Game::ScreenHeight}, // Links
        {(float) Game::ScreenWidth - 5, 0, 1, (float) Game::ScreenHeight}, // Rechts
        {0, 40, (float) Game::ScreenWidth, 5}, // Oben
        {0, (float) Game::ScreenHeight - 50, (float) Game::ScreenWidth, 1} // Unten
    };
    Texture2D background = LoadTexture("assets/graphics/backgrounds/Background1_Boss_Room.png");
    // Your own initialization code here
    // ...
    // ...
    RenderTexture2D canvas = LoadRenderTexture(Game::ScreenWidth, Game::ScreenHeight);
    float renderScale{}; // this and the line below are relevant to drawing later.
    Rectangle renderRec{};


    // Main game loop
    while (!WindowShouldClose() && currentState != STATE_EXIT) {
        float dt = GetFrameTime();

        // Apply global volume from Options and update music stream
        float currentVolume = options.IsMuted() ? 0.0f : options.GetMasterVolume();
        if (IsAudioDeviceReady()) {
            SetMasterVolume(currentVolume);
            if (bgm.IsLoaded()) SetMusicVolume(bgm.GetStream(), currentVolume);

            if (bgm.IsLoaded()) {
                if (!bgmStarted) {
                    bgm.Play();
                    bgmStarted = true;
                    TraceLog(LOG_INFO, "Background music started in loop after audio became ready");
                }
                bgm.Update();
            }
        }

        // --- 1. LOGIK UPDATE ---
        switch (currentState) {
            case STATE_PAUSE:
                pauseMenu.Update();

                // Zurück zum Spiel mit ESC oder Button 0
                if (IsKeyPressed(KEY_ESCAPE) || pauseMenu.GetChoice() == 0) {
                    pauseMenu.ResetChoice();
                    runTimer.Start();
                    currentState = STATE_BOSS_1;
                } else if (pauseMenu.GetChoice() == 1) {
                    pauseMenu.ResetChoice();
                    pauseMenu.Open();
                    previousState = STATE_PAUSE;
                    currentState = STATE_OPTIONS;
                } else if (pauseMenu.GetChoice() == 2) {
                    pauseMenu.ResetChoice();
                    pauseMenu.Open();
                    runTimer.Reset();
                    currentState = STATE_MENU;
                }
                break;

            case STATE_BOSS_1: {
                player.Update(dt, walls);
                golem.Update(dt);
                hp.Update(dt);
                attackCD.Update(dt);
                dashCD.Update(dt);


                player.Animate(dt);
                runTimer.Update(dt);


                //==========================
                Rectangle br = golem.GetRect();
                Vector2 bossPos = {br.x + br.width / 2.0f, br.y + br.height / 2.0f};
                bossAtk.SetBossHP(golem.getHealth(), golem.getMaxHealth());

                bossAtk.Update(dt, bossPos, player.GetPos(), player.GetCollision(), hp, golem);

                float dmg = bossAtk.CheckDamage(dt, bossPos, player.GetCollision());
                if (dmg > 0) hp.TakeDamage(static_cast<int>(dmg));


                // Pause aktivieren
                if (IsKeyPressed(KEY_ESCAPE)) {
                    runTimer.Stop();
                    currentState = STATE_PAUSE;
                }


                // ... Rest deiner Kampf-Logik (Dash, Melee etc.) ...
                if (dashCD.Ready() && IsKeyPressed(KEY_LEFT_SHIFT) && player.getMoving()) {
                    player.Dash(walls, dt);
                    hp.invincibleTimer = hp.invincibleDuration;
                    dashCD.Trigger();
                }
                melee.UpdateDirection();

                if (attackCD.Ready() && IsKeyPressed(KEY_SPACE)) {
                    melee.Start(player.GetPos(), player.GetSize());
                    attackCD.Trigger();
                    if (CheckCollisionRecs(melee.hitBox, golem.GetDmgBox())) {
                        golem.takeDamage(melee.damage);
                    }
                }
                melee.Update(dt, player.GetPos(), player.GetSize());

                if (hp.Gethealth() <= 0) {
                    runTimer.Reset();
                    currentState = STATE_DEATH;
                }
                if (!golem.isAlive()) {
                    runTimer.Stop();
                    nameInput.Clear();
                    currentState = STATE_UPGRADES;
                }
                break;
            }
            case STATE_OPTIONS:
                // Options menu logic
                options.Update();

                if (IsAudioDeviceReady()) {
                    if (bgm.IsLoaded()) {


                        // Lautstärke per Schritt ändern (einmaliger Tastendruck)
                        float volOpt = options.GetMasterVolume();
                        const float volStep = 0.05f;
                        if (IsKeyPressed(KEY_D)) {
                            volOpt = std::min(1.0f, volOpt + volStep);
                            options.SetMasterVolume(volOpt);
                        }
                        if (IsKeyPressed(KEY_A)) {
                            volOpt = std::max(0.0f, volOpt - volStep);
                            options.SetMasterVolume(volOpt);
                        }

                        // Muten (Options bereits unterstützt, hier nur Shortcut)
                        if (IsKeyPressed(KEY_M)) {
                            options.ToggleMute();
                        }

                        // Sicherstellen, dass die Musik-Lautstärke dem Options-Wert folgt
                        float currentVolFromOptions = options.IsMuted() ? 0.0f : options.GetMasterVolume();
                        SetMusicVolume(bgm.GetStream(), currentVolFromOptions);

                    } else {
                        // Kein Track geladen: kurze Info-Trace auf Taste P
                        if (IsKeyPressed(KEY_P) || IsKeyPressed(KEY_O)) {
                            TraceLog(LOG_WARNING, "No background music loaded to control from Options");
                        }
                    }
                } else {
                    // Audiogerät nicht bereit: informieren
                    if (IsKeyPressed(KEY_P) || IsKeyPressed(KEY_O) || IsKeyPressed(KEY_UP) || IsKeyPressed(KEY_DOWN)) {
                        TraceLog(LOG_WARNING, "Audio device not ready - cannot control music from Options");
                    }
                }

                if (IsKeyPressed(KEY_ESCAPE)) {
                    currentState = previousState;
                }
                break;

            case STATE_MENU:
                mainMenu.Update();
                if (mainMenu.GetChoice() == 0) {
                    hp.Init();
                    melee.Reset();
                    player.Reset();
                    golem.Init();
                    bossAtk.Init();
                    runTimer.Reset();
                    runTimer.Start();
                    previousState = STATE_MENU;
                    currentState = STATE_BOSS_1;
                    hp.invincibleTimer = hp.invincibleDuration;
                    mainMenu.ResetChoice();
                } else if (mainMenu.GetChoice() == 1) {
                    previousState = STATE_MENU;
                    currentState = STATE_OPTIONS;
                    mainMenu.ResetChoice();
                } else if (mainMenu.GetChoice() == 3) {
                    currentState = STATE_EXIT;
                } else if (mainMenu.GetChoice() == 2) {
                    previousState = STATE_MENU;
                    currentState = STATE_HIGHSCORES;
                    mainMenu.ResetChoice();
                }
                break;

            case STATE_DEATH:
                deathScreen.Update();
                if (deathScreen.GetChoice() == 0) {
                    hp.Init();
                    player.Reset();
                    melee.Reset();
                    golem.Init();
                    bossAtk.Init();
                    hp.invincibleTimer = hp.invincibleDuration;
                    runTimer.Reset();
                    runTimer.Start();

                    deathScreen.ResetChoice();
                    currentState = STATE_BOSS_1;
                } else if (deathScreen.GetChoice() == 0) {
                    deathScreen.ResetChoice();
                    mainMenu.ResetChoice();
                    runTimer.Reset();
                    currentState = STATE_MENU;
                }
                break;

            case STATE_NAME_ENTRY:
                nameInput.Update();
                if (IsKeyPressed(KEY_ENTER)) {
                    if (!nameInput.text.empty()) {
                        HighscoreEntry e{nameInput.text, runTimer.elapsedMs};
                        board.AddAndPersist(SCORE_FILE, e, 10); //
                        currentState = STATE_HIGHSCORES;
                    } else {
                    }
                }


                // Optional: Drücken Sie ESC, um das Speichern abzubrechen (dies führt dazu, dass Sie den Level abschließen,
                // ohne ihn an die Bestenliste zu übermitteln).
                if (IsKeyPressed(KEY_ESCAPE)) {
                    runTimer.Reset();
                    currentState = STATE_MENU;
                }
                break;

            case STATE_HIGHSCORES:
                if (IsKeyPressed(KEY_ESCAPE) || IsKeyPressed(KEY_ENTER)) {
                    currentState = STATE_MENU;
                }
                if (IsKeyPressed(KEY_L)) {
                    board.Reset("scores.csv");
                }

                break;
            case STATE_UPGRADES:
                runTimer.Stop();
                upgradeScreen.Update();
                if (upgradeScreen.GetChoice() == 0) {
                    Upgrades.Upgrade1(hp,melee);
                    upgradeScreen.ResetChoice();
                    golem.Init();
                    currentState = STATE_BOSS_1;
                }
                if (upgradeScreen.GetChoice() == 1) {
                    Upgrades.Upgrade2(hp,player);
                    upgradeScreen.ResetChoice();
                    golem.Init();
                    currentState = STATE_BOSS_1;
                }
                if (upgradeScreen.GetChoice() == 2) {
                    Upgrades.Upgrade3(melee,player);
                    upgradeScreen.ResetChoice();
                    golem.Init();
                    currentState = STATE_BOSS_1;
                }
                    //currentState = STATE_BOSS_2;

                break;
            case STATE_BOSS_2:
                // Hier könntest du den nächsten Bosskampf oder das nächste Level starten
                if (IsKeyPressed(KEY_ESCAPE)) {
                    runTimer.Stop();
                    previousState = STATE_BOSS_2;
                    currentState = STATE_PAUSE;
                }
                // ... Rest deiner Kampf-Logik (Dash, Melee etc.) ...
                if (dashCD.Ready() && IsKeyPressed(KEY_LEFT_SHIFT) && player.getMoving()) {
                    player.Dash(walls, dt);
                    hp.invincibleTimer = hp.invincibleDuration;
                    dashCD.Trigger();
                }
                melee.UpdateDirection();
                if (hp.Gethealth() <= 0) {
                    runTimer.Reset();
                    currentState = STATE_DEATH;
                }
                //currentState = STATE_NAME_ENTRY;
                break;

            default:
                // Fallback: nichts tun. Dies verhindert statische Analyse-Warnungen
                break;

        }

        BeginDrawing();
        // You can draw on the screen between BeginDrawing() and EndDrawing()
        // For the letterbox we draw on canvas instead

        BeginTextureMode(canvas);
        {
            //Within this block is where we draw our app to the canvas and YOUR code goes.
            ClearBackground(WHITE);

            //Logik-Weiche (Draw) ===
            if (currentState == STATE_OPTIONS) {
                options.Draw();
            } else if (currentState == STATE_MENU) {
                mainMenu.Draw();
            } else if (currentState == STATE_BOSS_1) {
                Rectangle br = golem.GetRect();
                Vector2 bossPosForDraw = {br.x + br.width / 2.0f, br.y + br.height / 2.0f};

                DrawTexture(background, 0, 0, WHITE);
                DrawRectangle(380, 30, 200, 45, Fade(BLACK, 0.6));

                bossAtk.Draw(bossPosForDraw);

                player.Draw();
                golem.Draw();

                if (melee.active) {
                    melee.Draw();
                    DrawRectangleRec(melee.hitBox,WHITE);
                }
                if (dashCD.Ready())
                    DrawText("Ready", 150, 20, 10, BLUE);
                else DrawText(TextFormat("Cooldown %.2f", dashCD.Remaining()), 150, 20, 10, BLUE);

                if (attackCD.Ready())
                    DrawText("Ready", 20, 20, 10, GREEN);
                else DrawText(TextFormat("Cooldown %.2f", attackCD.Remaining()), 20, 20, 10, GREEN);





                if (CheckCollisionRecs(player.GetCollision(), golem.GetRect()) && golem.isAlive()) {
                    hp.TakeDamage(10);
                }
                hp.Draw(player.GetCollision());
                DrawText(("Time: " + RunTimer::FormatMinSecMs(runTimer.elapsedMs)).c_str(),
                         395, 40, 24, WHITE);

            } else if (currentState == STATE_BOSS_2) {
                ClearBackground(BLACK);
                DrawText("Boss 2 Kampf (noch nicht implementiert)", 60, 60, 30, RED);
                DrawText("Drücke ESC, um zu pausieren", 60, 120, 20, GRAY);
                DrawText(TextFormat("Der Wert dmg ist: %.2f", melee.damage),200,200,20,WHITE);
                DrawText(TextFormat("Der Wert hp ist: %.2f", hp.maxHp),400,200,20,WHITE);
            }


            else if (currentState == STATE_DEATH || currentState == STATE_PAUSE) {
                // Zeichne evtl. den Spieler/Boss noch (starr), damit es nicht leer aussieht
                DrawTexture(background, 0, 0, GRAY);
                player.Draw();
                golem.Draw();
                hp.Draw(player.GetCollision());
                DrawText(("Time: " + RunTimer::FormatMinSecMs(runTimer.elapsedMs)).c_str(), 395, 40, 24, WHITE);

                // Zeichne das passende Menü
                if (currentState == STATE_PAUSE) {
                    pauseMenu.Draw();
                }
                if (currentState == STATE_DEATH) {
                    deathScreen.Draw();
                }
            } else if (currentState == STATE_NAME_ENTRY) {
                ClearBackground(BLACK);
                DrawText("VICTORY!", 60, 60, 50, GREEN);
                DrawText(("Your time: " + RunTimer::FormatMinSecMs(runTimer.elapsedMs)).c_str(),
                         60, 130, 28, RAYWHITE);
                nameInput.Draw(60, 200);

                if (nameInput.text.empty()) {
                    DrawText("Please enter at least one character!", 60, 320, 20, RED);
                }
            } else if (currentState == STATE_HIGHSCORES) {
                ClearBackground(BLACK);
                board.Draw(60, 60);
                DrawText("ENTER/ESC: Back", 60, 520, 22, GRAY);
            } else if (currentState == STATE_UPGRADES) {
                upgradeScreen.Draw();
            }
        }
        EndTextureMode();

        //The following lines put the canvas in the middle of the window and have the negative as black
        ClearBackground(BLACK); // If you want something else than a black void in the background

        renderScale = std::min(static_cast<float>(GetScreenHeight()) / static_cast<float>(canvas.texture.height),
                               // Calculates how big or small the canvas has to be rendered.
                               static_cast<float>(GetScreenWidth()) / static_cast<float>(canvas.texture.width));
        // Priority is given to the smaller side.
        renderScale = floorf(renderScale);
        if (renderScale < 1) renderScale = 1; // Ensure that scale is at least 1.
        renderRec.width = static_cast<float>(canvas.texture.width) * renderScale;
        renderRec.height = static_cast<float>(canvas.texture.height) * renderScale;
        renderRec.x = (static_cast<float>(GetScreenWidth()) - renderRec.width) / 2.0f;
        renderRec.y = (static_cast<float>(GetScreenHeight()) - renderRec.height) / 2.0f;

        DrawTexturePro(canvas.texture,
                       Rectangle{0, 0, static_cast<float>(canvas.texture.width), static_cast<float>(-canvas.texture.height)},
                       renderRec,
                       {}, 0, WHITE);

        if (IsKeyDown(KEY_LEFT_CONTROL) && IsKeyDown(KEY_S)) {
            DrawText(TextFormat("Render scale: %.0f", renderScale), 10, 10, 20, LIGHTGRAY);
        }
        EndDrawing();
    }
    // De-initialization here
    // ...
    // ...
    melee.Unload();
    golem.Unload();
    player.Unload();

    UnloadRenderTexture(canvas);

    // Unload background music and close audio
    if (IsAudioDeviceReady()) {
        bgm.Unload();
        CloseAudioDevice();
    } else {
        // Ensure resources are still released safely
        bgm.Unload();
    }

    // Close window and OpenGL context
    CloseWindow();

    return EXIT_SUCCESS;
};
