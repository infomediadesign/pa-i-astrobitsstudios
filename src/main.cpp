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
    MainMenu mainMenu;
    Options options;
    GameState currentState = MENU;
    Player hp;
    Enemy golem;
    controller player;
    plattack melee = plattack();
    melee.Init();
    golem.Init();
    player.Init();
    hp.Init();
    std::vector<Wall> walls = {
        {0, 0, 1, (float) GetScreenHeight()},
        {(float) GetScreenWidth() - 1, 0, 1, (float) GetScreenHeight()},
        {{0, 40, (float) GetScreenWidth(), 1}},
        {{0, (float) GetScreenHeight(), (float) GetScreenWidth(), (float) 1}},

    };


    // Your own initialization code here
    // ...
    // ...
    RenderTexture2D canvas = LoadRenderTexture(Game::ScreenWidth, Game::ScreenHeight);
    float renderScale{}; // this and the line below are relevant to drawing later.
    Rectangle renderRec{};


    // Main game loop
    while (!WindowShouldClose() && currentState != EXIT) {

        float dt = GetFrameTime();

        // --- 1. LOGIK UPDATE ---
                if (currentState == MENU) {
                    mainMenu.Update();
                    if (mainMenu.GetChoice() == 0) {
                        hp.Init();
                        melee.Reset();
                        player.Reset();
                        currentState = PLAYING;
                        mainMenu.ResetChoice();
                    }
                    else if (mainMenu.GetChoice() == 2) {
                        currentState = EXIT;
                    }else if (mainMenu.GetChoice()==1) {
                        currentState = OPTIONS;
                        mainMenu.ResetChoice();
                    }
                }
        if (IsKeyPressed(KEY_ESCAPE)&& currentState == OPTIONS) {
            currentState = MENU;
        }

          if (currentState == PLAYING) {
                player.Update(dt, walls);
                golem.Update(dt);
                hp.Update(dt);
                attackCD.Update(dt);
                dashCD.Update(dt);
                player.Animate(dt);

                if (dashCD.Ready() && IsKeyPressed(KEY_LEFT_SHIFT)) {
                    player.Dash(walls, dt);
                    hp.invincibleTimer = hp.invincibleDuration;
                    hp.Update(dt);
                    dashCD.Trigger();
                }

                if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT) && attackCD.Ready()) {
                    melee.Start(player.GetPos(), player.GetSize());
                    attackCD.Trigger();
                }
                melee.Update(dt, player.GetPos(), player.GetSize());

                if (melee.active) {
                    if (CheckCollisionRecs(melee.dstH, golem.GetRect())) {
                        if (!golem.wasHit) golem.TakeDamage(melee.damage);
                    }
                } else golem.wasHit = false;

              if (IsKeyPressed(KEY_ESCAPE)) currentState = MENU;

        }


        if (IsKeyDown(KEY_LEFT_ALT) && IsKeyPressed(KEY_ENTER)) {
            //Fullscreen logic.
            if (IsWindowFullscreen()) {
                ToggleFullscreen();
                SetWindowSize(Game::ScreenWidth, Game::ScreenHeight);
            } else {
                SetWindowSize(GetMonitorWidth(GetCurrentMonitor()), GetMonitorHeight(GetCurrentMonitor()));
                ToggleFullscreen();
            }
        }


        BeginDrawing();
        // You can draw on the screen between BeginDrawing() and EndDrawing()
        // For the letterbox we draw on canvas instead
        BeginTextureMode(canvas);
        {
            //Within this block is where we draw our app to the canvas and YOUR code goes.
            ClearBackground(WHITE);

            //Logik-Weiche (Draw) ===
            if (currentState == OPTIONS) {
                options.Draw();
            }
            if (currentState == MENU) {
                mainMenu.Draw();
            } else if (currentState == PLAYING) {
                player.Draw();
                golem.Draw();
                if (melee.active)
                    melee.Draw();
                if (dashCD.Ready())
                    DrawText("Ready", 150, 20, 24,BLUE);
                else DrawText(TextFormat("Cooldown %.2f", dashCD.Remaining()), 150, 20, 24,BLUE);

                if (attackCD.Ready())
                    DrawText("Ready", 20, 20, 24,GREEN);
                else DrawText(TextFormat("Cooldown %.2f", attackCD.Remaining()), 20, 20, 24,GREEN);

                if (CheckCollisionRecs(player.GetCollision(), golem.GetRect()) && golem.active) {
                    hp.TakeDamage(10);
                    DrawText("Hit", 400, 100, 24,BLACK);
                }
                hp.Draw(player.GetCollision());
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
        } // Main game loop end
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
