#include "logic.h"
#include "../enemy/enemy.h"
#include <iostream>
#include "../utils/gameElements.h"
#include <cstdlib>
#include <algorithm> // Necessário para std::max

// Definições das variáveis e constantes externas declaradas em logic.h
std::chrono::milliseconds enemyMoveInterval(500); // Definição: valor inicial, DEVE ser o mesmo de INITIAL_ENEMY_MOVE_INTERVAL
const std::chrono::milliseconds INITIAL_ENEMY_MOVE_INTERVAL(500); // Valor inicial da velocidade (mais lento)
const std::chrono::milliseconds MIN_ENEMY_MOVE_INTERVAL(80);    // Valor mínimo da velocidade (mais rápido)
const int SPEED_DECREASE_PER_TIER_MS = 35; // A cada 2 inimigos, diminui 35ms do intervalo

int enemiesDefeatedCount = 0; // Definição: contador de inimigos derrotados

void updateTire() {
    if (tiroAtivo) {
        tiroY--;
        if (tiroY < 0) {
            tiroAtivo = false;
        }
    }
}

// Nova função para ajustar a velocidade dos inimigos
void adjustEnemySpeed() {
    // Calcula o "tier" de velocidade: cada 2 inimigos derrotados aumentam a velocidade
    int speedTier = enemiesDefeatedCount / 2;

    // Calcula o novo intervalo, diminuindo-o a cada tier
    std::chrono::milliseconds newInterval = INITIAL_ENEMY_MOVE_INTERVAL - std::chrono::milliseconds(speedTier * SPEED_DECREASE_PER_TIER_MS);

    // Garante que o intervalo não seja menor que o mínimo (limite de velocidade)
    enemyMoveInterval = std::max(newInterval, MIN_ENEMY_MOVE_INTERVAL);
}

void checkCollisions() {
    for (int i = 0; i < 25; i++) {
        if (inimigoVivo[i] && tiroAtivo &&
            tiroX == inimigos[i][0] && tiroY == inimigos[i][1]) {
            inimigoVivo[i] = false;
            tiroAtivo = false;
            score += 10;
            
            // Inimigo derrotado: atualiza o contador e a velocidade
            enemiesDefeatedCount++; 
            adjustEnemySpeed();
        }
    }
    // Colisão do tiro inimigo com a nave do jogador
    if (tiroInimigoAtivo && tiroInimigoY == altura - 1 && tiroInimigoX == naveX) {
        vidas--;
        tiroInimigoAtivo = false; 
        if (vidas <= 0) {
            gameOver = true;
            std::cout << "Você perdeu! Todas as suas vidas se foram.\n";
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