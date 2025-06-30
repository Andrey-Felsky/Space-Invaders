#include "logic.h"
#include "../mapa/mapa.h" // For ALTURA_MAPA, LARGURA_MAPA
#include "../utils/constants.h" // For ENEMY_EXPLOSION_FREQ, etc.
#include <windows.h> // For Beep()
#include <cstdlib> // For rand()
#include <algorithm> // For std::min, std::remove_if

extern GameMode currentGameMode;
// Global variable definitions (declared in logic.h)
std::chrono::high_resolution_clock::time_point lastEnemyShotTime;

// Definition of global barrier array
Barrier barriers[NUM_BARRIERS];

// --- Placeholder implementations for functions declared in logic.h ---
// You should replace these with your actual implementations if they exist.
void updatePlayerBullets() {
    for (int i = 0; i < 2; ++i) {
        for (auto it = players[i].bullets.begin(); it != players[i].bullets.end(); ) {
            // Apply movement
            it->x += it->dx;
            it->y += it->dy;

            bool removed = false;

            // Check for wall reflection only for the specific ship type
            if (players[i].shipConfig.type == ShipType::TYPE_4_DIAGONAL_REFLECT) { // Diagonal Reflecting Bullets
                if ((it->x <= 0 && it->dx < 0) || (it->x >= LARGURA_MAPA - 1 && it->dx > 0)) {
                    it->dx *= -1;
                }
            }

            // Remove bullet if it goes off-screen
            if (players[i].shipConfig.type == ShipType::TYPE_5_LASER_BEAM) { // Laser 1x3
                if (it->y + 2 < 0) { // Entire 1x3 laser is off-screen (bottom of laser is above top of screen)
                    it = players[i].bullets.erase(it);
                    removed = true;
                }
            } else if (it->y < 0 || (players[i].shipConfig.type != ShipType::TYPE_4_DIAGONAL_REFLECT && (it->x < 0 || it->x >= LARGURA_MAPA))) { // Standard or Diagonal
                it = players[i].bullets.erase(it);
                removed = true;
            }
            
            if (!removed) {
                ++it;
            }
        }
    }
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
    if (currentGameMode == GameMode::SINGLE_PLAYER) {
        if (players[0].vidas <= 0) {
            gameOver = true;
            playerWon = false;
            return;
        }
    } else { // TWO_PLAYER
        if (players[0].vidas <= 0 && players[1].vidas <= 0) {
            gameOver = true;
            playerWon = false;
            return;
        }
    }

    bool allEnemiesDefeated = true;
    for (int i = 0; i < TOTAL_INITIAL_ENEMIES; ++i) {
        if (inimigoVivo[i]) {
            allEnemiesDefeated = false;
            break;
        }
    }

    // Se todos os inimigos foram derrotados, inicia a luta contra o chefe se ainda não estiver ativa
    if (allEnemiesDefeated && !bossFightActive) {
        initBoss(); // Prepara o chefe para a batalha
        bossFightActive = true;
        return; // Retorna para o loop do jogo para começar a luta contra o chefe
    }

    // Condição de vitória final: O chefe foi derrotado
    if (bossFightActive && boss.health <= 0) {
        gameOver = true;
        playerWon = true;
    }
}
// --- End of placeholder implementations ---

// Helper function to check collision with a player, allowing for different hitbox sizes.
// hitboxWidth should be an odd number (1, 3, 5, etc.).
bool checkPlayerCollision(int objectX, int objectY, const Player& player, int hitboxWidth) {
    if (player.vidas <= 0) {
        return false; // Cannot collide with a dead player.
    }

    // Check if the object is at the player's vertical level.
    if (objectY != ALTURA_MAPA - 1) {
        return false;
    }

    // Calculate the horizontal bounds for collision.
    int halfWidth = hitboxWidth / 2;
    int leftBound = player.x - halfWidth;
    int rightBound = player.x + halfWidth;

    return (objectX >= leftBound && objectX <= rightBound);
}

void checkCollisions() {
    auto now = std::chrono::high_resolution_clock::now();

    // --- Colisões de Tiros do Jogador ---
    for (int p_idx = 0; p_idx < 2; ++p_idx) {
        for (auto itBullet = players[p_idx].bullets.begin(); itBullet != players[p_idx].bullets.end(); ) {
            bool bulletHitSomething = false;
            bool bulletRemoved = false; // Flag to indicate if the bullet was removed from the vector
            int bulletX = itBullet->x;
            int bulletY = itBullet->y;

            // 1. Checar colisão com Barreiras
            // Lasers do not hit barriers, only regular bullets do.
            if (players[p_idx].shipConfig.type != ShipType::TYPE_5_LASER_BEAM) {
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
            }

            if (bulletHitSomething) {
                itBullet = players[p_idx].bullets.erase(itBullet);
                bulletRemoved = true;
                continue; // Pula para a próxima bala
            }

            // 2. Checar colisão com Inimigos (se não atingiu uma barreira)
            for (int i = 0; i < TOTAL_INITIAL_ENEMIES; ++i) {
                if (inimigoVivo[i]) { // Only check against live enemies
                    bool hit = false;
                    if (players[p_idx].shipConfig.type == ShipType::TYPE_5_LASER_BEAM) {
                        // Check collision for a 1x3 vertical beam
                        if (bulletX == inimigos[i][0] && // Same column
                            (bulletY == inimigos[i][1] || bulletY + 1 == inimigos[i][1] || bulletY + 2 == inimigos[i][1])) {
                            hit = true;
                        }
                    } else {
                        // Standard 1x1 bullet collision
                        if (bulletX == inimigos[i][0] && bulletY == inimigos[i][1]) {
                            hit = true;
                        }
                    }

                    if (hit) {
                        inimigoVivo[i] = false;
                        players[p_idx].score += 10;
                        enemiesDefeatedCount++;
                        explosionActiveEnemy = true;
                        explosionEnemyX = inimigos[i][0];
                        explosionEnemyY = inimigos[i][1];
                        enemyExplosionStartTime = now;
                        Beep(ENEMY_EXPLOSION_FREQ, ENEMY_EXPLOSION_DUR);
                        if (rand() % 100 < currentItemDropChance) {
                            itemDropActive = true; itemDropX = inimigos[i][0]; itemDropY = inimigos[i][1]; itemDropType = static_cast<ItemType>(rand() % 6);
                        }
                        if (players[p_idx].shipConfig.type != ShipType::TYPE_5_LASER_BEAM) { // Regular bullets are removed
                            itBullet = players[p_idx].bullets.erase(itBullet);
                            bulletRemoved = true;
                            break; // A bala só pode atingir um inimigo
                        }
                    }
                }
            }

            if (!bulletRemoved) { // Only advance if the bullet was not removed
                ++itBullet; // Avança apenas se nada foi atingido
            }
        }
    }

    // --- Colisões de Tiros do Jogador vs. Chefe ---
    if (bossFightActive && boss.health > 0) {
        for (int p_idx = 0; p_idx < 2; ++p_idx) {
            for (auto itBullet = players[p_idx].bullets.begin(); itBullet != players[p_idx].bullets.end(); ) {
                bool bulletRemoved = false;
                int bulletX = itBullet->x;
                int bulletY = itBullet->y;
                bool hit = false;

                if (players[p_idx].shipConfig.type == ShipType::TYPE_5_LASER_BEAM) {
                    // Check collision for a 1x3 vertical beam against the boss area
                    if (bulletX >= boss.x && bulletX < boss.x + BOSS_WIDTH) { // Is the laser column inside the boss's horizontal span
                        // Check if any of the 3 vertical parts of the laser overlap with the boss's vertical span
                        int laserTop = bulletY;
                        int laserBottom = bulletY + 2;
                        int bossTop = boss.y;
                        int bossBottom = boss.y + BOSS_HEIGHT - 1;
                        if (std::max(laserTop, bossTop) <= std::min(laserBottom, bossBottom)) {
                            hit = true; // Laser hits boss, but doesn't disappear
                        }
                    }
                } else {
                    // Standard 1x1 bullet collision
                    if (bulletX >= boss.x && bulletX < boss.x + BOSS_WIDTH &&
                        bulletY >= boss.y && bulletY < boss.y + BOSS_HEIGHT) {
                        hit = true; // Regular bullet hits boss and is removed
                    }
                }

                if (hit) {
                    boss.health--; // Reduz a vida do chefe
                    itBullet = players[p_idx].bullets.erase(itBullet); // Remove a bala
                    Beep(300, 50); // Som de dano no chefe
                    bulletRemoved = true;
                }

                if (!bulletRemoved) { // Only advance if the bullet was not removed
                    ++itBullet;
                }
            }
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

        // 2. Checar colisão com os Jogadores (se não atingiu uma barreira)
        if (!enemyBulletHitSomething) {
            for (int p_idx = 0; p_idx < 2; ++p_idx) {
                if (checkPlayerCollision(tiroInimigoX, tiroInimigoY, players[p_idx], 1)) { // 1x1 hitbox for damage
                    players[p_idx].vidas--;
                    enemyBulletHitSomething = true;

                    players[p_idx].explosionActive = true;
                    players[p_idx].explosionX = players[p_idx].x;
                    players[p_idx].explosionY = ALTURA_MAPA - 1;
                    players[p_idx].explosionStartTime = now;
                    Beep(PLAYER_EXPLOSION_FREQ, PLAYER_EXPLOSION_DUR);

                    if (players[0].vidas <= 0 && players[1].vidas <= 0) {
                        gameOver = true;
                    }
                    break; // Bala atinge apenas um jogador
                }
            }
        }

        if (enemyBulletHitSomething) {
            tiroInimigoAtivo = false;
        }
    }

    // --- Colisões de Tiros do Chefe ---
    for (auto itBullet = boss.bullets.begin(); itBullet != boss.bullets.end(); ) {
        bool bulletHitSomething = false;
        int bulletX = itBullet->x; // Access x from Bullet struct
        int bulletY = itBullet->y; // Access y from Bullet struct

        // 1. Checar colisão com Barreiras
        for (int i = 0; i < NUM_BARRIERS; ++i) {
            if (bulletX >= barriers[i].x && bulletX < barriers[i].x + BARRIER_WIDTH &&
                bulletY >= barriers[i].y && bulletY < barriers[i].y + BARRIER_HEIGHT) {
                if (barriers[i].shape[0][0] != ' ') {
                    barriers[i].shape[0][0] = ' '; // Danifica a barreira (1x1)
                    bulletHitSomething = true;
                    break;
                }
            }
        }

        // 2. Checar colisão com Jogadores
        if (!bulletHitSomething) {
            for (int p_idx = 0; p_idx < 2; ++p_idx) {
                if (checkPlayerCollision(bulletX, bulletY, players[p_idx], 1)) { // 1x1 hitbox for damage
                    players[p_idx].vidas--;
                    bulletHitSomething = true;
                    players[p_idx].explosionActive = true;
                    players[p_idx].explosionX = players[p_idx].x;
                    players[p_idx].explosionY = ALTURA_MAPA - 1;
                    players[p_idx].explosionStartTime = now;
                    Beep(PLAYER_EXPLOSION_FREQ, PLAYER_EXPLOSION_DUR);
                    break;
                }
            }
        }

        if (bulletHitSomething) {
            itBullet = boss.bullets.erase(itBullet);
        } else {
            ++itBullet;
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
    for (int p_idx = 0; p_idx < 2; ++p_idx) {
        if (itemDropActive && checkPlayerCollision(itemDropX, itemDropY, players[p_idx], 3)) { // 1x3 hitbox for items
            // Apply item effect
            switch (itemDropType) {
                case ItemType::EXTRA_LIFE: players[p_idx].vidas++; break;
                case ItemType::SPEED_BOOST: players[p_idx].speedBoostEndTime = now + SPEED_BOOST_DURATION; break;
                case ItemType::EXTRA_SHOT: players[p_idx].maxBulletsAllowed = EXTRA_SHOT_MAX_BULLETS; players[p_idx].extraShotEndTime = now + EXTRA_SHOT_DURATION; break;
                case ItemType::MULTI_SHOT: players[p_idx].multiShotActive = true; players[p_idx].multiShotEndTime = now + MULTI_SHOT_DURATION; break;
                case ItemType::BONUS_POINTS: players[p_idx].score += BONUS_POINTS_AMOUNT; break;
                case ItemType::ENEMY_FREEZE: enemyFreezeEndTime = now + ENEMY_FREEZE_DURATION; break;
            }
            itemDropActive = false;
            Beep(700, 70); // Som de item coletado
            break; // Item é coletado por apenas um jogador
        }
    }
}

void updateExplosions() {
    auto now = std::chrono::high_resolution_clock::now();

    // Desativa a explosão do inimigo se a duração já passou
    if (explosionActiveEnemy && (now - enemyExplosionStartTime >= EXPLOSION_DURATION)) {
        explosionActiveEnemy = false;
    }

    // Desativa a explosão do jogador se a duração já passou
    for (int i = 0; i < 2; ++i) {
        if (players[i].explosionActive && (now - players[i].explosionStartTime >= EXPLOSION_DURATION)) {
            players[i].explosionActive = false;
        }
    }
}