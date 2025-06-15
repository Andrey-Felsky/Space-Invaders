#include "logic.h"
#include "../enemy/enemy.h" 
#include <iostream>


void updateTire() {
    if (tiroAtivo) {
        tiroY--;
        if (tiroY < 0) {
            tiroAtivo = false;
        }
    }
}

void checkCollisions() {
    for (int i = 0; i < 5; i++) {
        if (inimigoVivo[i] && tiroAtivo &&
            tiroX == inimigos[i][0] && tiroY == inimigos[i][1]) {
            inimigoVivo[i] = false;
            tiroAtivo = false;
            score += 10;
        }
    }
}

void checkEndOfGame() {
    // derrota se algum inimigo chegou na linha da nave
    for (int i = 0; i < 5; i++) {
        if (inimigoVivo[i] && inimigos[i][1] >= 19) {
            gameOver = true;
            //std::cout << "Você perdeu! Inimigos invadiram a base.\n";
        }
    }

    // vitória se todos mortos
    bool venceu = true;
    for (int i = 0; i < 5; i++) {
        if (inimigoVivo[i]) {
            venceu = false;
            break;
        }
    }

    if (venceu) {
        gameOver = true;
        std::cout << "Parabéns, você venceu!\n";
    }
}

void updateTiroInimigo() {
    if (!tiroInimigoAtivo) {
        // Cria lista com índices de inimigos vivos
        int vivos[5];
        int totalVivos = 0;

        for (int i = 0; i < 5; i++) {
            if (inimigoVivo[i]) {
                vivos[totalVivos++] = i;
            }
        }

        if (totalVivos > 0) {
            int index = rand() % totalVivos;
            int escolhido = vivos[index];

            tiroInimigoX = inimigos[escolhido][0];
            tiroInimigoY = inimigos[escolhido][1] + 1;
            tiroInimigoAtivo = true;
        }
    } else {
        tiroInimigoY++;
        if (tiroInimigoY >= altura - 1) {
            tiroInimigoAtivo = false;
        }
        if (tiroInimigoY == altura - 1 && tiroInimigoX == naveX) {
            gameOver = true;
        }
    }
}
