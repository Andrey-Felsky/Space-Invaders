#define NOMINMAX

#include <iostream>
#include <windows.h>
#include <conio.h>
#include <chrono>
#include <cstdlib>
#include <ctime>
#include <algorithm>
#include <vector>
#include <string> // Adicionado para std::string em ShipConfig
#include <limits> // Adicionado para std::numeric_limits em selectShip

#include "mapa/mapa.h"
#include "enemy/enemy.h"
#include "logic/logic.h"
#include "menu/menu.h"
#include "ranking/score.h"
#include "utils/cleanScreen/cleanScreen.h"
#include "utils/constants.h"

using namespace std;
using namespace std::chrono;

// --- NOVAS DEFINIÇÕES E GLOBAIS PARA SELEÇÃO DE NAVE ---
enum class ShipType {
    TYPE_1_FAST_SINGLE,    // Nave 1: Rápida, 1 tiro
    TYPE_2_BALANCED_EXTRA, // Nave 2: Normal, Tiro Extra (2 balas na tela)
    TYPE_3_BALANCED_MULTI  // Nave 3: Normal, Tiro Múltiplo
};

struct ShipConfig {
    ShipType type;
    std::string name;
    std::string description;
    std::chrono::milliseconds moveCooldown; // Menor = mais rápido
    int initialMaxBullets;                  // Quantos tiros na tela por vez (ou "rajadas" para multi-tiro)
    bool initialMultiShotActive;            // Se o tiro múltiplo é padrão para esta nave
};

ShipConfig chosenShipConfig; // Guarda a configuração da nave selecionada pelo jogador
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
 
std::chrono::high_resolution_clock::time_point lastPlayerMoveTime; // Para controlar o cooldown de movimento do jogador

const int FPS = 30;
const std::chrono::milliseconds frameDuration(1000 / FPS);

void input() {
    if (_kbhit()) {
        char tecla = _getch();
        auto now = high_resolution_clock::now(); // Pega o tempo atual para checar cooldown

        if (tecla == 'a' || tecla == 'A') {
            if (duration_cast<milliseconds>(now - lastPlayerMoveTime) >= chosenShipConfig.moveCooldown) {
                if (naveX > 0) naveX--;
                lastPlayerMoveTime = now; // Atualiza o tempo do último movimento
            }
        }
        else if (tecla == 'd' || tecla == 'D') {
            if (duration_cast<milliseconds>(now - lastPlayerMoveTime) >= chosenShipConfig.moveCooldown) {
                if (naveX < LARGURA_MAPA - 1) naveX++;
                lastPlayerMoveTime = now; // Atualiza o tempo do último movimento
            }
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

void selectShip() {
    system("cls");
    cout << "Escolha sua Nave:\n\n";

    cout << "1. Nave Agil\n";
    cout << "   - Descricao: Um tiro por vez, se movimenta muito rapido.\n";
    cout << "   - Atributos: Movimento Rapido (50ms cooldown), 1 Tiro Max, Sem Tiro Multiplo.\n\n";

    cout << "2. Nave Tática\n";
    cout << "   - Descricao: Um pouco mais lenta, mas com capacidade para Tiro Extra.\n";
    cout << "   - Atributos: Movimento Normal (100ms cooldown), 2 Tiros Max, Sem Tiro Multiplo.\n\n";

    cout << "3. Nave Destruidora\n";
    cout << "   - Descricao: Um pouco mais lenta, mas dispara um Tiro Multiplo.\n";
    cout << "   - Atributos: Movimento Normal (100ms cooldown), 1 Rajada Max, Tiro Multiplo Ativo.\n\n";

    cout << "Escolha (1-3): ";

    char choice_char;
    cin >> choice_char;
    cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n'); // Limpa o buffer de entrada

    switch (choice_char) {
        case '1':
            chosenShipConfig = {ShipType::TYPE_1_FAST_SINGLE, "Nave Agil", "Um tiro por vez, se movimenta muito rapido.", std::chrono::milliseconds(50), 1, false};
            break;
        case '2':
            chosenShipConfig = {ShipType::TYPE_2_BALANCED_EXTRA, "Nave Tática", "Tiro Extra (2 balas na tela).", std::chrono::milliseconds(100), 2, false};
            break;
        case '3':
            chosenShipConfig = {ShipType::TYPE_3_BALANCED_MULTI, "Nave Destruidora", "Tiro Multiplo.", std::chrono::milliseconds(100), 1, true};
            break;
        default:
            cout << "Escolha invalida. Usando Nave Agil por padrao.\n";
            chosenShipConfig = {ShipType::TYPE_1_FAST_SINGLE, "Nave Agil", "Um tiro por vez, se movimenta muito rapido.", std::chrono::milliseconds(50), 1, false};
            Sleep(1500); // Dá tempo para o usuário ler a mensagem
            break;
    }
    system("cls");
}

void game(){
    gameOver = false;
    playerBullets.clear();
    score = 0;
    naveX = LARGURA_MAPA / 2;
    vidas = 3;

    // Aplica a configuração da nave escolhida
    maxPlayerBulletsAllowed = chosenShipConfig.initialMaxBullets;
    multiShotActive = chosenShipConfig.initialMultiShotActive;
    lastPlayerMoveTime = high_resolution_clock::now(); // Inicializa para o primeiro movimento

    itemDropActive = false;
    itemDropType = ItemType::EXTRA_LIFE; // Reset

    enemiesDefeatedCount = 0;
    enemyMoveInterval = INITIAL_ENEMY_MOVE_INTERVAL;
    originalEnemyMoveIntervalMs = INITIAL_ENEMY_MOVE_INTERVAL;

    // Reseta os timers de power-ups para um estado que indica que não estão ativos
    auto now_for_init = high_resolution_clock::now(); 
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
        // Se o multi-shot estava ativo por um power-up e o tempo expirou, reverte para o base da nave
        if (multiShotActive && multiShotActive != chosenShipConfig.initialMultiShotActive && now >= multiShotEndTime) {
            multiShotActive = chosenShipConfig.initialMultiShotActive;
        }
        // Se maxPlayerBulletsAllowed foi aumentado por um power-up e o tempo expirou, reverte para o base da nave
        if (maxPlayerBulletsAllowed > chosenShipConfig.initialMaxBullets && now >= extraShotEndTime) {
            maxPlayerBulletsAllowed = chosenShipConfig.initialMaxBullets;
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
    hideCursor(); // Garante que o cursor está escondido

    // Inicializa com uma nave padrão. selectShip() irá sobrescrever isso.
    // Necessário caso haja um caminho no menu que pule a seleção e vá direto para o jogo (improvável com a estrutura atual).
    chosenShipConfig = {ShipType::TYPE_1_FAST_SINGLE, "Nave Agil", "Um tiro por vez, se movimenta muito rapido.", std::chrono::milliseconds(50), 1, false};

    while (true) {
        menu(); // Função do menu/menu.h.
                // Assume-se que se "Jogar" for escolhido, menu() retorna.
                // Se "Sair" for escolhido, menu() chama exit(0).

        selectShip(); // Chama a seleção de nave APÓS o menu principal e ANTES do jogo.
        game();       // Inicia o jogo com a nave escolhida.
    }

    return 0; // Inalcançável se menu() trata a saída.
}