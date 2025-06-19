#ifndef LOGICA_H
#define LOGICA_H

#include <chrono>
#include "../utils/constants.h"

extern bool tiroAtivo;
extern int tiroX, tiroY;

extern int naveX;
extern int score;
extern bool gameOver;
extern int vidas;

extern bool tiroInimigoAtivo;
extern int tiroInimigoX, tiroInimigoY;

extern bool itemDropActive;
extern int itemDropX, itemDropY;

extern std::chrono::milliseconds enemyMoveInterval;
extern const std::chrono::milliseconds INITIAL_ENEMY_MOVE_INTERVAL;
extern const std::chrono::milliseconds MIN_ENEMY_MOVE_INTERVAL;

extern int enemiesDefeatedCount;

extern bool explosionActiveEnemy;
extern int explosionEnemyX, explosionEnemyY;
extern bool explosionActivePlayer;
extern int explosionPlayerX, explosionPlayerY;
extern std::chrono::high_resolution_clock::time_point explosionStartTime;

void updateTire();
void checkCollisions();
void checkEndOfGame();
void updateTiroInimigo();
void adjustEnemySpeed();
void tryDropItem(int enemyX, int enemyY);
void updateItemDrop();

#endif