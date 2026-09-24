#include "player.h"
#include "raylib.h"
#include "raymath.h"
#include <math.h>

#define PLAYER_SPEED 200.0f
#define PLAYER_ROTATION_SPEED 180.0f
#define MAXIMUM_SPEED 300.0f
#define FRICTION 0.996f
#define SCREEN_WIDTH 1280
#define SCREEN_HEIGHT 720

Player InitPlayer(void) {
  Player p;
  p.position = (Vector2){SCREEN_WIDTH / 2.0f, SCREEN_HEIGHT / 2.0f};
  p.velocity = (Vector2){0.0f, 0.0f};
  p.rotation = 0.0f;
  p.health = 100;
  p.score = 0;
  p.lives = 3;
  p.shooterTimer = 0.0f;
  p.fuel = 100.0f;
  p.maxFuel = 100.0f;
  p.ammo = 50;
  p.maxAmmo = 100;
  p.invulnerableTimer = 2.5f;
  return p;
}

void UpdatePlayer(Player *player, Sound sound) {
  float dt = GetFrameTime();
  if (IsKeyDown(KEY_LEFT) || IsKeyDown(KEY_A)) {
    player->rotation -= PLAYER_ROTATION_SPEED * dt;
  }
  if (IsKeyDown(KEY_RIGHT) || IsKeyDown(KEY_D)) {
    player->rotation += PLAYER_ROTATION_SPEED * dt;
  }

  if (player->rotation > 360.0f)
    player->rotation = 0.0f;
  if (player->rotation < 0.0f)
    player->rotation = 360.0f;

  if (IsKeyDown(KEY_UP) || IsKeyDown(KEY_W)) {
    if (player->fuel > 0) {
      if (!IsSoundPlaying(sound)) {
        PlaySound(sound);
      }
      player->fuel -= 15.0f * dt;
      float rad = player->rotation * DEG2RAD;
      player->velocity.x += sinf(rad) * PLAYER_SPEED * dt;
      player->velocity.y -= cosf(rad) * PLAYER_SPEED * dt;
    } else {
      StopSound(sound);
    }
  } else {
    StopSound(sound);
  }

  player->velocity.x *= powf(FRICTION, dt * 60.0f);
  player->velocity.y *= powf(FRICTION, dt * 60.0f);
  if (IsKeyDown(KEY_S) || IsKeyDown(KEY_DOWN)) {
    player->velocity.x *= 0.93f;
    player->velocity.y *= 0.93f;
  }
  if (player->invulnerableTimer > 0.0f) {
    player->invulnerableTimer -= dt;
  }
  float x_speed = player->velocity.x;
  float y_speed = player->velocity.y;
  float speed = sqrtf((x_speed * x_speed) + (y_speed * y_speed));
  if (speed > MAXIMUM_SPEED) {
    player->velocity.x = (x_speed / MAXIMUM_SPEED) * MAXIMUM_SPEED;
    player->velocity.y = (y_speed / MAXIMUM_SPEED) * MAXIMUM_SPEED;
  }

  player->position.x += player->velocity.x * dt;
  player->position.y += player->velocity.y * dt;

  if (player->position.x > SCREEN_WIDTH)
    player->position.x = 0;
  if (player->position.x < 0)
    player->position.x = SCREEN_WIDTH;

  if (player->position.y > SCREEN_HEIGHT)
    player->position.y = 0;
  if (player->position.y < 0)
    player->position.y = SCREEN_HEIGHT;
}

void DrawPlayer(const Player *player) {
  if (player->invulnerableTimer > 0.0f) {
    if (((int)(player->invulnerableTimer * 12)) % 2 != 0)
      return;
  }

  float rad = player->rotation * DEG2RAD;
  Vector2 fwd = (Vector2){sinf(rad), -cosf(rad)};
  Vector2 right = (Vector2){cosf(rad), sinf(rad)};

  Vector2 nose = Vector2Add(player->position, Vector2Scale(fwd, 22.0f));
  Vector2 rightWing = Vector2Add(Vector2Add(player->position, Vector2Scale(fwd, -14.0f)), Vector2Scale(right, 16.0f));
  Vector2 leftWing = Vector2Add(Vector2Add(player->position, Vector2Scale(fwd, -14.0f)), Vector2Scale(right, -16.0f));
  Vector2 notch = Vector2Add(player->position, Vector2Scale(fwd, -7.0f));

  Vector2 leftGun = Vector2Add(leftWing, Vector2Scale(fwd, 9.0f));
  Vector2 rightGun = Vector2Add(rightWing, Vector2Scale(fwd, 9.0f));
  Vector2 cockpit = Vector2Add(player->position, Vector2Scale(fwd, 3.0f));

  DrawTriangle(nose, leftWing, notch, (Color){24, 38, 58, 255});
  DrawTriangle(nose, notch, rightWing, (Color){18, 28, 44, 255});

  DrawLineEx(nose, leftWing, 3.5f, Fade((Color){0, 240, 255, 255}, 0.3f));
  DrawLineEx(nose, rightWing, 3.5f, Fade((Color){0, 240, 255, 255}, 0.3f));
  DrawLineEx(leftWing, notch, 3.5f, Fade((Color){0, 240, 255, 255}, 0.3f));
  DrawLineEx(rightWing, notch, 3.5f, Fade((Color){0, 240, 255, 255}, 0.3f));

  DrawLineEx(nose, leftWing, 1.8f, (Color){0, 245, 255, 255});
  DrawLineEx(nose, rightWing, 1.8f, (Color){0, 245, 255, 255});
  DrawLineEx(leftWing, notch, 1.8f, (Color){0, 190, 235, 255});
  DrawLineEx(rightWing, notch, 1.8f, (Color){0, 190, 235, 255});

  DrawLineEx(leftWing, leftGun, 2.0f, (Color){0, 245, 255, 255});
  DrawLineEx(rightWing, rightGun, 2.0f, (Color){0, 245, 255, 255});

  DrawCircleV(cockpit, 3.5f, (Color){255, 220, 50, 255});
  DrawCircleLines((int)cockpit.x, (int)cockpit.y, 4.0f, (Color){255, 255, 255, 180});
}
