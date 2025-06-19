#include "enemy.h"
#include <windows.h>
#include "../utils/constants.h"

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