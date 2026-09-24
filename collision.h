#ifndef COLLISION_H
#define COLLISION_H

#include "player.h"
#include "aestroids.h"
#include "bullets.h"
#include "raylib.h"
#include "particle.h"
#include "crates.h"

void CheckCollisions(Bullet bullets[], Astroid astroids[], Player *player, Particle particle[], Crate crates[], Sound sound);

#endif