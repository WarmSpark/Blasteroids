#ifndef STARFIELD_H
#define STARFIELD_H

#include "raylib.h"

#define MAX_STARS 120

typedef struct {
    Vector2 position;
    float speed;
    float size;
    Color color;
} Star;

void InitStarfield(Star stars[]);
void UpdateStarfield(Star stars[], Vector2 playerVelocity);
void DrawStarfield(const Star stars[]);

#endif
