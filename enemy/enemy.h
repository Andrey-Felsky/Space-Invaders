#ifndef ENEMY_H
#define ENEMY_H

extern int inimigos[5][2];    // [i][0] = x, [i][1] = y
extern bool inimigoVivo[5];
extern int dirInimigo;

void initEnemy();
void moveEnemies();

#endif
