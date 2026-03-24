//
// Created by divyansh_1410 on 23/03/26.
//
#include <math.h>

#include "raylib.h"
#include "aestroids.h"
#define SCREEN_WIDTH 1280
#define SCREEN_HEIGHT 720

void InitAstroids(Astroid astroids[])
{
    for (int i=0;i<MAX_AESTROIDS;i++) {
        astroids[i].active = false;
    }

}
void SpawnAstroids(Astroid astroids[],Vector2 position,AstroidSize size)
{
    for (int i=0;i<MAX_AESTROIDS;i++) {
        if (!astroids[i].active) {
            astroids[i].position=position;
            astroids[i].size=size;
            if (size==LARGE)astroids[i].radius=60.0f;
            if (size==MEDIUM)astroids[i].radius=30.0f;
            if (size==SMALL)astroids[i].radius=15.0f;
            float angle=GetRandomValue(5,175)*DEG2RAD;
            float speed=GetRandomValue(45,200);
            astroids[i].velocity.x=sinf(angle)*speed;
            astroids[i].velocity.y=-cosf(angle)*speed;
            astroids[i].active=true;
            break;
        }
    }
}
void UpdateAstroids(Astroid astroid[])
{
    for (int i=0;i<MAX_AESTROIDS;i++) {
        float dt=GetFrameTime();
        if (astroid[i].active) {
            astroid[i].position.x+=astroid[i].velocity.x*dt;
            astroid[i].position.y+=astroid[i].velocity.y*dt;
            if (astroid[i].position.x > SCREEN_WIDTH)  astroid[i].position.x = 0;
            if (astroid[i].position.x < 0)             astroid[i].position.x = SCREEN_WIDTH;
            if (astroid[i].position.y > SCREEN_HEIGHT) astroid[i].position.y = 0;
            if (astroid[i].position.y < 0)             astroid[i].position.y = SCREEN_HEIGHT;
        }
    }
}
void DrawAstroids(Astroid astroids[])
{
    for (int i=0;i<MAX_AESTROIDS;i++) {
        if (astroids[i].active) {
            DrawCircleLinesV(astroids[i].position,astroids[i].radius,BLUE);
        }
    }
}

void SpiltAstroid(Astroid astroids[],int index)
{
    if (astroids[index].size==LARGE) {
        SpawnAstroids(astroids,astroids[index].position,MEDIUM);
        SpawnAstroids(astroids,astroids[index].position,MEDIUM);

    }
    if (astroids[index].size==MEDIUM) {
        SpawnAstroids(astroids,astroids[index].position,SMALL);
        SpawnAstroids(astroids,astroids[index].position,SMALL);

    }


    astroids[index].active=false;

}