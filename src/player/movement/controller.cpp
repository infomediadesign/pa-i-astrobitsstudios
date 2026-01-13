//
// Created by Ben on 13.01.2026.
#include "raylib.h"

#include "config.h"
//

#include "controller.h"



void controller::Update(float dt)
{
    if (IsKeyDown(KEY_W)) {
        pos.y -= speed;
        /*  if (collision3 == true) {
             ball.pos.y=ball.pos.y+ball.speed;
          }
          */
    }
    if (IsKeyDown(KEY_S)) {
        pos.y += speed;
        /* if (collision4 == true) {
            ball.pos.y=ball.pos.y-ball.speed;
         }*/
    }
    if (IsKeyDown(KEY_A)) {
       pos.x -= speed;
        /* if (collision1 == true) {
             ball.pos.x=ball.pos.x+ball.speed;
         }*/

    }
    if (IsKeyDown(KEY_D)) {
        pos.x += speed;
        /* if (collision2 == true) {
             ball.pos.x=ball.pos.x-ball.speed;
         }*/
    }
}
void controller::Init()
{
    texture =LoadTexture("assets/graphics/player.png");
    pos = {10, 100};
    speed = 7;
    frames=0;
    frameCount=0;
    frameSpeed = 8;
    size={0.0f,0.0f, (float)texture.width,(float)texture.height};
}
void controller::Draw()
{
    if (IsKeyDown(KEY_A))
    {
        size ={size.x,0.0f, (float)-texture.width/8,(float)texture.height/2};
    }else size ={size.x,0.0f, (float)texture.width/8,(float)texture.height/2};

    DrawTextureRec(texture, size, pos, WHITE);
}
void controller::Animate(float dt)
{
    frameCount++;
    if (frameCount >=(60/frameSpeed)) {
        frameCount =0;
        frames++;
        if (frames>8) frames=0;
        size.x = (float) frames*(float)texture.width/8;
    }

}
void controller::Unload()
{
    UnloadTexture(texture);
}
void controller::Dash(float dt) {
    if (IsKeyPressed(KEY_Q) ) {
        if (IsKeyDown(KEY_D))
            pos.x = pos.x+texture.width/8 + 500;
        //if (pos.x > wallright.x)
          //  pos.x=wallright.x - texture.width/8;
        if (IsKeyDown(KEY_A))
            pos.x = pos.x-texture.width/8 - 500;
       // if (pos.x < wallleft.x)
         //   pos.x=wallleft.x;
        if (IsKeyDown(KEY_W))
            pos.y =  pos.y - texture.height - 500;
        //if (pos.y < wallup.y)
          //  pos.y=wallup.y;
        if (IsKeyDown(KEY_S))
            pos.y = pos.y + texture.height/2 + 500;
       // if (pos.y > walldown.y)
         //   pos.y=walldown.y - texture.height/2;
    }
}