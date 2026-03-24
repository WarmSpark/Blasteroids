//
// Created by divyansh_1410 on 24/03/26.
//

#ifndef PARTICLE_H
#define PARTICLE_H
#include "raylib.h"
#include "stdbool.h"
#define MAX_PARTICLES 200
typedef struct {
        Vector2 position;
        Vector2 velocity;
        float lifetime;
        float maxLifetime;
        bool active;
}Particle;
void InitParticle(Particle particle[]);
void UpdateParticle(Particle particle[]);
void DrawParticle(Particle particle[]);
void SpawnExplosion(Particle particle[],Vector2 position,int count);

#endif //PARTICLE_H