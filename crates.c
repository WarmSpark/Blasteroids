#include "crates.h"
#include "raymath.h"
#include <math.h>

#define SCREEN_WIDTH 1280
#define SCREEN_HEIGHT 720

void InitCrates(Crate crates[]) {
    for (int i = 0; i < MAX_CRATES; i++) {
        crates[i].active = false;
    }
}

void SpawnCrate(Crate crates[], Vector2 position) {
    for (int i = 0; i < MAX_CRATES; i++) {
        if (!crates[i].active) {
            crates[i].active = true;
            crates[i].position = position;
            crates[i].lifetime = 12.0f;
            crates[i].type = (GetRandomValue(0, 1) == 0) ? CRATE_FUEL : CRATE_AMMO;
            float angle = (float)GetRandomValue(0, 360) * DEG2RAD;
            float speed = (float)GetRandomValue(15, 35);
            crates[i].velocity = (Vector2){sinf(angle) * speed, -cosf(angle) * speed};
            break;
        }
    }
}

void UpdateCrates(Crate crates[]) {
    float dt = GetFrameTime();
    for (int i = 0; i < MAX_CRATES; i++) {
        if (crates[i].active) {
            crates[i].lifetime -= dt;
            crates[i].position.x += crates[i].velocity.x * dt;
            crates[i].position.y += crates[i].velocity.y * dt;
            if (crates[i].position.x < 0)
                crates[i].position.x = SCREEN_WIDTH;
            if (crates[i].position.x > SCREEN_WIDTH)
                crates[i].position.x = 0;
            if (crates[i].position.y < 0)
                crates[i].position.y = SCREEN_HEIGHT;
            if (crates[i].position.y > SCREEN_HEIGHT)
                crates[i].position.y = 0;
            if (crates[i].lifetime <= 0.0f)
                crates[i].active = false;
        }
    }
}

void DrawCrates(const Crate crates[]) {
    for (int i = 0; i < MAX_CRATES; i++) {
        if (crates[i].active) {
            if (crates[i].lifetime < 3.0f && ((int)(crates[i].lifetime * 10)) % 2 != 0) {
                continue;
            }
            Color col = (crates[i].type == CRATE_FUEL) ? LIME : YELLOW;
            const char *label = (crates[i].type == CRATE_FUEL) ? "F" : "A";
            Rectangle rect = {crates[i].position.x - 8, crates[i].position.y - 8, 16, 16};
            DrawRectangleLinesEx(rect, 2.0f, col);
            DrawText(label, (int)crates[i].position.x - 4, (int)crates[i].position.y - 6, 12, col);
        }
    }
}

void CheckCratePickup(Crate crates[], Player *player) {
    for (int i = 0; i < MAX_CRATES; i++) {
        if (crates[i].active) {
            if (Vector2Distance(player->position, crates[i].position) < 22.0f) {
                if (crates[i].type == CRATE_FUEL) {
                    player->fuel += 35.0f;
                    if (player->fuel > player->maxFuel)
                        player->fuel = player->maxFuel;
                } else {
                    player->ammo += 20;
                    if (player->ammo > player->maxAmmo)
                        player->ammo = player->maxAmmo;
                }
                crates[i].active = false;
            }
        }
    }
}
