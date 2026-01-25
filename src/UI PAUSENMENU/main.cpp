#include <cstdlib>
#include <algorithm>

#include "raylib.h"

#include "config.h"
#include <math.h>
#include "cooldown.h"
#include <vector>
#include "Sprite.h"
#include "boss/golem/boss.h"
#include "player/movement/controller.h"
#include "enviroment/walls.h"

#include "PauseMenu.h"

int main() {

    Cooldown attackCD(1.0f);
    Cooldown dashCD(5.0f);

    SetConfigFlags(FLAG_WINDOW_RESIZABLE | FLAG_VSYNC_HINT);
    InitWindow(Game::ScreenWidth, Game::ScreenHeight, Game::PROJECT_NAME);
    SetTargetFPS(60);

#ifdef GAME_START_FULLSCREEN
    ToggleFullscreen();
#endif

    Enemy golem;
    controller player;
    golem.Init();
    player.Init();

    PauseMenu pauseMenu;

    Sprite shot;
    shot.texture = LoadTexture("assets/graphics/shot.png");
    Rectangle srcH = {0,0,(float)shot.texture.width,(float)shot.texture.height};

    std::vector<Wall> walls = {
            { 0, 0, 1, (float)GetScreenHeight() },
            { (float)GetScreenWidth() - 1, 0, 1, (float)GetScreenHeight() },
            { { 0, 40, (float)GetScreenWidth(), 1 } },
            { { 0, (float)GetScreenHeight(), (float)GetScreenWidth(), 1 } },
    };

    Texture2D myTexture = LoadTexture("assets/graphics/testimage.png");
    RenderTexture2D canvas = LoadRenderTexture(Game::ScreenWidth, Game::ScreenHeight);

    float renderScale{};
    Rectangle renderRec{};

    while (!WindowShouldClose())
    {
        float dt = GetFrameTime();

        // -------------------------
        // Pause menu input
        // -------------------------
        pauseMenu.UpdateToggle();
        PauseAction act = pauseMenu.UpdateInput();

        if (act == PauseAction::Resume) {
            pauseMenu.isPaused = false;
        } else if (act == PauseAction::Restart) {
            // Re-init game objects (simple restart)
            pauseMenu.isPaused = false;
            golem.Unload();
            player.Unload();

            golem.Init();
            player.Init();

            attackCD = Cooldown(1.0f);
            dashCD   = Cooldown(5.0f);
        } else if (act == PauseAction::Quit) {
            break;
        }

        // -------------------------
        // Fullscreen toggle (still works)
        // -------------------------
        if (IsKeyDown(KEY_LEFT_ALT) && IsKeyPressed(KEY_ENTER)) {
            if (IsWindowFullscreen()) {
                ToggleFullscreen();
                SetWindowSize(Game::ScreenWidth, Game::ScreenHeight);
            } else {
                SetWindowSize(GetMonitorWidth(GetCurrentMonitor()), GetMonitorHeight(GetCurrentMonitor()));
                ToggleFullscreen();
            }
        }

        // -------------------------
        // Update gameplay ONLY when not paused
        // -------------------------
        if (!pauseMenu.isPaused) {
            player.Animate(dt);
            player.Update(dt, walls);
            player.Dash(walls);

            dashCD.Trigger();
            dashCD.Update(dt);

            golem.Update(dt);

            // Attack input only when playing
            if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT) && attackCD.Ready()) {
                attackCD.Trigger();
            }
            attackCD.Update(dt);
        }

        // -------------------------
        // Draw
        // -------------------------
        BeginDrawing();
        BeginTextureMode(canvas);
        {
            ClearBackground(WHITE);

            player.Draw();
            golem.Draw();

            if (dashCD.Ready())
                DrawText("Ready", 150, 20, 24, BLUE);
            else
                DrawText(TextFormat("Cooldown %.2f", dashCD.Remaining()), 150, 20, 24, BLUE);

            if (attackCD.Ready())
                DrawText("Ready", 20, 20, 24, GREEN);
            else
                DrawText(TextFormat("Cooldown %.2f", attackCD.Remaining()), 20, 20, 24, GREEN);

            // Draw pause menu INSIDE canvas (important)
            pauseMenu.Draw();
        }
        EndTextureMode();

        // Letterbox render to window
        ClearBackground(BLACK);

        renderScale = std::min(GetScreenHeight() / (float)canvas.texture.height,
                               GetScreenWidth()  / (float)canvas.texture.width);
        renderScale = floorf(renderScale);
        if (renderScale < 1) renderScale = 1;

        renderRec.width  = canvas.texture.width * renderScale;
        renderRec.height = canvas.texture.height * renderScale;
        renderRec.x = (GetScreenWidth() - renderRec.width) / 2.0f;
        renderRec.y = (GetScreenHeight() - renderRec.height) / 2.0f;

        DrawTexturePro(canvas.texture,
                       Rectangle{0, 0, (float)canvas.texture.width, (float)-canvas.texture.height},
                       renderRec,
                       {}, 0, WHITE);

        if (IsKeyDown(KEY_LEFT_CONTROL) && IsKeyDown(KEY_S)) {
            DrawText(TextFormat("Render scale: %.0f", renderScale), 10, 10, 20, LIGHTGRAY);
        }

        EndDrawing();
    }

    // De-init
    UnloadTexture(myTexture);
    UnloadTexture(shot.texture);
    UnloadRenderTexture(canvas);

    golem.Unload();
    player.Unload();

    CloseWindow();
    return EXIT_SUCCESS;
}