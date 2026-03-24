
#ifndef BULLETS_H
#define BULLETS_H
#include "raylib.h"
#include <stdbool.h>
#define MAX_BULLETS 1000
typedef struct {
    Vector2 position;
    Vector2 velocity;
    bool active;
} Bullet;

void InitBullets(Bullet bullets[]);
void UpdateBullets(Bullet bullets[]);
void DrawBullets(Bullet bullets[]);
void FireBullet(Bullet bullets[], Vector2 position,float rotation);
#endif //BULLETS_H