#include "enemy.h"
#include <windows.h>

int dirInimigo = 1;

void initEnemy() {
    int colunasInimigos = 7;
    int linhasInimigos = 3;
    int espacamentoX = 4;
    int espacamentoY = 2;

    int contador = 0;

    for (int linha = 0; linha < linhasInimigos; linha++) {
        for (int coluna = 0; coluna < colunasInimigos; coluna++) {
            inimigos[contador][0] = 2 + coluna * espacamentoX;
            inimigos[contador][1] = 2 + linha * espacamentoY;
            inimigoVivo[contador] = true;
            contador++;
        }
    }
}

void moveEnemies() {
    bool mudouDirecao = false;
    for (int i = 0; i < 25; i++) {
        if (inimigoVivo[i] && (inimigos[i][0] <= 0 || inimigos[i][0] >= largura - 1)) {
            dirInimigo *= -1;
            mudouDirecao = true;
            break;
        }
    }

    for (int i = 0; i < 25; i++) {
        inimigos[i][0] += dirInimigo;
        if (mudouDirecao) {
            inimigos[i][1]++;
        }
    }
}