#ifndef LOGICA_H
#define LOGICA_H

#include <chrono>
#include <vector>  // Required for std::vector
#include <utility> // Required for std::pair
#include "../utils/constants.h"
#include "../player/player.h"
#include "../enemy/enemy.h"

// --- Barrier Definitions ---
struct Barrier {
    int x, y;
    char shape[BARRIER_HEIGHT][BARRIER_WIDTH];
};
extern Barrier barriers[NUM_BARRIERS];
// --- End Barrier Definitions ---

extern Player players[2];
extern bool inimigoVivo[ENEMY_ARRAY_MAX_SIZE];
extern int inimigos[ENEMY_ARRAY_MAX_SIZE][2];
extern bool gameOver;
extern bool playerWon;

extern bool tiroInimigoAtivo;
extern int tiroInimigoX, tiroInimigoY;

extern bool itemDropActive;
extern int itemDropX, itemDropY;
extern ItemType itemDropType; // Now using the enum from constants.h
extern int currentItemDropChance;
extern GameMode currentGameMode;
extern bool bossFightActive;

extern std::chrono::milliseconds enemyMoveInterval;
extern int enemiesDefeatedCount;

extern bool explosionActiveEnemy;
extern int explosionEnemyX, explosionEnemyY;
extern std::chrono::high_resolution_clock::time_point enemyExplosionStartTime;

// Extern declarations for power-up timers (defined in main.cpp)
extern std::chrono::high_resolution_clock::time_point speedBoostEndTime;
extern std::chrono::high_resolution_clock::time_point extraShotEndTime;
extern std::chrono::high_resolution_clock::time_point multiShotEndTime;
extern std::chrono::high_resolution_clock::time_point enemyFreezeEndTime;
extern std::chrono::high_resolution_clock::time_point lastEnemyShotTime; // Para controlar o tiro inimigo
extern std::chrono::milliseconds originalEnemyMoveIntervalMs;
 
void updatePlayerBullets(); // Renamed from updateTire
void checkCollisions();
void checkEndOfGame();
void updateTiroInimigo();
void updateItemDrop();
void updateExplosions();

#endif