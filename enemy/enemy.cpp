#include "enemy.h"
#include <windows.h>
#include "../utils/constants.h"
#include <chrono> // Required for time checks

// External declaration for the freeze timer
extern std::chrono::high_resolution_clock::time_point enemyFreezeEndTime;

int dirInimigo = 1;

void initEnemy() {
    int contador = 0;

    for (int linha = 0; linha < NUM_ENEMY_ROWS_INIT; linha++) {
        for (int coluna = 0; coluna < NUM_ENEMY_COLS_INIT; coluna++) {
            inimigos[contador][0] = 2 + coluna * ENEMY_INIT_SPACING_X;
            inimigos[contador][1] = 2 + linha * ENEMY_INIT_SPACING_Y;
            inimigoVivo[contador] = true;
            contador++;
        }
    }
}

void moveEnemies() {
    // Check if enemies are frozen
    if (std::chrono::high_resolution_clock::now() < enemyFreezeEndTime) {
        return; // Skip movement if frozen
    }

    bool hitEdge = false;
    for (int i = 0; i < TOTAL_INITIAL_ENEMIES; i++) {
        if (inimigoVivo[i]) {
            inimigos[i][0] += dirInimigo;

            if (inimigos[i][0] >= LARGURA_MAPA - 1 || inimigos[i][0] <= 0) {
                hitEdge = true;
            }
        }
    }

    if (hitEdge) {
        dirInimigo *= -1;
        for (int i = 0; i < TOTAL_INITIAL_ENEMIES; i++) {
            if (inimigoVivo[i]) {
                inimigos[i][1]++;
            }
        }
    }
}