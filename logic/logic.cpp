#include "logic.h"
#include "../mapa/mapa.h" // For ALTURA_MAPA, LARGURA_MAPA
#include "../utils/constants.h" // For ENEMY_EXPLOSION_FREQ, etc.
#include <windows.h> // For Beep()
#include <cstdlib> // For rand()
#include <algorithm> // For std::min, std::remove_if

// Global variable definitions (declared in logic.h)
std::chrono::high_resolution_clock::time_point lastEnemyShotTime;

// Definition of global barrier array
Barrier barriers[NUM_BARRIERS];

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
    auto now = std::chrono::high_resolution_clock::now();

    // 1. Move existing bullet
    if (tiroInimigoAtivo) {
        tiroInimigoY++;
        if (tiroInimigoY >= ALTURA_MAPA) {
            tiroInimigoAtivo = false;
        }
    }

    // 2. Fire new bullet if conditions met
    if (!tiroInimigoAtivo && (now - lastEnemyShotTime >= ENEMY_SHOT_INTERVAL)) {
        // Find a random active enemy to shoot from
        std::vector<int> activeEnemiesIndices;
        for (int i = 0; i < TOTAL_INITIAL_ENEMIES; ++i) {
            if (inimigoVivo[i]) {
                activeEnemiesIndices.push_back(i);
            }
        }

        if (!activeEnemiesIndices.empty()) {
            int shooterIndex = activeEnemiesIndices[rand() % activeEnemiesIndices.size()];
            tiroInimigoX = inimigos[shooterIndex][0];
            tiroInimigoY = inimigos[shooterIndex][1] + 1; // Start just below the enemy
            tiroInimigoAtivo = true;
            lastEnemyShotTime = now; // Update shot time here
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

    // --- Colisões de Tiros do Jogador ---
    for (auto itBullet = playerBullets.begin(); itBullet != playerBullets.end(); ) {
        bool bulletHitSomething = false;
        int bulletX = itBullet->first;
        int bulletY = itBullet->second;

        // 1. Checar colisão com Barreiras
        for (int i = 0; i < NUM_BARRIERS; ++i) {
            if (bulletX >= barriers[i].x && bulletX < barriers[i].x + BARRIER_WIDTH &&
                bulletY >= barriers[i].y && bulletY < barriers[i].y + BARRIER_HEIGHT) {
                
                int barrierCol = bulletX - barriers[i].x;
                int barrierRow = bulletY - barriers[i].y;

                if (barriers[i].shape[barrierRow][barrierCol] != ' ') {
                    barriers[i].shape[barrierRow][barrierCol] = ' '; // Danifica a barreira
                    bulletHitSomething = true;
                    break; // A bala só pode atingir uma barreira
                }
            }
        }

        if (bulletHitSomething) {
            itBullet = playerBullets.erase(itBullet);
            continue; // Pula para a próxima bala
        }

        // 2. Checar colisão com Inimigos (se não atingiu uma barreira)
        for (int i = 0; i < TOTAL_INITIAL_ENEMIES; ++i) {
            if (inimigoVivo[i] && bulletX == inimigos[i][0] && bulletY == inimigos[i][1]) {
                inimigoVivo[i] = false;
                score += 10;
                enemiesDefeatedCount++;

                explosionActiveEnemy = true;
                explosionEnemyX = inimigos[i][0];
                explosionEnemyY = inimigos[i][1];
                enemyExplosionStartTime = now;
                Beep(ENEMY_EXPLOSION_FREQ, ENEMY_EXPLOSION_DUR);

                if (rand() % 100 < currentItemDropChance) {
                    itemDropActive = true;
                    itemDropX = inimigos[i][0];
                    itemDropY = inimigos[i][1];
                    itemDropType = static_cast<ItemType>(rand() % 6);
                }

                bulletHitSomething = true;
                break; // A bala só pode atingir um inimigo
            }
        }

        if (bulletHitSomething) {
            itBullet = playerBullets.erase(itBullet);
        } else {
            ++itBullet; // Avança apenas se nada foi atingido
        }
    }

    // --- Colisões de Tiros do Inimigo ---
    if (tiroInimigoAtivo) {
        bool enemyBulletHitSomething = false;

        // 1. Checar colisão com Barreiras
        for (int i = 0; i < NUM_BARRIERS; ++i) {
            if (tiroInimigoX >= barriers[i].x && tiroInimigoX < barriers[i].x + BARRIER_WIDTH &&
                tiroInimigoY >= barriers[i].y && tiroInimigoY < barriers[i].y + BARRIER_HEIGHT) {
                
                int barrierCol = tiroInimigoX - barriers[i].x;
                int barrierRow = tiroInimigoY - barriers[i].y;

                if (barriers[i].shape[barrierRow][barrierCol] != ' ') {
                    barriers[i].shape[barrierRow][barrierCol] = ' '; // Danifica a barreira
                    enemyBulletHitSomething = true;
                    break;
                }
            }
        }

        // 2. Checar colisão com o Jogador (se não atingiu uma barreira)
        if (!enemyBulletHitSomething && tiroInimigoY == ALTURA_MAPA - 1 && tiroInimigoX >= naveX - 1 && tiroInimigoX <= naveX + 1) { // Checa uma área maior para a nave
            vidas--;
            enemyBulletHitSomething = true;

            explosionActivePlayer = true;
            explosionPlayerX = naveX;
            explosionPlayerY = ALTURA_MAPA - 1;
            playerExplosionStartTime = now;
            Beep(PLAYER_EXPLOSION_FREQ, PLAYER_EXPLOSION_DUR);

            if (vidas <= 0) {
                gameOver = true;
            }
        }

        if (enemyBulletHitSomething) {
            tiroInimigoAtivo = false;
        }
    }

    // --- Colisões de Inimigos com Barreiras ---
    for (int i = 0; i < TOTAL_INITIAL_ENEMIES; ++i) {
        if (inimigoVivo[i]) {
            int enemyX = inimigos[i][0];
            int enemyY = inimigos[i][1];

            for (int b = 0; b < NUM_BARRIERS; ++b) {
                // Checa se a posição do inimigo se sobrepõe à caixa delimitadora da barreira
                if (enemyX >= barriers[b].x && enemyX < barriers[b].x + BARRIER_WIDTH &&
                    enemyY >= barriers[b].y && enemyY < barriers[b].y + BARRIER_HEIGHT) {

                    int barrierCol = enemyX - barriers[b].x;
                    int barrierRow = enemyY - barriers[b].y;

                    // Checa se o segmento específico da barreira ainda está intacto
                    if (barriers[b].shape[barrierRow][barrierCol] != ' ') {
                        // Colisão detectada!
                        inimigoVivo[i] = false; // Destrói o inimigo
                        barriers[b].shape[barrierRow][barrierCol] = ' '; // Destrói o segmento da barreira

                        explosionActiveEnemy = true; // Ativa a explosão do inimigo
                        explosionEnemyX = enemyX; explosionEnemyY = enemyY;
                        enemyExplosionStartTime = now;
                        Beep(ENEMY_EXPLOSION_FREQ, ENEMY_EXPLOSION_DUR);
                        break; // Inimigo destruído, passa para o próximo inimigo
                    }
                }
            }
        }
    }

    // --- Colisão do Jogador com Itens ---
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