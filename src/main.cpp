#include <cstdlib>
#include <algorithm>
#include "raylib.h"
#include "config.h"
#include <math.h>
#include"cooldown.h"
#include <vector>
#include "Sprite.h"
// #include "boss/golem/AttackJump/AttackJump.h" // removed: jump handled by BossAngriff
#include "boss/golem/GolemController/GolemController.h"

#include "enviroment/background.h"
#include "player/movement/controller.h"
#include "enviroment/walls.h"
#include "player/combat/plattack.h"
#include "player/schaden/schaden.h"
#include "Menu/MainMenu_UI.h"
#include "Menu/Options.h"
#include "Menu/Death_Screen.h"
#include "Menu/pauseMenu.h"
#include "Menu/highscore.h"
#include "boss/golem/Golem_Angriff.h"


int main() {
    Cooldown attackCD(0.5f);
    Cooldown dashCD(3.0f);
    // Jumpattack is now handled by the boss system; removed standalone cooldown
    //Cooldown jumpAttackCD(1.75f);
    // Raylib initialization
    // Project name, screen size, fullscreen mode etc. can be specified in the config.h file
    SetConfigFlags(FLAG_WINDOW_RESIZABLE | FLAG_VSYNC_HINT | FLAG_WINDOW_UNDECORATED);
    InitWindow(GetMonitorWidth(0), GetMonitorHeight(0), Game::PROJECT_NAME);
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
        STATE_LOADING = 8,
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

    const std::string SCORE_FILE = "highscores.csv";
    board.Load(SCORE_FILE);
    Player hp;
    GolemController golem;
    controller player;
    plattack melee;
    // AttackJump attack_jump;  // removed: jump handled by BossAngriff
    melee.Init();
    golem.Init();
    player.Init();
    hp.Init();
    BossAngriff bossAtk;
    bossAtk.Init();

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
                pauseMenu.Open();
                player.Animate(dt);
                runTimer.Update(dt);


                //==========================
                Rectangle br = golem.GetRect();
                Vector2 bossPos = {br.x + br.width / 2.0f, br.y + br.height / 2.0f};
                bossAtk.SetBossHP(golem.getHealth(), golem.getMaxHealth());

                bossAtk.Update(dt, bossPos, player.GetPos(), player.GetCollision(), hp, golem);

                float dmg = bossAtk.CheckDamage(dt, bossPos, player.GetCollision());
                if (dmg > 0) hp.TakeDamage(dmg);


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
                    currentState = STATE_LOADING;
                }
                break;
            }
            case STATE_OPTIONS:
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
                } else if (deathScreen.GetChoice() == 1) {
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
            case STATE_LOADING:
                runTimer.Stop();
                Image GenImageColor(int Screenwidth, int Screenheight, Color color);
                // Hier könntest du einen Ladebildschirm anzeigen oder eine kurze Verzögerung einbauen
                // Zum Beispiel:
                ClearBackground(BLACK);
                DrawText("Loading...", 60, 60, 50, GREEN);
                DrawText("Please wait while we prepare your victory screen.", 60, 130, 28, RAYWHITE);
                EndDrawing();
                // Simuliere Ladezeit
                for (int i = 0; i < 120; i++) {
                    // etwa 2 Sekunden bei 60 FPS
                    BeginDrawing();
                    ClearBackground(BLACK);
                    DrawText("Loading...", 60, 60, 50, GREEN);
                    DrawText("Please wait while we prepare your victory screen.", 60, 130, 28, RAYWHITE);
                    EndDrawing();
                    WaitTime(1.0f / 60.0f); // Warte auf den nächsten Frame
                    currentState = STATE_BOSS_2; // Wechsel zum nächsten Zustand nach der Ladezei
                }
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
                currentState = STATE_NAME_ENTRY;
                break;
            default:
                // Unhandled states: do nothing
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
                Rectangle hb = golem.GetDmgBox();
                Vector2 bossPosForDraw = {br.x + br.width / 2.0f, br.y + br.height / 2.0f};

                DrawTexture(background, 0, 0, WHITE);
                DrawRectangle(380, 30, 200, 45, Fade(BLACK, 0.6));

                bossAtk.Draw(bossPosForDraw);

                player.Draw();
                golem.Draw();

                //Hitboxen Zeichnen
                // DrawRectangleRec(hb, YELLOW);
                //DrawRectangleRec(br, BLUE);

                //DrawRectangleRec(player.GetHitbox(), GREEN);


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
                    //   DrawRectangle(0,0,Game::ScreenWidth,Game::ScreenHeight,Fade(RED,0.3));
                }
                hp.Draw(player.GetCollision());
                DrawText(("Time: " + RunTimer::FormatMinSecMs(runTimer.elapsedMs)).c_str(),
                         395, 40, 24, WHITE);
                if (hp.takeDamage) {
                    //  DrawRectangle(0,0,Game::ScreenWidth,Game::ScreenHeight,Fade(RED,0.3));
                }
            } else if (currentState == STATE_DEATH || currentState == STATE_PAUSE) {
                // Zeichne evtl. den Spieler/Boss noch (starr), damit es nicht leer aussieht
                DrawTexture(background, 0, 0, GRAY);
                player.Draw();
                golem.Draw();
                hp.Draw(player.GetCollision());
                DrawText(("Time: " + RunTimer::FormatMinSecMs(runTimer.elapsedMs)).c_str(), 395, 40, 24, WHITE);

                // Jetzt den roten Text drüber zeichnen
                if (currentState == STATE_PAUSE) {
                    pauseMenu.Draw();
                } else if (currentState == STATE_DEATH) {
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
            }
        }
        EndTextureMode();
        //The following lines put the canvas in the middle of the window and have the negative as black
        ClearBackground(BLACK); // If you want something else than a black void in the background
        // then you can add stuff here.


        renderScale = std::min(GetScreenHeight() / (float) canvas.texture.height,
                               // Calculates how big or small the canvas has to be rendered.
                               GetScreenWidth() / (float) canvas.texture.width);
        // Priority is given to the smaller side.
        renderScale = floorf(renderScale);
        if (renderScale < 1) renderScale = 1; // Ensure that scale is at least 1.
        renderRec.width = canvas.texture.width * renderScale;
        renderRec.height = canvas.texture.height * renderScale;
        renderRec.x = (GetScreenWidth() - renderRec.width) / 2.0f;
        renderRec.y = (GetScreenHeight() - renderRec.height) / 2.0f;
        DrawTexturePro(canvas.texture,
                       Rectangle{0, 0, (float) canvas.texture.width, (float) -canvas.texture.height},
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


    // Close window and OpenGL context
    CloseWindow();

    return EXIT_SUCCESS;
};
