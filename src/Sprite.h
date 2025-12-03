#pragma once
struct Sprite {
    Texture2D texture;
    Vector2 pos;
    float speed = 200;
    //Animation
    int frames;
    Rectangle size;
    int frameSpeed;
    int frameCount;
};