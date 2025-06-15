#ifndef ENEMY_H
#define ENEMY_H

// Include gameElements.h to access MAX_ENEMIES
#include "../utils/gameElements.h" 

extern int inimigos[MAX_ENEMIES][2];    // [i][0] = x, [i][1] = y // Changed to MAX_ENEMIES
extern bool inimigoVivo[MAX_ENEMIES]; // Changed to MAX_ENEMIES
extern int dirInimigo;

void initEnemy();
void moveEnemies();

#endif