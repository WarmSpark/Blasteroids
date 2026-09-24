#include "starfield.h"
#include <stdlib.h>

#define SCREEN_WIDTH 1280
#define SCREEN_HEIGHT 720

void InitStarfield(Star stars[]) {
    for (int i = 0; i < MAX_STARS; i++) {
        stars[i].position = (Vector2){
            (float)GetRandomValue(0, SCREEN_WIDTH),
            (float)GetRandomValue(0, SCREEN_HEIGHT)
        };

        if (i < 60) {
            stars[i].speed = 10.0f;
            stars[i].size = 1.0f;
            stars[i].color = Fade(LIGHTGRAY, 0.35f);
        } else if (i < 100) {
            stars[i].speed = 25.0f;
            stars[i].size = 1.5f;
            stars[i].color = Fade(RAYWHITE, 0.7f);
        } else {
            stars[i].speed = 50.0f;
            stars[i].size = 2.0f;
            stars[i].color = (GetRandomValue(0, 1) == 0) ? WHITE : SKYBLUE;
        }
    }
}

void UpdateStarfield(Star stars[], Vector2 playerVelocity) {
    float dt = GetFrameTime();

    for (int i = 0; i < MAX_STARS; i++) {
        float depthFactor = stars[i].speed / 50.0f;
        stars[i].position.x -= playerVelocity.x * depthFactor * 0.15f * dt;
        stars[i].position.y -= playerVelocity.y * depthFactor * 0.15f * dt;

        stars[i].position.y += stars[i].speed * 0.2f * dt;

        if (stars[i].position.x < 0) stars[i].position.x = SCREEN_WIDTH;
        if (stars[i].position.x > SCREEN_WIDTH) stars[i].position.x = 0;
        if (stars[i].position.y < 0) stars[i].position.y = SCREEN_HEIGHT;
        if (stars[i].position.y > SCREEN_HEIGHT) stars[i].position.y = 0;
    }
}

void DrawStarfield(const Star stars[]) {
    for (int i = 0; i < MAX_STARS; i++) {
        DrawCircleV(stars[i].position, stars[i].size, stars[i].color);
    }
}
