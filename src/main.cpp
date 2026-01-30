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

int main() {
    Cooldown attackCD(0.5f);
    Cooldown dashCD(3.0f);
    // Raylib initialization
    // Project name, screen size, fullscreen mode etc. can be specified in the config.h.in file
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
        STATE_PAUSE = 4
    };

    MainMenu mainMenu;
    Options options;
    pauseMenu pauseMenu;
    GameState currentState = STATE_MENU;
    GameState previousState = STATE_PLAYING;
    Death_Screen deathScreen;
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
            currentState = STATE_PLAYING;
        }
        else if (pauseMenu.GetChoice() == 1) {
            pauseMenu.ResetChoice();
            previousState = STATE_PAUSE;
            currentState = STATE_OPTIONS;
        }
        else if (pauseMenu.GetChoice() == 2) {
            pauseMenu.ResetChoice();
            currentState = STATE_MENU;
        }
        break;

    case STATE_PLAYING:
        player.Update(dt, walls);
        golem.Update(dt);
        hp.Update(dt);
        attackCD.Update(dt);
        dashCD.Update(dt);
        player.Animate(dt);

        // Pause aktivieren
        if (IsKeyPressed(KEY_ESCAPE)) {
            currentState = STATE_PAUSE;
        }

        // ... Rest deiner Kampf-Logik (Dash, Melee etc.) ...
        if (dashCD.Ready() && IsKeyPressed(KEY_LEFT_SHIFT)) {
            player.Dash(walls, dt);
            hp.invincibleTimer = hp.invincibleDuration;
            dashCD.Trigger();
        }
        if (hp.Gethealth() <= 0) {
            currentState = STATE_DEATH;
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
            deathScreen.ResetChoice();
            currentState = STATE_PLAYING;
        }
        else if (deathScreen.GetChoice() == 1) {
            deathScreen.ResetChoice();
            mainMenu.ResetChoice();
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
            else if (currentState == STATE_PAUSE) {
                pauseMenu.Draw();
            }
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


            } else if (currentState == STATE_DEATH) {
                // Zeichne evtl. den Spieler/Boss noch (starr), damit es nicht leer aussieht
                player.Draw();
                golem.Draw();

                // Jetzt den roten Text drüber zeichnen
                deathScreen.Draw();
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

}

