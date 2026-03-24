//
// Created by divyansh_1410 on 23/03/26.
//

#ifndef AESTROIDS_H
#define AESTROIDS_H
#include <raylib.h>
#include <stdbool.h>
#define MAX_AESTROIDS 30

typedef enum {
    LARGE,
    MEDIUM,
    SMALL
}AstroidSize;

typedef struct {
    Vector2 position;
    Vector2 velocity;
    Vector2 points[8];
    float radius;
    bool active;
    AstroidSize size;
}Astroid;
void InitAstroids(Astroid astroids[]);
void UpdateAstroids(Astroid astroid[]);
void DrawAstroids(Astroid astroids[]);
void SpawnAstroids(Astroid astroids[],Vector2 position,AstroidSize size);
void SpiltAstroid(Astroid astroids[],int index);




#endif //AESTROIDS_H