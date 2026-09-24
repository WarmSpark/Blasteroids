#include "raylib.h"
#include "raymath.h"
#include "aestroids.h"
#include <math.h>

#define SCREEN_WIDTH 1280
#define SCREEN_HEIGHT 720

void InitAstroids(Astroid astroids[]) {
  for (int i = 0; i < MAX_AESTROIDS; i++) {
    astroids[i].active = false;
  }
}

void SpawnAstroids(Astroid astroids[], Vector2 position, AstroidSize size) {
  for (int i = 0; i < MAX_AESTROIDS; i++) {
    if (!astroids[i].active) {
      astroids[i].rotation = 0.0f;
      astroids[i].angularVelocity = (float)GetRandomValue(-200, 200) / 100.0f;
      astroids[i].position = position;
      astroids[i].size = size;
      if (size == LARGE)
        astroids[i].radius = 60.0f;
      if (size == MEDIUM)
        astroids[i].radius = 30.0f;
      if (size == SMALL)
        astroids[i].radius = 15.0f;
      for (int j = 0; j < 8; j++) {
        float angle = (j * 45.0f) * DEG2RAD;
        float dist = GetRandomValue((int)(astroids[i].radius * 0.6f),
                                    (int)(astroids[i].radius));
        astroids[i].points[j].x = sinf(angle) * dist;
        astroids[i].points[j].y = -cosf(angle) * dist;
      }
      float angle = GetRandomValue(5, 175) * DEG2RAD;
      float speed = GetRandomValue(45, 200);
      astroids[i].velocity.x = sinf(angle) * speed;
      astroids[i].velocity.y = -cosf(angle) * speed;
      astroids[i].active = true;
      break;
    }
  }
}

void UpdateAstroids(Astroid astroid[]) {
  for (int i = 0; i < MAX_AESTROIDS; i++) {
    float dt = GetFrameTime();
    if (astroid[i].active) {
      astroid[i].position.x += astroid[i].velocity.x * dt;
      astroid[i].position.y += astroid[i].velocity.y * dt;
      astroid[i].rotation += astroid[i].angularVelocity * dt;
      if (astroid[i].position.x > SCREEN_WIDTH)
        astroid[i].position.x = 0;
      if (astroid[i].position.x < 0)
        astroid[i].position.x = SCREEN_WIDTH;
      if (astroid[i].position.y > SCREEN_HEIGHT)
        astroid[i].position.y = 0;
      if (astroid[i].position.y < 0)
        astroid[i].position.y = SCREEN_HEIGHT;
    }
  }
}

void DrawAstroids(Astroid astroids[]) {
  for (int i = 0; i < MAX_AESTROIDS; i++) {
    if (astroids[i].active) {
      Color rimColor;
      if (astroids[i].size == LARGE) {
        rimColor = (Color){255, 145, 55, 255};
      } else if (astroids[i].size == MEDIUM) {
        rimColor = (Color){255, 195, 70, 255};
      } else {
        rimColor = (Color){255, 95, 65, 255};
      }

      Vector2 rotPts[8];
      for (int j = 0; j < 8; j++) {
        rotPts[j] = Vector2Rotate(astroids[i].points[j], astroids[i].rotation);
      }

      for (int j = 0; j < 8; j++) {
        Vector2 p1 = rotPts[j];
        Vector2 p2 = rotPts[(j + 1) % 8];

        Vector2 edge = Vector2Subtract(p2, p1);
        Vector2 norm = Vector2Normalize((Vector2){-edge.y, edge.x});
        float dot = norm.x * -0.707f + norm.y * -0.707f;
        float bright = 0.5f + 0.5f * dot;

        unsigned char r = (unsigned char)(20 + 35 * bright);
        unsigned char g = (unsigned char)(25 + 40 * bright);
        unsigned char b = (unsigned char)(38 + 55 * bright);
        Color facetColor = (Color){r, g, b, 255};

        DrawTriangle(astroids[i].position,
                     Vector2Add(astroids[i].position, p2),
                     Vector2Add(astroids[i].position, p1), facetColor);

        DrawLineV(astroids[i].position, Vector2Add(astroids[i].position, p1),
                  (Color){55, 70, 95, 110});

        DrawLineEx(Vector2Add(astroids[i].position, p1),
                   Vector2Add(astroids[i].position, p2), 3.5f,
                   Fade(rimColor, 0.25f));
        DrawLineEx(Vector2Add(astroids[i].position, p1),
                   Vector2Add(astroids[i].position, p2), 1.8f, rimColor);
      }
    }
  }
}

void SpiltAstroid(Astroid astroids[], int index) {
  if (astroids[index].size == LARGE) {
    SpawnAstroids(astroids, astroids[index].position, MEDIUM);
    SpawnAstroids(astroids, astroids[index].position, MEDIUM);
  }
  if (astroids[index].size == MEDIUM) {
    SpawnAstroids(astroids, astroids[index].position, SMALL);
    SpawnAstroids(astroids, astroids[index].position, SMALL);
  }

  astroids[index].active = false;
}
