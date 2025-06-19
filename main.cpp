#define NOMINMAX

#include <iostream>
#include <windows.h>
#include <conio.h>
#include <chrono>
#include <cstdlib>
#include <ctime>
#include <algorithm>
#include <vector>

#include "mapa/mapa.h"
#include "enemy/enemy.h"
#include "logic/logic.h"
#include "menu/menu.h"
#include "ranking/score.h"
#include "utils/cleanScreen/cleanScreen.h"
#include "utils/constants.h"

using namespace std;
using namespace std::chrono;

char mapa[ALTURA_MAPA][LARGURA_MAPA];
int naveX = LARGURA_MAPA / 2;

bool inimigoVivo[ENEMY_ARRAY_MAX_SIZE];
int inimigos[ENEMY_ARRAY_MAX_SIZE][2];

std::vector<std::pair<int, int>> playerBullets; // {x, y} for each bullet

int score = 0;
bool gameOver = false;

bool tiroInimigoAtivo = false;
int tiroInimigoX = 0, tiroInimigoY = 0;

int vidas = 3;

bool itemDropActive = false;
int itemDropX = 0, itemDropY = 0;
ItemType itemDropType = ItemType::EXTRA_LIFE; // Default, will be randomized

// Power-up state variables
int maxPlayerBulletsAllowed = 1; // Normal limit is 1, increased by EXTRA_SHOT
bool multiShotActive = false;

std::chrono::high_resolution_clock::time_point speedBoostEndTime;
std::chrono::high_resolution_clock::time_point extraShotEndTime;
std::chrono::high_resolution_clock::time_point multiShotEndTime;
std::chrono::high_resolution_clock::time_point enemyFreezeEndTime;

// This will be used to revert enemy speed after a speed boost or freeze
std::chrono::milliseconds originalEnemyMoveIntervalMs = INITIAL_ENEMY_MOVE_INTERVAL;
 
const int FPS = 30;
const std::chrono::milliseconds frameDuration(1000 / FPS);

void input() {
    if (_kbhit()) {
        char tecla = _getch();
        if (tecla == 'a' || tecla == 'A') {
            if (naveX > 0) naveX--;
        }
        else if (tecla == 'd' || tecla == 'D') {
            if (naveX < LARGURA_MAPA - 1) naveX++;
        }
        else if (tecla == ' ') {
            if (playerBullets.size() < static_cast<size_t>(maxPlayerBulletsAllowed)) {
                if (multiShotActive) {
                    // Fire 3 bullets: center, left, right if possible
                    playerBullets.push_back({naveX, ALTURA_MAPA - 2});
                    if (naveX > 0) playerBullets.push_back({naveX - 1, ALTURA_MAPA - 2});
                    if (naveX < LARGURA_MAPA - 1) playerBullets.push_back({naveX + 1, ALTURA_MAPA - 2});
                    // Optional: Sort and unique to prevent exact overlaps if naveX is near edge and fires multiple
                    std::sort(playerBullets.begin(), playerBullets.end());
                    playerBullets.erase(std::unique(playerBullets.begin(), playerBullets.end()), playerBullets.end());
                } else {
                    // Fire single bullet
                    playerBullets.push_back({naveX, ALTURA_MAPA - 2});
                }
            }
        }
    }
}

void game(){
    gameOver = false;
    playerBullets.clear();
    score = 0;
    naveX = LARGURA_MAPA / 2;
    vidas = 3;

    itemDropActive = false;
    itemDropType = ItemType::EXTRA_LIFE; // Reset

    enemiesDefeatedCount = 0;
    enemyMoveInterval = INITIAL_ENEMY_MOVE_INTERVAL;
    originalEnemyMoveIntervalMs = INITIAL_ENEMY_MOVE_INTERVAL;

    // Reset power-up states for a new game
    maxPlayerBulletsAllowed = 1;
    multiShotActive = false;
    auto now_for_init = high_resolution_clock::now(); // Initialize timers to a state indicating they are not active
    speedBoostEndTime = now_for_init;
    extraShotEndTime = now_for_init;
    multiShotEndTime = now_for_init;
    enemyFreezeEndTime = now_for_init;

    initEnemy();

    system("cls");
    string nome;
    cout << "Digite seu nome: ";
    cin >> nome;
    cin.ignore();

    auto gameStartTime = high_resolution_clock::now();
    auto lastFrameTime = high_resolution_clock::now();
    auto lastEnemyMoveTime = high_resolution_clock::now();
    auto lastEnemyShotTime = high_resolution_clock::now();
    auto lastItemDropMoveTime = high_resolution_clock::now();

    const std::chrono::milliseconds enemyShotInterval(800);

    while (!gameOver) {
        auto now = high_resolution_clock::now();
        duration<float> elapsedTime = now - gameStartTime;
        float tempoDecorrido = elapsedTime.count();

        // Check and manage active power-up timers
        if (multiShotActive && now >= multiShotEndTime) {
            multiShotActive = false;
        }
        if (maxPlayerBulletsAllowed > 1 && now >= extraShotEndTime) {
            maxPlayerBulletsAllowed = 1; // Revert to single bullet capacity
        }
        // Speed boost for player bullets doesn't have a direct timer here, it affects bullet update speed.
        // Enemy freeze is checked in enemy movement logic.
        // If "Aumento de Velocidade" meant faster enemy movement (a debuff pickup), we'd manage it here too.
        // For now, assuming "Aumento de Velocidade" is player bullet speed (handled in updatePlayerBullets) or player movement (not implemented yet).
        // If it was enemy speed change:
        // if (enemyMoveInterval != originalEnemyMoveIntervalMs && now >= speedBoostEndTime) { // Assuming speedBoostEndTime was for enemy speed
        //     enemyMoveInterval = originalEnemyMoveIntervalMs;
        // }


        if (now - lastFrameTime >= frameDuration) {
            cleanScreen();
            render(score, tempoDecorrido, vidas);
            input();
            updatePlayerBullets(); // Updated from updateTire
            updateTiroInimigo();
            checkCollisions();
            checkEndOfGame();
            lastFrameTime = now;
        }

        if (now - lastEnemyMoveTime >= enemyMoveInterval) {
            // moveEnemies will check for freeze internally
            moveEnemies(); 
            lastEnemyMoveTime = now;
        }

        if (!tiroInimigoAtivo && now - lastEnemyShotTime >= enemyShotInterval) {
            updateTiroInimigo();
            lastEnemyShotTime = now;
        }

        if (itemDropActive && now - lastItemDropMoveTime >= ITEM_MOVE_INTERVAL) {
            updateItemDrop();
            lastItemDropMoveTime = now;
        }

        while (high_resolution_clock::now() - lastFrameTime < frameDuration) {
        }
    }
    system("cls");
    auto gameEndTime = high_resolution_clock::now();
    duration<float> finalDuration = gameEndTime - gameStartTime;

    resetColor();
    cout << "\nScore final: " << score << endl;

    saveScore(nome, score, finalDuration.count());

    cout << "\nPressione ENTER para voltar ao menu...";
    cin.get();
    system("cls");
}

int main() {
    srand(time(NULL));
    system("cls");
    hideCursor(); // Ensure cursor is hidden

    while (true) {
        menu();
        game();
    }

    return 0;
}