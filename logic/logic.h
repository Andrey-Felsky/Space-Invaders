#ifndef LOGICA_H
#define LOGICA_H

#include <chrono>
#include <vector>  // Required for std::vector
#include <utility> // Required for std::pair
#include "../utils/constants.h"

extern std::vector<std::pair<int, int>> playerBullets;
extern int naveX;
extern int score;
extern bool gameOver;
extern int vidas;

extern bool tiroInimigoAtivo;
extern int tiroInimigoX, tiroInimigoY;

extern bool itemDropActive;
extern int itemDropX, itemDropY;
extern ItemType itemDropType; // Now using the enum from constants.h

extern int maxPlayerBullets;    // For power-ups
extern bool multiShotActive;    // For power-ups
extern std::chrono::milliseconds enemyMoveInterval;
extern const std::chrono::milliseconds INITIAL_ENEMY_MOVE_INTERVAL;
extern const std::chrono::milliseconds MIN_ENEMY_MOVE_INTERVAL;

extern int enemiesDefeatedCount;

extern bool explosionActiveEnemy;
extern int explosionEnemyX, explosionEnemyY;
extern bool explosionActivePlayer;
extern int explosionPlayerX, explosionPlayerY;
extern std::chrono::high_resolution_clock::time_point explosionStartTime;

// Extern declarations for power-up timers (defined in main.cpp)
extern std::chrono::high_resolution_clock::time_point speedBoostEndTime;
extern std::chrono::high_resolution_clock::time_point extraShotEndTime;
extern std::chrono::high_resolution_clock::time_point multiShotEndTime;
extern std::chrono::high_resolution_clock::time_point enemyFreezeEndTime;
extern std::chrono::milliseconds originalEnemyMoveIntervalMs;
 
void updatePlayerBullets(); // Renamed from updateTire
void checkCollisions();
void checkEndOfGame();
void updateTiroInimigo();
void adjustEnemySpeed();
void tryDropItem(int enemyX, int enemyY);
void updateItemDrop();

#endif