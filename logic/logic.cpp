#include "logic.h"
#include "../enemy/enemy.h" 
#include <iostream>
#include "../utils/gameElements.h"
#include "../utils/constants.h"
#include <cstdlib>
#include <algorithm>
#include <chrono>

std::chrono::milliseconds enemyMoveInterval(INITIAL_ENEMY_MOVE_INTERVAL);
const std::chrono::milliseconds INITIAL_ENEMY_MOVE_INTERVAL(500);
const std::chrono::milliseconds MIN_ENEMY_MOVE_INTERVAL(80);
const int SPEED_DECREASE_PER_TIER_MS = 35;

int enemiesDefeatedCount = 0;

bool explosionActiveEnemy = false;
int explosionEnemyX = 0, explosionEnemyY = 0;
bool explosionActivePlayer = false;
int explosionPlayerX = 0, explosionPlayerY = 0;
std::chrono::high_resolution_clock::time_point explosionStartTime;

void updateTire() {
    if (tiroAtivo) {
        tiroY--;
        if (tiroY < 0) {
            tiroAtivo = false;
        }
    }
}

void adjustEnemySpeed() {
    int speedTier = enemiesDefeatedCount / 2;
    std::chrono::milliseconds newInterval = INITIAL_ENEMY_MOVE_INTERVAL - std::chrono::milliseconds(speedTier * SPEED_DECREASE_PER_TIER_MS);
    enemyMoveInterval = std::max(newInterval, MIN_ENEMY_MOVE_INTERVAL);
}

void checkCollisions() {
    for (int i = 0; i < ENEMY_ARRAY_MAX_SIZE; i++) {
        if (inimigoVivo[i] && tiroAtivo &&
            tiroX == inimigos[i][0] && tiroY == inimigos[i][1]) {
            inimigoVivo[i] = false;
            tiroAtivo = false;
            score += 10;
            
            enemiesDefeatedCount++; 
            adjustEnemySpeed();

            explosionActiveEnemy = true;
            explosionEnemyX = inimigos[i][0];
            explosionEnemyY = inimigos[i][1];
            explosionStartTime = std::chrono::high_resolution_clock::now();
        }
    }
    if (tiroInimigoAtivo && tiroInimigoY == ALTURA_MAPA - 1 && tiroInimigoX == naveX) {
        vidas--;
        tiroInimigoAtivo = false; 

        explosionActivePlayer = true;
        explosionPlayerX = naveX;
        explosionPlayerY = ALTURA_MAPA - 1;
        explosionStartTime = std::chrono::high_resolution_clock::now();

        if (vidas <= 0) {
            gameOver = true;
        }
    }
}

void checkEndOfGame() {
    for (int i = 0; i < TOTAL_INITIAL_ENEMIES; i++) {
        if (inimigoVivo[i] && inimigos[i][1] >= ALTURA_MAPA - 1) {
            gameOver = true;
            std::cout << "Você perdeu! Inimigos invadiram a base.\n";
            return;
        }
    }

    bool venceu = true;
    for (int i = 0; i < TOTAL_INITIAL_ENEMIES; i++) {
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
        int vivos[ENEMY_ARRAY_MAX_SIZE];
        int totalVivos = 0;

        for (int i = 0; i < TOTAL_INITIAL_ENEMIES; i++) {
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
        if (tiroInimigoY >= ALTURA_MAPA) {
            tiroInimigoAtivo = false;
        }
    }
}