#include "enemy.h"
#include <windows.h>

// diz para onde os inimigos estão indo.
// se for 1, eles vao para a direita. Se for -1, eles vao para a esquerda.
int dirInimigo = 1;

void initEnemy() {
    int colunasInimigos = 7;
    int linhasInimigos = 3;
    int espacamentoX = 4;  
    int espacamentoY = 2;  

    //variavel para saber qual posição do array sera preenchido
    int contador = 0;

    for (int linha = 0; linha < linhasInimigos; linha++) {
        for (int coluna = 0; coluna < colunasInimigos; coluna++) {
            // onde cada inimigo vai "nascer" no mapa
            inimigos[contador][0] = 2 + coluna * espacamentoX; // x: comecamos em 2 (para não colar na borda) e somamos o espacamento
            inimigos[contador][1] = 2 + linha * espacamentoY; // y: comecamos em 2 e descemos linha por linha  
            inimigoVivo[contador] = true;
            contador++;
        }
    }
}


void moveEnemies() {
    // move tudo na direcao atual
    for (int i = 0; i < 25; i++) {
        inimigos[i][0] += dirInimigo;
    }

    for (int i = 0; i < 25; i++) {
        // Se o inimigo esta vivo e chegou em alguma borda:
        if (inimigoVivo[i] && (inimigos[i][0] <= 0 || inimigos[i][0] >= 29)) {
            dirInimigo *= -1; //muda a direcao
            for (int j = 0; j < 25; j++) {
                inimigos[j][1]++;
            }
            break;
        }
    }
}

