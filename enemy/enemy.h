#ifndef ENEMY_H
#define ENEMY_H

#include "../utils/constants.h"

extern int inimigos[ENEMY_ARRAY_MAX_SIZE][2];
extern bool inimigoVivo[ENEMY_ARRAY_MAX_SIZE];
extern int dirInimigo;

void initEnemy();
void moveEnemies();

#endif