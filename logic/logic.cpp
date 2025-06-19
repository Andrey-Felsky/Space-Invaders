#include "logic.h"
#include "../enemy/enemy.h"
#include <iostream>
#include "../utils/gameElements.h"
#include <cstdlib>

void updateTire() {
    if (tiroAtivo) {
        tiroY--;
        if (tiroY < 0) {
            tiroAtivo = false;
        }
    }
}

void checkCollisions() {
    for (int i = 0; i < 25; i++) {
        if (inimigoVivo[i] && tiroAtivo &&
            tiroX == inimigos[i][0] && tiroY == inimigos[i][1]) {
            inimigoVivo[i] = false;
            tiroAtivo = false;
            score += 10;
        }
    }
    // Colisão do tiro inimigo com a nave do jogador
    if (tiroInimigoAtivo && tiroInimigoY == altura - 1 && tiroInimigoX == naveX) {
        vidas--; // Decrementa uma vida
        tiroInimigoAtivo = false; // Desativa o tiro inimigo para que não continue "colidindo"
        if (vidas <= 0) {
            gameOver = true;
            std::cout << "Você perdeu! Todas as suas vidas se foram.\n";
        } else {
             // Opcional: Pausar brevemente ou exibir mensagem de que foi atingido
        }
    }
}

void checkEndOfGame() {
    for (int i = 0; i < 25; i++) {
        if (inimigoVivo[i] && inimigos[i][1] >= altura - 1) {
            gameOver = true;
            std::cout << "Você perdeu! Inimigos invadiram a base.\n";
            return;
        }
    }

    bool venceu = true;
    for (int i = 0; i < 25; i++) {
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
        int vivos[25];
        int totalVivos = 0;

        for (int i = 0; i < 25; i++) {
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
        if (tiroInimigoY >= altura) {
            tiroInimigoAtivo = false;
        }
    }
}