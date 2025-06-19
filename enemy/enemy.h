#ifndef ENEMY_H
#define ENEMY_H

#include "../mapa/mapa.h"

extern int inimigos[25][2];    // [i][0] = x, [i][1] = y
extern bool inimigoVivo[25];
extern int dirInimigo;

void initEnemy();
void moveEnemies();

#endif
