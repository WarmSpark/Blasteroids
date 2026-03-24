//
// Created by divyansh_1410 on 23/03/26.
//
#include "player.h"
#include "raylib.h"
#include <math.h>

#define PLAYER_SPEED 200.0f
#define PLAYER_ROTATION_SPEED 180.0f
#define MAXIMUM_SPEED 300.0f
#define FRICTION 0.99f
#define SCREEN_WIDTH 1280
#define SCREEN_HEIGHT 720

Player InitPlayer(void) {

    Player p;
    p.position = (Vector2){SCREEN_WIDTH/2,SCREEN_HEIGHT/2};
    p.velocity=  (Vector2){0.0f,0.0f};
    p.rotation=0.0f;
    p.health=100;
    p.score=0;
    p.lives=3;
    p.shooterTimer=0.0f;
    return p;
}
void UpdatePlayer(Player *player,Sound sound) {

    float dt=GetFrameTime();
    if (IsKeyDown(KEY_LEFT)||IsKeyDown(KEY_A)) {
        player->rotation-=PLAYER_ROTATION_SPEED*dt;
    }
    if (IsKeyDown(KEY_RIGHT)||IsKeyDown(KEY_D)) {
        player->rotation+=PLAYER_ROTATION_SPEED*dt;
    }

    if (player->rotation>360.0f) player->rotation=0.0f;
    if (player->rotation<0.0f) player->rotation=360.0f;//rotation normalize


    if (IsKeyDown(KEY_UP)||IsKeyDown(KEY_W)) {
        if (!IsSoundPlaying(sound)){PlaySound(sound);}

        float rad=player->rotation*DEG2RAD;
        player->velocity.x+=sinf(rad)*PLAYER_SPEED*dt;
        player->velocity.y-=cosf(rad)*PLAYER_SPEED*dt;

    }
    else {StopSound(sound);}
    //friction in action
    player->velocity.x *= powf(FRICTION,dt * 60.0f);
    player->velocity.y *= powf(FRICTION, dt * 60.0f);

    float x_speed=player->velocity.x;
    float y_speed=player->velocity.y;
    float speed = sqrtf((x_speed*x_speed)+(y_speed*y_speed));
    if (speed>MAXIMUM_SPEED) {
        player->velocity.x=(x_speed/MAXIMUM_SPEED)*MAXIMUM_SPEED;
        player->velocity.y=(y_speed/MAXIMUM_SPEED)*MAXIMUM_SPEED;
    }

    player->position.x+=player->velocity.x*dt;
    player->position.y+=player->velocity.y*dt;

    if (player->position.x > SCREEN_WIDTH)  player->position.x = 0;
    if (player->position.x < 0)             player->position.x = SCREEN_WIDTH;

    if (player->position.y > SCREEN_HEIGHT) player->position.y = 0;
    if (player->position.y < 0)             player->position.y = SCREEN_HEIGHT;
}
void DrawPlayer(const Player *player) {

    Vector2 v1,v2,v3;
    float rad=player->rotation*DEG2RAD;
    v1.x=player->position.x+sinf(rad)*20;
    v1.y=player->position.y-cosf(rad)*20;

    v2.x=player->position.x+sinf(rad+2.5f)*15;
    v2.y=player->position.y-cosf(rad+2.5f)*15;

    v3.x=player->position.x+sinf(rad-2.5f)*15;
    v3.y=player->position.y-cosf(rad-2.5f)*15;
    DrawTriangleLines(v1,v2,v3,WHITE);
    if (IsKeyDown(KEY_UP)||IsKeyDown(KEY_W)) {
        Vector2 flame;
        flame.x=player->position.x-sinf(rad)*18;
        flame.y=player->position.y+cosf(rad)*18;
        DrawCircleV(flame,3,GREEN);
    }


}








