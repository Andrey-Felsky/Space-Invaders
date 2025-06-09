#include "enemy.h"


int dirInimigo = 1;

void initEnemy() {
    for (int i = 0; i < 5; i++) {
        inimigos[i][0] = 2 + i * 5; // x
        inimigos[i][1] = 2;         // y
        inimigoVivo[i] = true;
    }
}

void moveEnemies() {
    for (int i = 0; i < 5; i++) {
        inimigos[i][0] += dirInimigo;
    }

    // inverter direção se algum bater na borda
    for (int i = 0; i < 5; i++) {
        if (inimigoVivo[i] && (inimigos[i][0] <= 0 || inimigos[i][0] >= 29)) {
            dirInimigo *= -1;
            for (int j = 0; j < 5; j++) {
                inimigos[j][1]++; // desce todos
            }
            break;
        }
    }
}
