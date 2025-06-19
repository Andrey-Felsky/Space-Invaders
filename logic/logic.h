#ifndef LOGICA_H
#define LOGICA_H

#include <chrono>

extern bool tiroAtivo;
extern int tiroX, tiroY;

extern int naveX;
extern int score;
extern bool gameOver;
extern int vidas; 

extern bool tiroInimigoAtivo;
extern int tiroInimigoX, tiroInimigoY;

extern const int largura;
extern const int altura;

extern std::chrono::milliseconds enemyMoveInterval;
extern const std::chrono::milliseconds INITIAL_ENEMY_MOVE_INTERVAL;
extern const std::chrono::milliseconds MIN_ENEMY_MOVE_INTERVAL;
extern const int SPEED_DECREASE_PER_TIER_MS;
extern int enemiesDefeatedCount;

void updateTire();
void checkCollisions();
void checkEndOfGame();
void updateTiroInimigo();
void adjustEnemySpeed();

#endif