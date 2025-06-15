#ifndef ENEMY_H
#define ENEMY_H

#include "../utils/gameElements.h" // Para MAX_ENEMIES

extern float inimigos[MAX_ENEMIES][2];
extern bool inimigoVivo[MAX_ENEMIES];
extern int dirInimigo;

// Novo: timer para controlar a descida dos inimigos
extern float enemyDescendTimer;
extern const float ENEMY_DESCEND_INTERVAL; // Definir em enemy.cpp

void initEnemy();
void moveEnemies();

#endif