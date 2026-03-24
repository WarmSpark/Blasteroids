#include "raylib.h"
#include "player.h"
#include "bullets.h"
#include "aestroids.h"
#include "collision.h"
#include "particle.h"
#include <stddef.h>
#include <stdio.h>
#define SCREEN_WIDTH 1280
#define SCREEN_HEIGHT 720
#define FPS 60

typedef enum {
    PLAYING,
    GAME_OVER,
}GameState;
GameState state = PLAYING;
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
    InitAudioDevice();
    Sound shoot = LoadSound("Sounds/laser.wav");
    if (shoot.stream.buffer == NULL) printf("shoot sound not loaded!\n");

    Sound engine = LoadSound("Sounds/thrust.wav");
    if (engine.stream.buffer == NULL) printf("engine sound not loaded!\n");

    Sound explode = LoadSound("Sounds/explosion.wav");
    if (explode.stream.buffer == NULL) printf("explode sound not loaded!\n");

    SetTargetFPS(FPS);
    Astroid astroids[MAX_AESTROIDS];
    Player player = InitPlayer();
    Bullet bullets[MAX_BULLETS];
    Particle particle[MAX_PARTICLES];
    InitBullets(bullets);
    InitAstroids(astroids);
    InitParticle(particle);
    Respawn(astroids);

    while (!WindowShouldClose()) {
        if (state==PLAYING) {
            UpdatePlayer(&player,engine);
            UpdateBullets(bullets);
            UpdateAstroids(astroids);

            if (IsKeyPressed(KEY_SPACE)) {
                FireBullet(bullets, player.position, player.rotation);
                PlaySound(shoot);
                player.shooterTimer = 0.3f;
            }
            player.shooterTimer -= GetFrameTime();
            if (player.shooterTimer<=0) {StopSound(shoot);}

            Respawn(astroids);
            UpdateParticle(particle);
            CheckCollisions(bullets,astroids,&player,particle,explode);
            if (player.lives<=0)state=GAME_OVER;
            BeginDrawing();
            ClearBackground(BLACK);
            DrawPlayer(&player);
            DrawBullets(bullets);
            DrawAstroids(astroids);
            DrawParticle(particle);
            DrawText(TextFormat("Score: %d", player.score), 10, 10, 20, WHITE);
            DrawText(TextFormat("Lives: %d", player.lives), 10, 35, 20, WHITE);
            EndDrawing();
        }
        if (state==GAME_OVER) {

            BeginDrawing();
            ClearBackground(BLACK);
            const char* gameOver = "GAME OVER";
            const char* scoreText = TextFormat("Score: %d", player.score);
            const char* restartText = "PRESS R TO RESTART";

            DrawText(gameOver,
                SCREEN_WIDTH/2 - MeasureText(gameOver, 60)/2,
                SCREEN_HEIGHT/2 - 100, 60, WHITE);

            DrawText(scoreText,
                SCREEN_WIDTH/2 - MeasureText(scoreText, 40)/2,
                SCREEN_HEIGHT/2, 40, WHITE);

            DrawText(restartText,
                SCREEN_WIDTH/2 - MeasureText(restartText, 30)/2,
                SCREEN_HEIGHT/2 + 70, 30, GRAY);
            if (IsKeyPressed(KEY_R)) {
                player = InitPlayer();
                InitBullets(bullets);
                InitAstroids(astroids);
                InitParticle(particle);
                Respawn(astroids);
                state=PLAYING;
            }
            EndDrawing();
        }
    }
    UnloadSound(shoot);
    UnloadSound(engine);
    UnloadSound(explode);
    CloseWindow();
    return 0;
}