#ifndef CRATES_H
#define CRATES_H

#include "player.h"
#include "raylib.h"
#include <stdbool.h>

#define MAX_CRATES 20

typedef enum {
    CRATE_FUEL,
    CRATE_AMMO
} CrateType;

typedef struct {
    Vector2 velocity;
    Vector2 position;
    CrateType type;
    float lifetime;
    bool active;
} Crate;

void InitCrates(Crate crates[]);
void SpawnCrate(Crate crates[], Vector2 position);
void UpdateCrates(Crate crates[]);
void DrawCrates(const Crate crates[]);
void CheckCratePickup(Crate crates[], Player *player);

#endif
