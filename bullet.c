//
// Created by divyansh_1410 on 23/03/26.
//
#include "bullets.h"
#include "raylib.h"
#include "math.h"
#define SCREEN_WIDTH 1280
#define SCREEN_HEIGHT 720
#define BULLET_SPEED 300.0f


void InitBullets(Bullet bullets[]) {
    for (int i=0;i<MAX_BULLETS;i++) {
        bullets[i].active=false;
    }
}

void FireBullet(Bullet bullets[],Vector2 pos,float rotation) {
    for (int i=0;i<MAX_BULLETS;i++) {
        if (!bullets[i].active) {
            float rad=rotation*DEG2RAD;
            bullets[i].position.x=pos.x+sinf(rad)*20;
            bullets[i].position.y=pos.y-cosf(rad)*20;
            bullets[i].velocity.x=sinf(rad)*BULLET_SPEED;
            bullets[i].velocity.y=-cosf(rad)*BULLET_SPEED;
            bullets[i].active=true;
            break;
        }
    }
}
void UpdateBullets(Bullet bullets[]) {
    for (int i=0;i<MAX_BULLETS;i++) {
        float dt=GetFrameTime();
        if (bullets[i].active) {
            bullets[i].position.x+=bullets[i].velocity.x*dt;
            bullets[i].position.y+=bullets[i].velocity.y*dt;
            if (bullets[i].position.x > SCREEN_WIDTH||
                bullets[i].position.x < 0||
                bullets[i].position.y > SCREEN_HEIGHT||
                bullets[i].position.y < 0)  bullets[i].active = false;
        }
    }

}

void DrawBullets(Bullet bullets[]) {
    for (int i=0;i<MAX_BULLETS;i++) {
        if (bullets[i].active) {
            DrawCircleV(bullets[i].position,3,RED);
        }
    }
}