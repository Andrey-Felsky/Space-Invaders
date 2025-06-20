#include "logic.h"
#include "../enemy/enemy.h" 
#include <iostream>
#include "../utils/gameElements.h"
#include "../utils/constants.h"
#include <cstdlib>
#include <algorithm>
#include <chrono>

const std::chrono::milliseconds MIN_ENEMY_MOVE_INTERVAL(80);
const int SPEED_DECREASE_PER_TIER_MS = 35;

int enemiesDefeatedCount = 0;

bool explosionActiveEnemy = false;
int explosionEnemyX = 0, explosionEnemyY = 0;
bool explosionActivePlayer = false;
int explosionPlayerX = 0, explosionPlayerY = 0;
std::chrono::high_resolution_clock::time_point explosionStartTime;

// Variáveis globais de estado do jogo definidas em main.cpp
extern bool gameOver;
extern bool playerWon;
extern int vidas;

// Difficulty-based variables from main.cpp
extern std::chrono::milliseconds enemyMoveInterval;
extern std::chrono::milliseconds currentEnemyMoveInterval; // The base for the current difficulty
extern int currentItemDropChance;
// ItemType itemDropType = ItemType::EXTRA_LIFE; // Definition moved to main.cpp

// Power-up state variables (defined in main.cpp, declared extern here)
extern int maxPlayerBulletsAllowed;
extern bool multiShotActive;
extern std::chrono::high_resolution_clock::time_point speedBoostEndTime; // Player bullet speed boost
extern std::chrono::high_resolution_clock::time_point extraShotEndTime;
extern std::chrono::high_resolution_clock::time_point multiShotEndTime;
extern std::chrono::high_resolution_clock::time_point enemyFreezeEndTime;
extern std::chrono::milliseconds originalEnemyMoveIntervalMs;


void updatePlayerBullets() {
    // int bulletSpeed = (std::chrono::high_resolution_clock::now() < speedBoostEndTime) ? 2 : 1; // Faster bullets if boosted
    // For simplicity, let's keep bullet speed at 1 unit per frame for now.
    // "Aumento de Velocidade" could also mean faster player ship, or faster fire rate (less delay between shots).
    // The current implementation allows firing as fast as maxPlayerBulletsAllowed is met.

    for (auto it = playerBullets.begin(); it != playerBullets.end(); ) {
        it->second--; // Move bullet up (decrement Y)
        if (it->second < 0) { // Bullet is off-screen
            it = playerBullets.erase(it);
        } else {
            ++it;
        }
    }
}

void adjustEnemySpeed() {
    int speedTier = enemiesDefeatedCount / 2;
    std::chrono::milliseconds newInterval = currentEnemyMoveInterval - std::chrono::milliseconds(speedTier * SPEED_DECREASE_PER_TIER_MS);
    enemyMoveInterval = std::max(newInterval, MIN_ENEMY_MOVE_INTERVAL);
}

void tryDropItem(int enemyX, int enemyY) {
    if (!itemDropActive && (rand() % 100) < currentItemDropChance) { // Use difficulty-based chance
        itemDropActive = true;
        itemDropX = enemyX;
        itemDropY = enemyY;
        itemDropType = static_cast<ItemType>(rand() % 6); // Randomly select from 6 item types
    }
}

void updateItemDrop() {
    if (itemDropActive) {
        itemDropY++;
        if (itemDropY >= ALTURA_MAPA - 1) { 
            if (itemDropX == naveX) {
                // Apply item effect
                auto now = std::chrono::high_resolution_clock::now();
                switch (itemDropType) {
                    case ItemType::EXTRA_LIFE:
                        vidas += EXTRA_LIFE_AMOUNT;
                        break;
                    case ItemType::SPEED_BOOST: // Let's assume this means faster player bullets or movement.
                                                // For now, we'll just set a timer. The effect itself needs more specific implementation.
                                                // If it's enemy speed, it would be:
                                                // originalEnemyMoveIntervalMs = enemyMoveInterval;
                                                // enemyMoveInterval = std::max(MIN_ENEMY_MOVE_INTERVAL, enemyMoveInterval - std::chrono::milliseconds(50)); // Example speed up
                        speedBoostEndTime = now + SPEED_BOOST_DURATION;
                        // This is a placeholder; actual speed boost effect needs more logic where speed is applied.
                        break;
                    case ItemType::EXTRA_SHOT:
                        maxPlayerBulletsAllowed = EXTRA_SHOT_MAX_BULLETS;
                        extraShotEndTime = now + EXTRA_SHOT_DURATION;
                        break;
                    case ItemType::MULTI_SHOT:
                        multiShotActive = true;
                        multiShotEndTime = now + MULTI_SHOT_DURATION;
                        break;
                    case ItemType::BONUS_POINTS:
                        score += BONUS_POINTS_AMOUNT;
                        break;
                    case ItemType::ENEMY_FREEZE:
                        // Storing original speed is important if freeze also implies a speed change after.
                        // For a simple freeze, just set the end time.
                        enemyFreezeEndTime = now + ENEMY_FREEZE_DURATION;
                        break;
                }
                itemDropActive = false;

            } else if (itemDropY >= ALTURA_MAPA) { 
                itemDropActive = false;
            }
        }
    }
}

void checkCollisions() {
    // Player bullets hitting enemies
    for (auto bullet_it = playerBullets.begin(); bullet_it != playerBullets.end(); ) {
        bool bullet_hit_enemy = false;
        for (int i = 0; i < TOTAL_INITIAL_ENEMIES; i++) {
            if (inimigoVivo[i] &&
                bullet_it->first == inimigos[i][0] &&  // bullet X matches enemy X
                bullet_it->second == inimigos[i][1]) { // bullet Y matches enemy Y
                
                inimigoVivo[i] = false; // Enemy is hit
                score += 10; // Base score for hitting an enemy
                
                enemiesDefeatedCount++; 
                adjustEnemySpeed();

                tryDropItem(inimigos[i][0], inimigos[i][1]); // Chance to drop an item

                explosionActiveEnemy = true;
                explosionEnemyX = inimigos[i][0];
                explosionEnemyY = inimigos[i][1];
                explosionStartTime = std::chrono::high_resolution_clock::now();

                bullet_it = playerBullets.erase(bullet_it); // Remove the bullet
                bullet_hit_enemy = true;
                break; // Enemy hit, bullet is gone, process next bullet
            }
        }
        if (!bullet_hit_enemy) {
            ++bullet_it; // Move to the next bullet if this one didn't hit anything
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
            // A mensagem de derrota agora é exibida pela tela de derrota.
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
        playerWon = true; // Define que o jogador venceu para exibir a tela correta.
        // A mensagem de vitória agora é exibida pela tela de vitória.
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