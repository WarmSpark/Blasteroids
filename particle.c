//
// Created by divyansh_1410 on 24/03/26.
//
#include "particle.h"
#include "raylib.h"
#include <math.h>

#define PARTICLE_SPEED 150.0f
#define PARTICLE_LIFETIME 0.8f


void InitParticle(Particle particle[]) {
    for (int i = 0; i < MAX_PARTICLES; i++) {
        particle[i].active = false;
    }
}

void SpawnExplosion(Particle particle[],Vector2 position,int count) {
    int spawned = 0;
    for (int i = 0; i < MAX_PARTICLES; i++) {
        if (!particle[i].active) {
            spawned++;
            particle[i].active = true;
            float angle=GetRandomValue(0,360)*DEG2RAD;
            float speed=GetRandomValue(50,PARTICLE_SPEED);
            particle[i].velocity.x=sinf(angle)*speed;
            particle[i].velocity.y=-cosf(angle)*speed;
            particle[i].position=position;
            particle[i].lifetime=PARTICLE_LIFETIME;
            particle[i].maxLifetime=PARTICLE_LIFETIME;
            if (spawned == count) break;
        }
    }
}

void UpdateParticle(Particle particle[]) {

    for (int i = 0; i < MAX_PARTICLES; i++) {
        float dt=GetFrameTime();
        if (particle[i].active) {
            particle[i].lifetime-=dt;
            particle[i].position.x+=particle[i].velocity.x*dt;
            particle[i].position.y+=particle[i].velocity.y*dt;
            if (particle[i].lifetime<=0) particle[i].active=false;
        }
    }
}
void DrawParticle(Particle particle[]) {
    for (int i = 0; i < MAX_PARTICLES; i++) {
        if (particle[i].active) {
            float alpha =(particle[i].lifetime/particle[i].maxLifetime);
            DrawCircleV(particle[i].position,2,Fade(BLUE,alpha));
        }
    }
}
