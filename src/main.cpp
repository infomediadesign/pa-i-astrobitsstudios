#include <cstdlib>
#include <algorithm>

#include "raylib.h"

#include "config.h"
#include <math.h>
#include"cooldown.h"
#include <vector>
#include"Sprite.h"
#include "boss.h"
int main() {
    bool facessouth;
    bool facesnorth;
    bool faceswest;
    bool faceseast=true;
    bool flip;
    float rotation;
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
    golem.Init();
    std::vector<Sprite> balls;
    Sprite ball;
    // Your own initialization code here
    ball.texture = LoadTexture("assets/graphics/player.png");
    ball.pos = {10, 100};
    ball.speed = 7;
    ball.frames=0;
    ball.frameCount=0;
    ball.frameSpeed = 8;
    ball.size={0.0f,0.0f, (float)ball.texture.width,(float)ball.texture.height};

    Sprite shot;
    shot.texture = LoadTexture("assets/graphics/shot.png");


    Rectangle srcH = {0,0,(float)shot.texture.width,(float)shot.texture.height};
    Rectangle wallleft = {0,0,(float) 1,(float)GetScreenHeight()};
    Rectangle wallright = { .x= (float) GetScreenWidth(), .y= 1, .width= (float) 1, (float)GetScreenHeight() };
    Rectangle wallup = {0,0,(float) GetScreenWidth(),(float) 1};
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
         ball.frameCount++;
        if (ball.frameCount >=(60/ball.frameSpeed)) {
            ball.frameCount =0;
            ball.frames++;
            if (ball.frames>8) ball.frames=0;
            ball.size.x = (float) ball.frames*(float)ball.texture.width/8;
        }
        /*
        if (IsKeyDown(KEY_RIGHT)) ball.frameSpeed ++;
        else if (IsKeyDown(KEY_LEFT)) ball.frameSpeed --;
        if (ball.frameSpeed>MAX_FRAME_SPEED) ball.frameSpeed = MAX_FRAME_SPEED;
        else if (ball.frameSpeed<MIN_FRAME_SPEED) ball.frameSpeed = MIN_FRAME_SPEED;
        */

        Rectangle player = {ball.pos.x, ball.pos.y, (float)ball.texture.width/9, (float)ball.texture.height/2};


        bool collision1 = CheckCollisionRecs( player,  wallleft);
        bool collision2 = CheckCollisionRecs( player,  wallright);
        bool collision3 = CheckCollisionRecs( player,  wallup);
        bool collision4 = CheckCollisionRecs( player,  walldown);

        if (IsKeyDown(KEY_W)) {
            ball.pos.y -= ball.speed;
            faceswest=false;
            faceseast=false;
            facessouth=false;
            facesnorth=true;
            rotation=270;
            if (collision3 == true) {
                ball.pos.y=ball.pos.y+ball.speed;
            }
        }
        if (IsKeyDown(KEY_S)) {
            ball.pos.y += ball.speed;
            faceswest=false;
            faceseast=false;
            facesnorth=false;
            facessouth=true;
            rotation=90;
            if (collision4 == true) {
                ball.pos.y=ball.pos.y-ball.speed;
            }
        }
        if (IsKeyDown(KEY_A)) {
            ball.pos.x -= ball.speed;
            faceseast=false;
            facesnorth=false;
            facessouth=false;
            faceswest=true;
            flip=true;
            rotation=180;
            if (collision1 == true) {
                ball.pos.x=ball.pos.x+ball.speed;
            }

        }
        if (IsKeyDown(KEY_D)) {
            ball.pos.x += ball.speed;
            faceswest=false;
            facesnorth=false;
            facessouth=false;
            faceseast=true;
            flip=false;
            rotation=0;
            if (collision2 == true) {
                ball.pos.x=ball.pos.x-ball.speed;
            }
        }
        if (IsKeyPressed(KEY_Q) && dashCD.Ready()) {
            if (faceseast==true)
                ball.pos.x = ball.pos.x+ball.texture.width/8 + 500;
            if (ball.pos.x > wallright.x)
                ball.pos.x=wallright.x - ball.texture.width/8;
            if (faceswest==true)
                ball.pos.x = ball.pos.x-ball.texture.width/8 - 500;
            if (ball.pos.x < wallleft.x)
                ball.pos.x=wallleft.x;
            if (facesnorth==true)
                ball.pos.y =  ball.pos.y - ball.texture.height - 500;
            if (ball.pos.y < wallup.y)
                ball.pos.y=wallup.y;
            if (facessouth==true)
                ball.pos.y = ball.pos.y + ball.texture.height/2 + 500;
            if (ball.pos.y > walldown.y)
                ball.pos.y=walldown.y - ball.texture.height/2;
            dashCD.Trigger();
        } dashCD.Update(GetFrameTime());
        if (faceseast==true) {shot.pos = {ball.pos.x+ball.texture.width, ball.pos.y};}
        if (facessouth==true) {shot.pos = {ball.pos.x, ball.pos.y+ball.texture.height};}
        if (faceswest==true) {shot.pos = {ball.pos.x-ball.texture.width-ball.speed, ball.pos.y};}
        if (facesnorth==true) {shot.pos = {ball.pos.x, ball.pos.y-ball.texture.height-ball.speed};}


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
             if (flip==true) {
                 ball.size ={ball.size.x,0.0f, (float)-ball.texture.width/8,(float)ball.texture.height/2};
             }else ball.size ={ball.size.x,0.0f, (float)ball.texture.width/8,(float)ball.texture.height/2};

             DrawTextureRec(ball.texture, ball.size, ball.pos, WHITE);  // Draw part of the texture


             if (dashCD.Ready())
                 DrawText("Ready", 150,20,24,BLUE);
             else DrawText(TextFormat("Cooldown %.2f",dashCD.Remaining()), 150,20,24,BLUE);

             if (attackCD.Ready())
                 DrawText("Ready", 20,20,24,GREEN);
             else DrawText(TextFormat("Cooldown %.2f",attackCD.Remaining()), 20,20,24,GREEN);

             //DrawTextureV(ball.texture, ball.pos, WHITE);

             if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT) && attackCD.Ready()) {

                 DrawTexturePro(shot.texture, srcH, dstH ,{(float)shot.texture.width /2,(float)shot.texture.height/2}, rotation, WHITE );
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
    UnloadTexture(ball.texture);
    UnloadTexture(shot.texture);
    golem.Unload();

    // Close window and OpenGL context
    CloseWindow();

    return EXIT_SUCCESS;
}
