#include <cstdlib>
#include <algorithm>

#include "raylib.h"

#include "config.h"
#include <math.h>
#include"cooldown.h"
#include <vector>
#include"Sprite.h"
#include "boss/golem/boss.h"
#include "player/movement/controller.h"

int main() {

    Cooldown attackCD (1.0f);
    Cooldown dashCD (5.0f);
    /*
    GetMonitorWidth(0);
    GetMonitorHeight(0);
    SetConfigFlags(FLAG_WINDOW_UNDECORATED);
    InitWindow(GetScreenWidth(), GetScreenHeight(), "Borderless Fullscreen");
    */
    // Raylib initialization
    // Project name, screen size, fullscreen mode etc. can be specified in the config.h.in file
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

    Sprite shot;
    shot.texture = LoadTexture("assets/graphics/shot.png");


    Rectangle srcH = {0,0,(float)shot.texture.width,(float)shot.texture.height};
    Rectangle wallleft = {0,0,(float) 1,(float)GetScreenHeight()};
    Rectangle wallright = {  (float) GetScreenWidth(),  0,  (float) 1, (float)GetScreenHeight() };
    Rectangle wallup = {0,40,(float) GetScreenWidth(),(float) 1};
    Rectangle walldown = {0,(float) GetScreenHeight(),(float) GetScreenWidth(),(float) 1};
    // Your own initialization code here
    // ...
    // ...
    Texture2D myTexture = LoadTexture("assets/graphics/testimage.png");
    RenderTexture2D canvas = LoadRenderTexture(Game::ScreenWidth, Game::ScreenHeight);
    float renderScale{}; // this and the line below are relevant to drawing later.
    Rectangle renderRec{};


    // Main game loop
    while (!WindowShouldClose()) // Detect window close button or ESC key
    {
       player.Animate(GetFrameTime());


       /* Rectangle player = {ball.pos.x, ball.pos.y, (float)ball.texture.width/9, (float)ball.texture.height/2};


        bool collision1 = CheckCollisionRecs( player,  wallleft);
        bool collision2 = CheckCollisionRecs( player,  wallright);
        bool collision3 = CheckCollisionRecs( player,  wallup);
        bool collision4 = CheckCollisionRecs( player,  walldown);
*/
        player.Update(GetFrameTime());
        player.Dash(renderScale);
        dashCD.Trigger();
        dashCD.Update(GetFrameTime());
       /* if (faceseast==true) {shot.pos = {ball.pos.x+ball.texture.width, ball.pos.y};}
        if (facessouth==true) {shot.pos = {ball.pos.x, ball.pos.y+ball.texture.height};}
        if (faceswest==true) {shot.pos = {ball.pos.x-ball.texture.width-ball.speed, ball.pos.y};}
        if (facesnorth==true) {shot.pos = {ball.pos.x, ball.pos.y-ball.texture.height-ball.speed};}
        */

        Rectangle dstH = {shot.pos.x+shot.texture.width / 2.0f,shot.pos.y+shot.texture.height/2.0f,(float)shot.texture.width,(float)shot.texture.height};



        if (IsKeyDown(KEY_LEFT_ALT) && IsKeyPressed(KEY_ENTER)) { //Fullscreen logic.
            if (IsWindowFullscreen()) {
                ToggleFullscreen();
                SetWindowSize(Game::ScreenWidth,Game::ScreenHeight);
            } else {
                SetWindowSize(GetMonitorWidth(GetCurrentMonitor()), GetMonitorHeight(GetCurrentMonitor()));
                ToggleFullscreen();
            }
        }
        // Updates that are made by frame are coded here
        // This is where YOUR logic code should go
        // ...
        // ...
        golem.Update(GetFrameTime());
        BeginDrawing();
        // You can draw on the screen between BeginDrawing() and EndDrawing()
        // For the letterbox we draw on canvas instead
        BeginTextureMode(canvas);
        { //Within this block is where we draw our app to the canvas and YOUR code goes.
            ClearBackground(WHITE);

             player.Draw();

             if (dashCD.Ready())
                 DrawText("Ready", 150,20,24,BLUE);
             else DrawText(TextFormat("Cooldown %.2f",dashCD.Remaining()), 150,20,24,BLUE);

             if (attackCD.Ready())
                 DrawText("Ready", 20,20,24,GREEN);
             else DrawText(TextFormat("Cooldown %.2f",attackCD.Remaining()), 20,20,24,GREEN);

             //DrawTextureV(ball.texture, ball.pos, WHITE);

             if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT) && attackCD.Ready()) {

                // DrawTexturePro(shot.texture, srcH, dstH ,{(float)shot.texture.width /2,(float)shot.texture.height/2}, rotation, WHITE );
                 attackCD.Trigger();

             } attackCD.Update(GetFrameTime());
             golem.Draw();

        }
        EndTextureMode();
        //The following lines put the canvas in the middle of the window and have the negative as black
        ClearBackground(BLACK); // If you want something else than a black void in the background
                                // then you can add stuff here.


        renderScale = std::min(GetScreenHeight() / (float) canvas.texture.height, // Calculates how big or small the canvas has to be rendered.
                               GetScreenWidth()  / (float) canvas.texture.width); // Priority is given to the smaller side.
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

    // De-initialization here
    // ...
    // ...
    UnloadTexture(shot.texture);
    golem.Unload();
    player.Unload();

    // Close window and OpenGL context
    CloseWindow();

    return EXIT_SUCCESS;
}



