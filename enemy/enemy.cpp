#include "enemy.h"
#include <chrono>
#include <cstdlib> // For rand()
int dirInimigo = 1;

// --- Boss Global Variables ---
// A definição real está em main.cpp, aqui são apenas usadas pelas funções
extern Boss boss;
extern bool bossFightActive;

void initEnemy()
{
    int startX = (LARGURA_MAPA - (NUM_ENEMY_COLS_INIT * ENEMY_INIT_SPACING_X)) / 2;
    int startY = 2;

    for (int i = 0; i < TOTAL_INITIAL_ENEMIES; i++)
    {
        int row = i / NUM_ENEMY_COLS_INIT;
        int col = i % NUM_ENEMY_COLS_INIT;
        inimigos[i][0] = startX + col * ENEMY_INIT_SPACING_X;
        inimigos[i][1] = startY + row * ENEMY_INIT_SPACING_Y;
        inimigoVivo[i] = true;
    }
    dirInimigo = 1;
}

void moveEnemies()
{
    auto now = std::chrono::high_resolution_clock::now();
    if (now < enemyFreezeEndTime)
    {
        return; // Inimigos estão congelados
    }

    bool moveDown = false;
    for (int i = 0; i < TOTAL_INITIAL_ENEMIES; i++)
    {
        if (inimigoVivo[i])
        {
            if ((inimigos[i][0] >= LARGURA_MAPA - 1 && dirInimigo == 1) || (inimigos[i][0] <= 0 && dirInimigo == -1))
            {
                moveDown = true;
                break;
            }
        }
    }

    if (moveDown)
    {
        for (int i = 0; i < TOTAL_INITIAL_ENEMIES; i++)
        {
            inimigos[i][1]++;
        }
        dirInimigo *= -1;
    }
    else
    {
        for (int i = 0; i < TOTAL_INITIAL_ENEMIES; i++)
        {
            inimigos[i][0] += dirInimigo;
        }
    }
}

// --- Boss Functions Implementation ---

void initBoss() {
    boss.active = true;
    boss.health = BOSS_INITIAL_HEALTH;
    boss.x = (LARGURA_MAPA - BOSS_WIDTH) / 2;
    boss.y = 1; // Posição Y no topo da tela
    boss.direction = 1;
    boss.bullets.clear();
    boss.lastShotTime = std::chrono::high_resolution_clock::now();
}

void updateBoss() {
    if (!boss.active || boss.health <= 0) return;

    // --- Movimento do Chefe ---
    boss.x += boss.direction;
    if (boss.x <= 0 || boss.x + BOSS_WIDTH >= LARGURA_MAPA) {
        boss.direction *= -1; // Inverte a direção ao atingir a borda
    }

    // --- Ataque do Chefe ---
    auto now = std::chrono::high_resolution_clock::now();
    if (now - boss.lastShotTime > BOSS_SHOT_INTERVAL) {
        // Atira uma bala do centro do chefe
        int bulletX = boss.x + BOSS_WIDTH / 2;
        int bulletY = boss.y + BOSS_HEIGHT;
        boss.bullets.push_back({bulletX, bulletY, 0, 1}); // dx=0, dy=1 (move down)
        boss.lastShotTime = now;
    }
}

void updateBossBullets() {
    for (auto it = boss.bullets.begin(); it != boss.bullets.end(); ) {
        it->y++; // Move a bala para baixo
        if (it->y >= ALTURA_MAPA) {
            it = boss.bullets.erase(it); // Remove se sair da tela
        } else {
            ++it;
        }
    }
}