#include "logic.h"
#include "../mapa/mapa.h" // For ALTURA_MAPA, LARGURA_MAPA
#include "../utils/constants.h" // For ENEMY_EXPLOSION_FREQ, etc.
#include <windows.h> // For Beep()
#include <cstdlib> // For rand()
#include <algorithm> // For std::min, std::remove_if

// --- Placeholder implementations for functions declared in logic.h ---
// You should replace these with your actual implementations if they exist.
void updatePlayerBullets() {
    for (auto& bullet : playerBullets) {
        bullet.second--; // Move bullet up
    }
    playerBullets.erase(std::remove_if(playerBullets.begin(), playerBullets.end(),
                                       [](const auto& b){ return b.second < 0; }),
                        playerBullets.end());
}
void updateTiroInimigo() {
    if (tiroInimigoAtivo) {
        tiroInimigoY++;
        if (tiroInimigoY >= ALTURA_MAPA) {
            tiroInimigoAtivo = false;
        }
    }
}
void updateItemDrop() {
    if (itemDropActive) {
        itemDropY++;
        if (itemDropY >= ALTURA_MAPA) {
            itemDropActive = false;
        }
    }
}
void checkEndOfGame() {
    // Condição de derrota: Inimigo alcançou a base
    for (int i = 0; i < TOTAL_INITIAL_ENEMIES; ++i) {
        if (inimigoVivo[i] && inimigos[i][1] >= ALTURA_MAPA - 1) {
            gameOver = true;
            playerWon = false;
            return; // Fim de jogo, não precisa checar outras condições
        }
    }

    // Condição de derrota: Vidas acabaram
    if (vidas <= 0) {
        gameOver = true;
        playerWon = false;
        return;
    }
    bool allEnemiesDefeated = true;
    for (int i = 0; i < TOTAL_INITIAL_ENEMIES; ++i) {
        if (inimigoVivo[i]) {
            allEnemiesDefeated = false;
            break;
        }
    }

    // Condição de vitória: Todos os inimigos foram derrotados
    if (allEnemiesDefeated) {
        gameOver = true;
        playerWon = true;
    }
}
// --- End of placeholder implementations ---

void checkCollisions() {
    auto now = std::chrono::high_resolution_clock::now();

    // Player bullet vs Enemy
    for (auto itBullet = playerBullets.begin(); itBullet != playerBullets.end(); ) {
        bool bulletHit = false;
        for (int i = 0; i < TOTAL_INITIAL_ENEMIES; ++i) {
            if (inimigoVivo[i] && itBullet->first == inimigos[i][0] && itBullet->second == inimigos[i][1]) {
                inimigoVivo[i] = false;
                score += 10;
                enemiesDefeatedCount++;

                explosionActiveEnemy = true;
                explosionEnemyX = inimigos[i][0];
                explosionEnemyY = inimigos[i][1];
                enemyExplosionStartTime = now;
                Beep(ENEMY_EXPLOSION_FREQ, ENEMY_EXPLOSION_DUR); // Som de explosão de inimigo

                if (rand() % 100 < currentItemDropChance) {
                    itemDropActive = true;
                    itemDropX = inimigos[i][0];
                    itemDropY = inimigos[i][1];
                    itemDropType = static_cast<ItemType>(rand() % 6); // Random item type
                }

                bulletHit = true;
                break;
            }
        }
        if (bulletHit) {
            itBullet = playerBullets.erase(itBullet);
        } else {
            ++itBullet;
        }
    }

    // Enemy bullet vs Player
    if (tiroInimigoAtivo && tiroInimigoY == ALTURA_MAPA - 1 && tiroInimigoX == naveX) {
        vidas--;
        tiroInimigoAtivo = false;

        explosionActivePlayer = true;
        explosionPlayerX = naveX;
        explosionPlayerY = ALTURA_MAPA - 1;
        playerExplosionStartTime = now;
        Beep(PLAYER_EXPLOSION_FREQ, PLAYER_EXPLOSION_DUR); // Som de explosão do jogador

        if (vidas <= 0) {
            gameOver = true;
        }
    }

    // Player vs Item Drop
    if (itemDropActive && itemDropY == ALTURA_MAPA - 1 && itemDropX == naveX) {
        // Apply item effect
        switch (itemDropType) {
            case ItemType::EXTRA_LIFE: vidas++; break;
            case ItemType::SPEED_BOOST: speedBoostEndTime = now + SPEED_BOOST_DURATION; break;
            case ItemType::EXTRA_SHOT: maxPlayerBulletsAllowed = EXTRA_SHOT_MAX_BULLETS; extraShotEndTime = now + EXTRA_SHOT_DURATION; break;
            case ItemType::MULTI_SHOT: multiShotActive = true; multiShotEndTime = now + MULTI_SHOT_DURATION; break;
            case ItemType::BONUS_POINTS: score += BONUS_POINTS_AMOUNT; break;
            case ItemType::ENEMY_FREEZE: enemyFreezeEndTime = now + ENEMY_FREEZE_DURATION; break;
        }
        itemDropActive = false;
        Beep(700, 70); // Som de item coletado
    }
}

void updateExplosions() {
    auto now = std::chrono::high_resolution_clock::now();

    // Desativa a explosão do inimigo se a duração já passou
    if (explosionActiveEnemy && (now - enemyExplosionStartTime >= EXPLOSION_DURATION)) {
        explosionActiveEnemy = false;
    }

    // Desativa a explosão do jogador se a duração já passou
    if (explosionActivePlayer && (now - playerExplosionStartTime >= EXPLOSION_DURATION)) {
        explosionActivePlayer = false;
    }
}