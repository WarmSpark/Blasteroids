//
// Created by divyansh_1410 on 23/03/26.
//

#ifndef COLLISION_H
#define COLLISION_H
#include "player.h"
#include "aestroids.h"
#include "bullets.h"
#include "raylib.h"

void CheckCollisions(Bullet bullets[],Astroid astroids[],Player *player);
#endif //COLLISION_H