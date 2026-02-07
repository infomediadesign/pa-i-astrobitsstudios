#include <cstdlib>
#include <algorithm>
#include "raylib.h"
#include "config.h"
#include <math.h>
#include"cooldown.h"
#include <vector>
#include "Sprite.h"
#include "boss/golem/boss.h"
#include "player/movement/controller.h"
#include "enviroment/walls.h"
#include "player/combat/plattack.h"
#include "player/schaden/schaden.h"
#include "Menu/MainMenu_UI.h"
#include "Menu/Options.h"
#include "Menu/Death_Screen.h"
#include "Menu/pauseMenu.h"
#include "Menu/highscore.h"

int main() {
    Cooldown attackCD(0.5f);
    Cooldown dashCD(3.0f);
    // Raylib initialization
    // Project name, screen size, fullscreen mode etc. can be specified in the config.h file
    SetConfigFlags(FLAG_WINDOW_RESIZABLE | FLAG_VSYNC_HINT| FLAG_WINDOW_UNDECORATED);
    InitWindow(GetMonitorWidth(0), GetMonitorHeight(0), Game::PROJECT_NAME);
    SetExitKey(KEY_NULL);
    SetTargetFPS(60);

#ifdef GAME_START_FULLSCREEN
    ToggleFullscreen();
#endif
    enum GameState {
        STATE_MENU = -1,
        STATE_PLAYING = 0,
        STATE_OPTIONS = 1,
        STATE_EXIT = 2,
        STATE_DEATH = 3,
        STATE_PAUSE = 4,
        STATE_VICTORY = 5,
        STATE_NAME_ENTRY = 6,
        STATE_HIGHSCORES = 7
    };

    MainMenu mainMenu;
    Options options;
    pauseMenu pauseMenu;
    GameState currentState = STATE_MENU;
    GameState previousState = STATE_PLAYING;
    Death_Screen deathScreen;
    RunTimer runTimer;
    HighscoreBoard board;
    NameInput nameInput;
    const std::string SCORE_FILE = "highscores.csv";
    board.Load(SCORE_FILE);
    Player hp;
    Enemy golem;
    controller player;
    plattack melee;
    melee.Init();
    golem.Init();
    player.Init();
    hp.Init();
    std::vector<Wall> walls = {
        {0, 0, 1, (float)Game::ScreenHeight},                               // Links
        {(float)Game::ScreenWidth - 5, 0, 1, (float)Game::ScreenHeight},    // Rechts
        {0, 40, (float)Game::ScreenWidth, 5},                               // Oben
        {0, (float)Game::ScreenHeight - 50, (float)Game::ScreenWidth, 1}     // Unten
    };

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
            currentState = STATE_PLAYING;
        }
        else if (pauseMenu.GetChoice() == 1) {
            pauseMenu.ResetChoice();
            previousState = STATE_PAUSE;
            currentState = STATE_OPTIONS;
        }
        else if (pauseMenu.GetChoice() == 2) {
            pauseMenu.ResetChoice();
            runTimer.Reset();
            currentState = STATE_MENU;
        }
        break;

    case STATE_PLAYING:
        runTimer.Update(dt);
        player.Update(dt, walls);
        golem.Update(dt);
        hp.Update(dt);
        attackCD.Update(dt);
        dashCD.Update(dt);
        melee.Update(dt,player.GetPos(), player.GetSize());
        player.Animate(dt);

        // Pause aktivieren
        if (IsKeyPressed(KEY_ESCAPE)) {
            runTimer.Stop();
            currentState = STATE_PAUSE;
        }

        // ... Rest deiner Kampf-Logik (Dash, Melee etc.) ...
        if (dashCD.Ready() && IsKeyPressed(KEY_LEFT_SHIFT)) {
            player.Dash(walls, dt);
            hp.invincibleTimer = hp.invincibleDuration;
            dashCD.Trigger();
        }
        if (attackCD.Ready() && IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
            melee.Start(player.GetPos(), player.GetSize());
            attackCD.Trigger();
            if (CheckCollisionRecs(melee.dstH, golem.GetRect())) {
                golem.TakeDamage(melee.damage);
            }
        }

        if (hp.Gethealth() <= 0) {
            runTimer.Reset();
            currentState = STATE_DEATH;
        }
        if (!golem.active) {
            runTimer.Stop();
            nameInput.Clear();
            currentState = STATE_NAME_ENTRY;
        }
        break;

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
            runTimer.Reset();
            runTimer.Start();
            currentState = STATE_PLAYING;
            mainMenu.ResetChoice();
        }
        else if (mainMenu.GetChoice() == 1) {
            previousState = STATE_MENU;
            currentState = STATE_OPTIONS;
            mainMenu.ResetChoice();
        }
        else if (mainMenu.GetChoice() == 2) {
            currentState = STATE_EXIT;
        }
        break;

    case STATE_DEATH:
        deathScreen.Update();
        if (deathScreen.GetChoice() == 0) {
            hp.Init();
            player.Reset();
            melee.Reset();
            hp.invincibleTimer = hp.invincibleDuration;
            runTimer.Reset();
            runTimer.Start();

            deathScreen.ResetChoice();
            currentState = STATE_PLAYING;
        }
        else if (deathScreen.GetChoice() == 1) {
            deathScreen.ResetChoice();
            mainMenu.ResetChoice();
            runTimer.Reset();
            currentState = STATE_MENU;
        }
        break;

    case STATE_NAME_ENTRY:
        nameInput.Update();
        if (IsKeyPressed(KEY_ENTER)) {
            std::string finalName = nameInput.text.empty() ? "Player" : nameInput.text;
            HighscoreEntry e{ finalName, runTimer.elapsedMs };
            board.AddAndPersist(SCORE_FILE, e, 10);     // ✅通关才保存
            currentState = STATE_HIGHSCORES;
        }
        // 可选：ESC 放弃保存（这会变成“通关但不写榜”）
        if (IsKeyPressed(KEY_ESCAPE)) {
            runTimer.Reset();
            currentState = STATE_MENU;
        }
        break;

    case STATE_HIGHSCORES:
        if (IsKeyPressed(KEY_ESCAPE) || IsKeyPressed(KEY_ENTER)) {
            currentState = STATE_MENU;
        }
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
            }
            /*else if (currentState == STATE_PAUSE) {
                pauseMenu.Draw();
            }*/
            else if (currentState == STATE_PLAYING) {
                player.Draw();
                golem.Draw();
                if (melee.active)
                    melee.Draw();
                if (dashCD.Ready())
                    DrawText("Ready", 150, 20, 24, BLUE);
                else DrawText(TextFormat("Cooldown %.2f", dashCD.Remaining()), 150, 20, 24, BLUE);

                if (attackCD.Ready())
                    DrawText("Ready", 20, 20, 24, GREEN);
                else DrawText(TextFormat("Cooldown %.2f", attackCD.Remaining()), 20, 20, 24, GREEN);

                if (CheckCollisionRecs(player.GetCollision(), golem.GetRect()) && golem.active) {
                    hp.TakeDamage(10);
                    DrawText("Hit", 400, 100, 24, BLACK);
                }
                hp.Draw(player.GetCollision());
                DrawText(("Time: " + RunTimer::FormatMinSecMs(runTimer.elapsedMs)).c_str(),
                 20, 60, 24, BLACK);


            } else if (currentState == STATE_DEATH || currentState == STATE_PAUSE) {
                // Zeichne evtl. den Spieler/Boss noch (starr), damit es nicht leer aussieht
                player.Draw();
                golem.Draw();

                // Jetzt den roten Text drüber zeichnen
                if (currentState == STATE_PAUSE) {
                    pauseMenu.Draw();
                }else if (currentState == STATE_DEATH) {
                    deathScreen.Draw();
                }
            }

            else if (currentState == STATE_NAME_ENTRY) {
                ClearBackground(BLACK);
                DrawText("VICTORY!", 60, 60, 50, GREEN);
                DrawText(("Your time: " + RunTimer::FormatMinSecMs(runTimer.elapsedMs)).c_str(),
                         60, 130, 28, RAYWHITE);
                nameInput.Draw(60, 200);
            }
            else if (currentState == STATE_HIGHSCORES) {
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

