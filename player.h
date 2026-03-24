#ifndef PLAYER_H
#define PLAYER_H
#include "raylib.h"
typedef struct {
    Vector2 position;
    Vector2 velocity;
    float rotation;
    int health;
    int score;
    int lives;
} Player;
Player InitPlayer(void);
void UpdatePlayer(Player *player);
void DrawPlayer(const Player *player);
#endif //PLAYER_H