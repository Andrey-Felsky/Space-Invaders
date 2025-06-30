#ifndef LOGICA_H
#define LOGICA_H

#include <chrono>
#include <vector>  // Required for vector
#include <utility> // Required for pair
#include "../utils/constants.h"
#include "../player/player.h"
#include "../enemy/enemy.h"
using namespace std;

struct Barrier {
    int x, y;
    char shape[BARRIER_HEIGHT][BARRIER_WIDTH]; // estrutura das barreiras
};
extern Barrier barriers[NUM_BARRIERS];

extern Player players[2];
extern bool inimigoVivo[ENEMY_ARRAY_MAX_SIZE];
extern int inimigos[ENEMY_ARRAY_MAX_SIZE][2];
extern bool gameOver;
extern bool playerWon;

extern bool tiroInimigoAtivo;
extern int tiroInimigoX, tiroInimigoY;

extern bool itemDropActive;
extern int itemDropX, itemDropY;
extern ItemType itemDropType;  
extern int currentItemDropChance;
extern GameMode currentGameMode;
extern bool bossFightActive;

extern chrono::milliseconds enemyMoveInterval;
extern int enemiesDefeatedCount;

extern bool explosionActiveEnemy;
extern int explosionEnemyX, explosionEnemyY;
extern chrono::high_resolution_clock::time_point enemyExplosionStartTime;

// definições externas para temporizadores (defined in main.cpp)
extern chrono::high_resolution_clock::time_point speedBoostEndTime;
extern chrono::high_resolution_clock::time_point extraShotEndTime;
extern chrono::high_resolution_clock::time_point multiShotEndTime;
extern chrono::high_resolution_clock::time_point enemyFreezeEndTime;
extern chrono::high_resolution_clock::time_point lastEnemyShotTime; // Para controlar o tiro inimigo
extern chrono::milliseconds originalEnemyMoveIntervalMs;
 
void updatePlayerBullets(); // Renamed from updateTire
void checkCollisions();
void checkEndOfGame();
void updateTiroInimigo();
void updateItemDrop();
void updateExplosions();

#endif