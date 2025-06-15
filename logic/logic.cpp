#include "logic.h"
#include "../enemy/enemy.h"
#include <iostream>
#include <cstdlib> // For rand()
#include <cmath>   // For roundf (in collisions) and fabs

// Necessário para acessar as velocidades do gameElements.h
#include "../utils/gameElements.h"

void updateTire() {
    if (tiroAtivo) {
        tiroY -= PLAYER_SHOT_SPEED * deltaTime; // Move baseado na velocidade e tempo

        // Se o tiro sair do mapa
        if (tiroY < 0.0f) {
            tiroAtivo = false;
        }
    }
}

void checkCollisions() {
    for (int i = 0; i < MAX_ENEMIES; i++) {
        if (inimigoVivo[i] && tiroAtivo) {
            // Usa as posições float para uma colisão mais precisa
            // Considera uma "área" de colisão em vez de apenas um ponto exato
            // Uma margem de 0.5f significa que se os centros estiverem a 0.5 unidade, eles colidem
            float margin = 0.4f; // Ajuste esta margem para refinar a colisão

            if (fabs(tiroX - inimigos[i][0]) < margin &&
                fabs(tiroY - inimigos[i][1]) < margin) {
                inimigoVivo[i] = false;
                tiroAtivo = false;
                score += 10;
                break; // Atingiu um inimigo, o tiro deve desaparecer
            }
        }
    }

    // Colisão do tiro inimigo com a nave
    if (tiroInimigoAtivo) {
        float margin = 0.4f; // Margem de colisão

        if (fabs(tiroInimigoX - naveX) < margin &&
            fabs(tiroInimigoY - (altura - 1.0f)) < margin) { // Altura-1 é a linha da nave
            gameOver = true;
            //std::cout << "Você foi atingido!\n";
        }
    }
}

void checkEndOfGame() {
    // derrota se algum inimigo chegou na linha da nave
    for (int i = 0; i < MAX_ENEMIES; i++) {
        if (inimigoVivo[i] && inimigos[i][1] >= altura - 1.0f) { // Comparar com float
            gameOver = true;
            //std::cout << "Você perdeu! Inimigos invadiram a base.\n";
            return; // Sai cedo se o jogo acabou
        }
    }

    // vitória se todos mortos
    bool venceu = true;
    for (int i = 0; i < MAX_ENEMIES; i++) {
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
        int vivos[MAX_ENEMIES];
        int totalVivos = 0;

        for (int i = 0; i < MAX_ENEMIES; i++) {
            if (inimigoVivo[i]) {
                vivos[totalVivos++] = i;
            }
        }

        if (totalVivos > 0) {
            // Dispara um novo tiro inimigo de um inimigo aleatório vivo
            int index = rand() % totalVivos;
            int escolhido = vivos[index];

            tiroInimigoX = inimigos[escolhido][0];
            tiroInimigoY = inimigos[escolhido][1] + 1.0f; // Começa um pouco abaixo do inimigo
            tiroInimigoAtivo = true;
        }
    } else {
        tiroInimigoY += ENEMY_SHOT_SPEED * deltaTime; // Move baseado na velocidade e tempo

        // Se o tiro inimigo sair da tela
        if (tiroInimigoY >= altura - 0.5f) { // Tolerância para atingir a linha da nave ou ir além
            tiroInimigoAtivo = false;
        }
    }
}