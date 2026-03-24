//
// Created by divyansh_1410 on 23/03/26.
//

#include "raylib.h"
#include "raymath.h"
#include "collision.h"

void CheckCollisions(Bullet bullets[],Astroid astroids[],Player *player,Particle particle[],Sound sound) {
    int index=0;
    for (int i=0;i<MAX_BULLETS;i++) {
        if (bullets[i].active) {
            for (int j=0;j<MAX_AESTROIDS;j++) {
                if (astroids[j].active) {
                    if (Vector2Distance(bullets[i].position,astroids[j].position)<astroids[j].radius) {
                        PlaySound(sound);
                        bullets[i].active=false;
                        player->score+=100;
                        index=j;
                        SpawnExplosion(particle,astroids[j].position,100);
                        SpiltAstroid(astroids,index);
                    }
                }
            }
        }
    }
    for (int k=0;k<MAX_AESTROIDS;k++) {
        if (astroids[k].active) {
            if (Vector2Distance(player->position,astroids[k].position)<astroids[k].radius+10) {
                SpawnExplosion(particle,astroids[k].position,40);
                PlaySound(sound);
                player->lives--;
                astroids[k].active=false;
                player->position = (Vector2){ 1280/2, 720/2 };
                player->velocity = (Vector2){ 0, 0 };
            }
        }
    }
}