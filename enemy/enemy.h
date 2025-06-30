#ifndef ENEMY_H
#define ENEMY_H
#include <chrono>
#include <vector>
using namespace std;
#include "../utils/constants.h"

extern int inimigos[ENEMY_ARRAY_MAX_SIZE][2];
extern bool inimigoVivo[ENEMY_ARRAY_MAX_SIZE];
extern int dirInimigo;

extern chrono::high_resolution_clock::time_point enemyFreezeEndTime; // Para acessar na main

void initEnemy();
void moveEnemies();

// --- Boss Definitions ---
struct Boss {
    int x, y;
    int health;
    bool active;
    int direction;
    vector<pair<int, int>> bullets;
    chrono::high_resolution_clock::time_point lastShotTime;
};

extern Boss boss;
extern bool bossFightActive;

void initBoss();
void updateBoss();
void updateBossBullets();

#endif // ENEMY_H