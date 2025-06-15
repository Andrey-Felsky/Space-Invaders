#include "logic.h"
#include "../enemy/enemy.h" 
#include <iostream>
#include <cstdlib> // For rand()

void updateTire() {
    if (tiroAtivo) {
        tiroY--;
        if (tiroY < 0) {
            tiroAtivo = false;
        }
    }
}

void checkCollisions() {
    for (int i = 0; i < MAX_ENEMIES; i++) { // Changed from 5 to MAX_ENEMIES
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
    for (int i = 0; i < MAX_ENEMIES; i++) { // Changed from 5 to MAX_ENEMIES
        if (inimigoVivo[i] && inimigos[i][1] >= altura - 1) { // Changed to altura - 1 for consistency
            gameOver = true;
            //std::cout << "Você perdeu! Inimigos invadiram a base.\n";
            return; // Exit early if game is over
        }
    }

    // vitória se todos mortos
    bool venceu = true;
    for (int i = 0; i < MAX_ENEMIES; i++) { // Changed from 5 to MAX_ENEMIES
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
        int vivos[MAX_ENEMIES]; // Changed to MAX_ENEMIES
        int totalVivos = 0;

        for (int i = 0; i < MAX_ENEMIES; i++) { // Changed from 5 to MAX_ENEMIES
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
        if (tiroInimigoY >= altura - 1) { // Adjusted boundary check
            tiroInimigoAtivo = false;
        }
        if (tiroInimigoY == altura - 1 && tiroInimigoX == naveX) {
            gameOver = true;
        }
    }
}