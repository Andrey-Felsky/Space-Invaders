#ifndef ENEMY_H
#define ENEMY_H
#include <chrono>

#include "../utils/constants.h"

extern int inimigos[ENEMY_ARRAY_MAX_SIZE][2];
extern bool inimigoVivo[ENEMY_ARRAY_MAX_SIZE];
extern int dirInimigo;

extern std::chrono::high_resolution_clock::time_point enemyFreezeEndTime; // For access in main or logic

void initEnemy();
void moveEnemies();

#endif