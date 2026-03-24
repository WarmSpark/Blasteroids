#include "raylib.h"
#include "player.h"
#include "bullets.h"
#include "aestroids.h"
#include "collision.h"
#define SCREEN_WIDTH 1280
#define SCREEN_HEIGHT 720
#define FPS 60
bool alldead(Astroid astroids[]) {
    for (int i=0;i<MAX_AESTROIDS;i++) {
        if (astroids[i].active) {
            return false;
        }
    }
    return true;
}
void Respawn(Astroid astroids[]) {
    if (alldead(astroids)) {
        for (int i = 0; i < 5; i++) {
            int edge = GetRandomValue(0, 3);
            Vector2 pos;
            if (edge == 0) pos = (Vector2){ GetRandomValue(0, SCREEN_WIDTH), 0 };
            if (edge == 1) pos = (Vector2){ GetRandomValue(0, SCREEN_WIDTH), SCREEN_HEIGHT };
            if (edge == 2) pos = (Vector2){ 0, GetRandomValue(0, SCREEN_HEIGHT) };
            if (edge == 3) pos = (Vector2){ SCREEN_WIDTH, GetRandomValue(0, SCREEN_HEIGHT) };
            SpawnAstroids(astroids, pos, LARGE);
        }
    }
}
int main(void) {
    InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "Blasteroids");
    SetTargetFPS(FPS);
    Astroid astroids[MAX_AESTROIDS];
    Player player = InitPlayer();
    Bullet bullets[MAX_BULLETS];

    InitBullets(bullets);
    InitAstroids(astroids);
    Respawn(astroids);

    while (!WindowShouldClose()) {
        UpdatePlayer(&player);
        UpdateBullets(bullets);
        UpdateAstroids(astroids);
        if (IsKeyPressed(KEY_SPACE)) {
            FireBullet(bullets,player.position,player.rotation);
        }
        Respawn(astroids);
        CheckCollisions(bullets,astroids,&player);
        BeginDrawing();
        ClearBackground(BLACK);
        DrawPlayer(&player);
        DrawBullets(bullets);
        DrawAstroids(astroids);
        DrawText(TextFormat("Score: %d", player.score), 10, 10, 20, WHITE);
        DrawText(TextFormat("Lives: %d", player.lives), 10, 35, 20, WHITE);
        EndDrawing();
    }

    CloseWindow();
    return 0;
}