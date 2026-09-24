#include "aestroids.h"
#include "bullets.h"
#include "collision.h"
#include "crates.h"
#include "particle.h"
#include "player.h"
#include "raylib.h"
#include "raymath.h"
#include "starfield.h"
#include <stddef.h>
#include <stdio.h>

#if defined(PLATFORM_WEB)
#include <emscripten/emscripten.h>
#endif

#define SCREEN_WIDTH 1280
#define SCREEN_HEIGHT 720
#define FPS 60

typedef enum {
  MENU,
  PLAYING,
  GAME_OVER,
} GameState;

GameState state = MENU;

static Astroid astroids[MAX_AESTROIDS];
static Player player;
static Bullet bullets[MAX_BULLETS];
static Particle particle[MAX_PARTICLES];
static Star stars[MAX_STARS];
static Crate crates[MAX_CRATES];

static Sound shoot;
static Sound engine;
static Sound explode;

bool alldead(Astroid astroids[]) {
  for (int i = 0; i < MAX_AESTROIDS; i++) {
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
      if (edge == 0)
        pos = (Vector2){GetRandomValue(0, SCREEN_WIDTH), 0};
      if (edge == 1)
        pos = (Vector2){GetRandomValue(0, SCREEN_WIDTH), SCREEN_HEIGHT};
      if (edge == 2)
        pos = (Vector2){0, GetRandomValue(0, SCREEN_HEIGHT)};
      if (edge == 3)
        pos = (Vector2){SCREEN_WIDTH, GetRandomValue(0, SCREEN_HEIGHT)};
      SpawnAstroids(astroids, pos, LARGE);
    }
  }
}

void UpdateDrawFrame(void) {
  if (state == MENU) {
    UpdateStarfield(stars, (Vector2){0, 0});

    if (IsKeyPressed(KEY_SPACE) || IsKeyPressed(KEY_ENTER) || IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
      state = PLAYING;
    }

    BeginDrawing();
    ClearBackground(BLACK);
    DrawStarfield(stars);

    const char *title = "BLASTEROIDS";
    int titleSize = 72;
    DrawText(title, SCREEN_WIDTH / 2 - MeasureText(title, titleSize) / 2 + 2, 82, titleSize, (Color){0, 180, 240, 100});
    DrawText(title, SCREEN_WIDTH / 2 - MeasureText(title, titleSize) / 2, 80, titleSize, (Color){0, 240, 255, 255});

    const char *sub = "CELESTIAL ORBITAL SHOOTER // WASM EDITION";
    DrawText(sub, SCREEN_WIDTH / 2 - MeasureText(sub, 18) / 2, 168, 18, (Color){255, 195, 70, 255});

    DrawRectangleRounded((Rectangle){SCREEN_WIDTH / 2.0f - 320, 215, 640, 315}, 0.08f, 6, (Color){9, 16, 30, 235});
    DrawRectangleRoundedLines((Rectangle){SCREEN_WIDTH / 2.0f - 320, 215, 640, 315}, 0.08f, 6, (Color){0, 220, 255, 140});

    DrawText("FLIGHT MANUAL & CONTROLS", SCREEN_WIDTH / 2 - MeasureText("FLIGHT MANUAL & CONTROLS", 20) / 2, 235, 20, WHITE);

    DrawText("[ W ] / [ UP ]", SCREEN_WIDTH / 2 - 270, 280, 18, (Color){0, 245, 255, 255});
    DrawText("Forward Thruster (Burns Fuel)", SCREEN_WIDTH / 2 - 50, 280, 18, (Color){200, 215, 235, 255});

    DrawText("[ A ] / [ D ]", SCREEN_WIDTH / 2 - 270, 315, 18, (Color){0, 245, 255, 255});
    DrawText("Rotate Ship Left / Right", SCREEN_WIDTH / 2 - 50, 315, 18, (Color){200, 215, 235, 255});

    DrawText("[ S ] / [ DOWN ]", SCREEN_WIDTH / 2 - 270, 350, 18, (Color){0, 245, 255, 255});
    DrawText("Active Retro-Brakes (Stop Drift)", SCREEN_WIDTH / 2 - 50, 350, 18, (Color){200, 215, 235, 255});

    DrawText("[ SPACE ]", SCREEN_WIDTH / 2 - 270, 385, 18, (Color){255, 90, 80, 255});
    DrawText("Fire Plasma Blaster (-1 Ammo)", SCREEN_WIDTH / 2 - 50, 385, 18, (Color){200, 215, 235, 255});

    DrawText("[ F ] CRATE", SCREEN_WIDTH / 2 - 270, 425, 18, LIME);
    DrawText("Fuel Canister (+35 Fuel)", SCREEN_WIDTH / 2 - 50, 425, 18, (Color){200, 215, 235, 255});

    DrawText("[ A ] CRATE", SCREEN_WIDTH / 2 - 270, 460, 18, YELLOW);
    DrawText("Ammo Box (+20 Munitions)", SCREEN_WIDTH / 2 - 50, 460, 18, (Color){200, 215, 235, 255});

    float time = (float)GetTime();
    if (((int)(time * 2.2f)) % 2 == 0) {
      const char *prompt = ">> PRESS SPACE TO INITIATE <<";
      DrawText(prompt, SCREEN_WIDTH / 2 - MeasureText(prompt, 26) / 2, 570, 26, (Color){255, 220, 60, 255});
    }

    const char *hint = "Tip: Zero-G inertia active. Use [S] to halt before rock collisions.";
    DrawText(hint, SCREEN_WIDTH / 2 - MeasureText(hint, 16) / 2, 625, 16, GRAY);

    EndDrawing();
    return;
  }

  if (state == PLAYING) {
    UpdatePlayer(&player, engine);
    UpdateStarfield(stars, player.velocity);
    UpdateCrates(crates);
    CheckCratePickup(crates, &player);

    if ((IsKeyDown(KEY_W) || IsKeyDown(KEY_UP)) && player.fuel > 0) {
      float rad = player.rotation * DEG2RAD;
      Vector2 exhaustPos;
      exhaustPos.x = player.position.x - sinf(rad) * 18;
      exhaustPos.y = player.position.y + cosf(rad) * 18;
      SpawnExhaust(particle, exhaustPos, player.rotation);
    }

    UpdateBullets(bullets);
    UpdateAstroids(astroids);
    player.shooterTimer -= GetFrameTime();

    if (IsKeyPressed(KEY_SPACE) && player.ammo > 0) {
      FireBullet(bullets, player.position, player.rotation);
      PlaySound(shoot);
      player.shooterTimer = 0.5f;
      player.ammo--;
    }

    if (player.shooterTimer <= 0.0f)
      StopSound(shoot);

    if (player.fuel <= 0.0f) {
      player.fuel += 1.0f * GetFrameTime();
    }

    Respawn(astroids);
    UpdateParticle(particle);
    CheckCollisions(bullets, astroids, &player, particle, crates, explode);

    if (player.lives <= 0) {
      state = GAME_OVER;
    }

    BeginDrawing();
    ClearBackground(BLACK);
    DrawStarfield(stars);
    DrawPlayer(&player);
    DrawBullets(bullets);
    DrawAstroids(astroids);
    DrawParticle(particle);
    DrawCrates(crates);

    DrawText("FUEL:", 10, 60, 20, WHITE);
    DrawRectangle(80, 62, 100, 16, DARKGRAY);
    DrawRectangle(80, 62, (int)(100 * (player.fuel / player.maxFuel)), 16, LIME);

    DrawText(TextFormat("Ammo: %d/%d", player.ammo, player.maxAmmo), 10, 85,
             20, player.ammo > 10 ? YELLOW : RED);
    DrawText(TextFormat("Score: %d", player.score), 10, 10, 20, WHITE);
    DrawText(TextFormat("Lives: %d", player.lives), 10, 35, 20, WHITE);
    EndDrawing();
  }

  if (state == GAME_OVER) {
    BeginDrawing();
    ClearBackground(BLACK);
    const char *gameOver = "GAME OVER";
    const char *scoreText = TextFormat("Score: %d", player.score);
    const char *restartText = "PRESS R TO RESTART";
    const char *menuText = "PRESS M FOR MAIN MENU";

    DrawText(gameOver, SCREEN_WIDTH / 2 - MeasureText(gameOver, 60) / 2,
             SCREEN_HEIGHT / 2 - 110, 60, WHITE);

    DrawText(scoreText, SCREEN_WIDTH / 2 - MeasureText(scoreText, 40) / 2,
             SCREEN_HEIGHT / 2 - 10, 40, WHITE);

    DrawText(restartText, SCREEN_WIDTH / 2 - MeasureText(restartText, 26) / 2,
             SCREEN_HEIGHT / 2 + 60, 26, (Color){255, 220, 60, 255});

    DrawText(menuText, SCREEN_WIDTH / 2 - MeasureText(menuText, 20) / 2,
             SCREEN_HEIGHT / 2 + 105, 20, GRAY);

    if (IsKeyPressed(KEY_R)) {
      player = InitPlayer();
      InitBullets(bullets);
      InitAstroids(astroids);
      InitParticle(particle);
      InitCrates(crates);
      Respawn(astroids);
      state = PLAYING;
    }

    if (IsKeyPressed(KEY_M)) {
      player = InitPlayer();
      InitBullets(bullets);
      InitAstroids(astroids);
      InitParticle(particle);
      InitCrates(crates);
      Respawn(astroids);
      state = MENU;
    }

    EndDrawing();
  }
}

int main(void) {
  InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "Blasteroids");
  InitAudioDevice();

  shoot = LoadSound("Sounds/laser.wav");
  if (shoot.stream.buffer == NULL)
    printf("shoot sound not loaded!\n");

  engine = LoadSound("Sounds/thrust.wav");
  if (engine.stream.buffer == NULL)
    printf("engine sound not loaded!\n");

  explode = LoadSound("Sounds/explosion.wav");
  if (explode.stream.buffer == NULL)
    printf("explode sound not loaded!\n");

  SetTargetFPS(FPS);

  player = InitPlayer();
  InitBullets(bullets);
  InitAstroids(astroids);
  InitParticle(particle);
  InitStarfield(stars);
  InitCrates(crates);
  Respawn(astroids);

#if defined(PLATFORM_WEB)
  emscripten_set_main_loop(UpdateDrawFrame, 0, 1);
#else
  while (!WindowShouldClose()) {
    UpdateDrawFrame();
  }
#endif

  UnloadSound(shoot);
  UnloadSound(engine);
  UnloadSound(explode);
  CloseWindow();
  return 0;
}
