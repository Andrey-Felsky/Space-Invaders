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
#include <iomanip> // Adicionado para std::setw e std::left

#include "player/player.h"
#include "mapa/mapa.h"
#include "enemy/enemy.h"
#include "logic/logic.h"
#include "menu/menu.h"
#include "ranking/score.h"
#include "utils/cleanScreen/cleanScreen.h"
#include "ui/ui.h" // Inclui as novas telas de vitoria/derrota
#include "utils/constants.h"
#include "utils/console_utils.h"

using namespace std;
using namespace std::chrono;

// Definição global das naves disponíveis para ser acessível em toda a aplicação
const std::vector<ShipConfig> AllShipOptions = {
    {ShipType::TYPE_1_FAST_SINGLE, "Nave Agil", "Um tiro por vez, se movimenta muito rapido.", std::chrono::milliseconds(50), 1, "-^-", 10, '\'', false},
    {ShipType::TYPE_2_BALANCED_EXTRA, "Nave Tática", "Um pouco mais lenta, mas com capacidade para Tiro Extra.", std::chrono::milliseconds(100), 2, "-O-", 11, '*', false},
    {ShipType::TYPE_3_BALANCED_MULTI, "Nave Destruidora", "Um pouco mais lenta, mas dispara um Tiro Multiplo.", std::chrono::milliseconds(100), 1, "-W-", 14, 'o', true}
};

// As definições de ShipType e ShipConfig agora vêm de "mapa/mapa.h"
// que é incluído acima.

// Barrier state
Player players[2];

ShipConfig chosenShipConfig; // Guarda a configuração da nave selecionada pelo jogador

GameMode currentGameMode = GameMode::TWO_PLAYER; // Default, will be set by menu
Difficulty currentDifficulty = Difficulty::NORMAL; // Dificuldade padrão
int currentItemDropChance = BASE_ITEM_DROP_CHANCE;
std::chrono::milliseconds currentEnemyMoveInterval = BASE_ENEMY_MOVE_INTERVAL;

char mapa[ALTURA_MAPA][LARGURA_MAPA];
std::chrono::milliseconds enemyMoveInterval; // Velocidade atual dos inimigos, modificada durante o jogo

bool inimigoVivo[ENEMY_ARRAY_MAX_SIZE];
int inimigos[ENEMY_ARRAY_MAX_SIZE][2];

bool gameOver = false;
bool playerWon = false; // Flag para determinar a tela final

bool tiroInimigoAtivo = false;
int tiroInimigoX = 0, tiroInimigoY = 0;

bool itemDropActive = false;
int itemDropX = 0, itemDropY = 0;
ItemType itemDropType = ItemType::EXTRA_LIFE; // Default, will be randomized

std::chrono::high_resolution_clock::time_point speedBoostEndTime;
std::chrono::high_resolution_clock::time_point extraShotEndTime;
std::chrono::high_resolution_clock::time_point multiShotEndTime;
std::chrono::high_resolution_clock::time_point enemyFreezeEndTime;

// Explosion state variables
bool explosionActiveEnemy = false;
int explosionEnemyX = 0, explosionEnemyY = 0;
std::chrono::high_resolution_clock::time_point enemyExplosionStartTime;

int enemiesDefeatedCount = 0;

// This will be used to revert enemy speed after a speed boost or freeze
std::chrono::milliseconds originalEnemyMoveIntervalMs; // Será definido com base na dificuldade
 
// Boss state
Boss boss;
bool bossFightActive = false;

std::chrono::high_resolution_clock::time_point lastPlayerMoveTime; // Para controlar o cooldown de movimento do jogador
const int FPS = 30;
const std::chrono::milliseconds frameDuration(1000 / FPS);

void autoPlayInput(Player& botPlayer) {
    auto now = high_resolution_clock::now();

    // A IA também precisa respeitar o cooldown de movimento da nave
    if (duration_cast<milliseconds>(now - botPlayer.lastMoveTime) < botPlayer.shipConfig.moveCooldown) {
        return; // Muito cedo para se mover, sai da função
    }

    // --- Lógica de Decisão da IA ---

    // Prioridade 1: Desviar de tiros inimigos
    if (tiroInimigoAtivo && tiroInimigoY >= ALTURA_MAPA / 2) { // Só se preocupa com tiros na metade inferior do mapa
        // Se o tiro está vindo diretamente ou muito perto
        if (abs(tiroInimigoX - botPlayer.x) <= 1) {
            // Tenta mover para a direita, se não puder, move para a esquerda.
            if (botPlayer.x < LARGURA_MAPA - 1) {
                botPlayer.x++;
            } else {
                botPlayer.x--;
            }
            botPlayer.lastMoveTime = now;
            return; // Decisão tomada, sai da função
        }
    }

    // Prioridade 2: Coletar itens
    if (itemDropActive) {
        // Move em direção ao item se não estiver alinhado
        if (itemDropX > botPlayer.x && botPlayer.x < LARGURA_MAPA - 1) {
            botPlayer.x++;
            botPlayer.lastMoveTime = now;
            return; // Ação do frame: mover para o item.
        } else if (itemDropX < botPlayer.x && botPlayer.x > 0) {
            botPlayer.x--;
            botPlayer.lastMoveTime = now;
            return; // Ação do frame: mover para o item.
        }
        // Se já estiver alinhado com o item, a IA pode prosseguir para a lógica de ataque.
    }

    // Prioridade 3: Atacar inimigos
    // Encontra o inimigo vivo mais próximo na linha de frente (mais abaixo)
    int targetX = -1, targetY = -1;
    for (int i = 0; i < ENEMY_ARRAY_MAX_SIZE; ++i) {
        if (inimigoVivo[i]) {
            if (inimigos[i][1] > targetY) { // inimigos[i][1] é a coordenada Y
                targetY = inimigos[i][1];
                targetX = inimigos[i][0]; // inimigos[i][0] é a coordenada X
            }
        }
    }

    if (targetX != -1) { // Se encontrou um alvo
        // Move para alinhar com o alvo
        if (targetX > botPlayer.x && botPlayer.x < LARGURA_MAPA - 1) {
            botPlayer.x++;
            botPlayer.lastMoveTime = now;
        } else if (targetX < botPlayer.x && botPlayer.x > 0) {
            botPlayer.x--;
            botPlayer.lastMoveTime = now;
        } else { // Se já está alinhado (targetX == naveX)
            // Atira! (copiado da lógica de input do jogador)
            if (botPlayer.bullets.size() < static_cast<size_t>(botPlayer.maxBulletsAllowed)) { // Verifica o limite de balas
                if (botPlayer.multiShotActive) {
                    // Dispara 3 balas: centro, esquerda, direita (se possível)
                    botPlayer.bullets.push_back({botPlayer.x, ALTURA_MAPA - 2});
                    if (botPlayer.x > 0) botPlayer.bullets.push_back({botPlayer.x - 1, ALTURA_MAPA - 2});
                    if (botPlayer.x < LARGURA_MAPA - 1) botPlayer.bullets.push_back({botPlayer.x + 1, ALTURA_MAPA - 2});
                } else {
                    // Dispara uma única bala
                    botPlayer.bullets.push_back({botPlayer.x, ALTURA_MAPA - 2});
                }
            }
        }
    }
}

void input(Player& p1, Player& p2) {
    if (_kbhit()) {
        char tecla = _getch();
        auto now = high_resolution_clock::now(); // Pega o tempo atual para checar cooldown

        // --- Player 1 Controls (A, D, Space) ---
        if ((tecla == 'a' || tecla == 'A') && p1.vidas > 0) {
            if (duration_cast<milliseconds>(now - p1.lastMoveTime) >= p1.shipConfig.moveCooldown) {
                if (p1.x > 0) p1.x--;
                p1.lastMoveTime = now;
            }
        }
        else if ((tecla == 'd' || tecla == 'D') && p1.vidas > 0) {
            if (duration_cast<milliseconds>(now - p1.lastMoveTime) >= p1.shipConfig.moveCooldown) {
                if (p1.x < LARGURA_MAPA - 1) p1.x++;
                p1.lastMoveTime = now;
            }
        }
        else if (tecla == ' ' && p1.vidas > 0) {
            if (p1.bullets.size() < static_cast<size_t>(p1.maxBulletsAllowed)) {
                if (p1.multiShotActive) {
                    // Dispara 3 balas: centro, esquerda, direita (se possível)
                    p1.bullets.push_back({p1.x, ALTURA_MAPA - 2});
                    if (p1.x > 0) p1.bullets.push_back({p1.x - 1, ALTURA_MAPA - 2});
                    if (p1.x < LARGURA_MAPA - 1) p1.bullets.push_back({p1.x + 1, ALTURA_MAPA - 2});
                } else {
                    // Dispara uma única bala
                    p1.bullets.push_back({p1.x, ALTURA_MAPA - 2});
                }
            }
        }

        // --- Player 2 Controls (Arrow Keys) ---
        else if (tecla == 0 || tecla == -32) { // Código especial para teclas de seta
            tecla = _getch(); // Pega o código real da tecla, consumindo-o do buffer

            // Apenas processa as teclas do P2 se não for um bot
            if (!(currentGameMode == GameMode::TWO_PLAYER && currentDifficulty == Difficulty::AUTO)) {
                if (tecla == 75 && p2.vidas > 0) { // Seta para Esquerda
                    if (duration_cast<milliseconds>(now - p2.lastMoveTime) >= p2.shipConfig.moveCooldown) {
                        if (p2.x > 0) p2.x--;
                        p2.lastMoveTime = now;
                    }
                }
                else if (tecla == 77 && p2.vidas > 0) { // Seta para Direita
                    if (duration_cast<milliseconds>(now - p2.lastMoveTime) >= p2.shipConfig.moveCooldown) {
                        if (p2.x < LARGURA_MAPA - 1) p2.x++;
                        p2.lastMoveTime = now;
                    }
                }
                else if (tecla == 72 && p2.vidas > 0) { // Seta para Cima (Atirar)
                    if (p2.bullets.size() < static_cast<size_t>(p2.maxBulletsAllowed)) {
                        if (p2.multiShotActive) {
                            // Dispara 3 balas: centro, esquerda, direita (se possível)
                            p2.bullets.push_back({p2.x, ALTURA_MAPA - 2});
                            if (p2.x > 0) p2.bullets.push_back({p2.x - 1, ALTURA_MAPA - 2});
                            if (p2.x < LARGURA_MAPA - 1) p2.bullets.push_back({p2.x + 1, ALTURA_MAPA - 2});
                        } else {
                            // Dispara uma única bala
                            p2.bullets.push_back({p2.x, ALTURA_MAPA - 2});
                        }
                    }
                }
            }
        }
    }
}

// Função auxiliar para imprimir texto centralizado dentro de uma largura específica
void printCentered(const std::string& text, int width) {
    int padding_left = (width - text.length()) / 2;
    if (padding_left < 0) padding_left = 0;
    int padding_right = width - text.length() - padding_left;
    if (padding_right < 0) padding_right = 0;
    std::cout << std::string(padding_left, ' ') << text << std::string(padding_right, ' ');
}

void selectShip(int playerIndex) {
    hideCursor();

    // Definir as configurações das naves
    const std::vector<ShipConfig>& shipOptions = AllShipOptions;

    int selected_option = 0;
    int prev_selected_option = -1; // Para detectar mudança na seleção
    char key;

    const int menuWidth = 58; // Largura da moldura do menu
    // Centraliza a moldura na tela (assumindo uma tela de 80 colunas)
    const int menuStartX = (80 - menuWidth) / 2;

    while (true) {
        // Redesenha apenas se a seleção mudou
        setCursorPosition(0, 0);
        resetConsoleColor();
        cout << "      JOGADOR " << playerIndex + 1 << ", ESCOLHA SUA NAVE!      " << endl;

        if (selected_option != prev_selected_option) {
            const ShipConfig& currentShip = shipOptions[selected_option];
            cleanScreen();

            // --- Desenha a Moldura Superior e o Título ---
            setCursorPosition(menuStartX, 2);
            std::cout << (char)201 << std::string(menuWidth, (char)205) << (char)187;
            setCursorPosition(menuStartX, 3);
            std::cout << (char)186;
            printCentered("SELECIONE SUA NAVE", menuWidth);
            std::cout << (char)186;
            setCursorPosition(menuStartX, 4);
            std::cout << (char)186 << std::string(menuWidth, ' ') << (char)186;

            // --- Desenha a Pré-visualização da Nave ---
            setCursorPosition(menuStartX, 5);
            std::cout << (char)186;
            setConsoleColor(currentShip.bulletColor);
            printCentered(currentShip.skin, menuWidth);
            resetConsoleColor();
            std::cout << (char)186;
            setCursorPosition(menuStartX, 6);
            std::cout << (char)186;
            setConsoleColor(currentShip.bulletColor);
            printCentered(std::string(1, currentShip.bulletChar), menuWidth);
            resetConsoleColor();
            std::cout << (char)186;
            setCursorPosition(menuStartX, 7);
            std::cout << (char)186 << std::string(menuWidth, ' ') << (char)186;

            // --- Desenha as Opções de Naves ---
            for (size_t i = 0; i < shipOptions.size(); ++i) {
                setCursorPosition(menuStartX, 8 + i);
                std::cout << (char)186;
                if (i == (size_t)selected_option) {
                    setConsoleColor(14); // Amarelo para selecionado
                    printCentered("> " + shipOptions[i].name + " <", menuWidth);
                } else {
                    resetConsoleColor(); // Branco para outros
                    printCentered(shipOptions[i].name, menuWidth);
                }
                resetConsoleColor();
                std::cout << (char)186;
            }
            setCursorPosition(menuStartX, 8 + shipOptions.size());
            std::cout << (char)186 << std::string(menuWidth, ' ') << (char)186;

            // --- Desenha a Moldura Inferior com a Descrição ---
            setCursorPosition(menuStartX, 9 + shipOptions.size());
            std::cout << (char)204 << std::string(menuWidth, (char)205) << (char)185;
            setCursorPosition(menuStartX, 10 + shipOptions.size());
            std::cout << (char)186 << std::string(menuWidth, ' ') << (char)186;
            setCursorPosition(menuStartX, 11 + shipOptions.size());
            std::string desc_line = " " + currentShip.description;
            std::cout << (char)186; setConsoleColor(11); std::cout << std::left << std::setw(menuWidth) << desc_line; resetConsoleColor(); std::cout << (char)186;
            setCursorPosition(menuStartX, 12 + shipOptions.size());
            std::string attributes = std::string("Atributos: Movimento ") + (currentShip.moveCooldown.count() == 50 ? "Rapido" : "Normal") + " (" + std::to_string(currentShip.moveCooldown.count()) + "ms), " + (currentShip.initialMultiShotActive ? "Tiro Multiplo" : std::to_string(currentShip.initialMaxBullets) + " Tiro Max");
            std::string attr_line = " " + attributes;
            std::cout << (char)186; setConsoleColor(11); std::cout << std::left << std::setw(menuWidth) << attr_line; resetConsoleColor(); std::cout << (char)186;
            setCursorPosition(menuStartX, 13 + shipOptions.size());
            std::cout << (char)186 << std::string(menuWidth, ' ') << (char)186;
            setCursorPosition(menuStartX, 14 + shipOptions.size());
            std::cout << (char)200 << std::string(menuWidth, (char)205) << (char)188;

            // --- Desenha as Instruções ---
            setCursorPosition(menuStartX, 16 + shipOptions.size());
            std::cout << "Use as setas para navegar e ENTER para confirmar.";

            prev_selected_option = selected_option;
        }

        // Input handling
        if (_kbhit()) {
            key = _getch();
            if (key == 0 || key == -32) { // Arrow keys
                key = _getch(); // Get the second byte
                if (key == 72) { // Up arrow
                    Beep(600, 50); // Som de navegação
                    selected_option = (selected_option == 0) ? shipOptions.size() - 1 : selected_option - 1;
                } else if (key == 80) { // Down arrow
                    Beep(600, 50); // Som de navegação
                    selected_option = (selected_option + 1) % shipOptions.size();
                }
            } else if (key == 13) { // Enter key
                players[playerIndex].shipConfig = shipOptions[selected_option];
                Beep(900, 100); // Som de confirmação
                cleanScreen();
                resetConsoleColor(); // Garante que a cor seja resetada ao sair
                return; // Exit ship selection
            } else if (key == 27) { // ESC key
                chosenShipConfig = {ShipType::NONE, "", "", std::chrono::milliseconds(0), 0, "", 0, ' ', false}; // Set to NONE
                Beep(400, 80); // Som de cancelamento/volta
                cleanScreen();
                resetConsoleColor(); // Garante que a cor seja resetada ao sair
                return; 
            }
        }
        // Small delay to prevent high CPU usage
        Sleep(1);
    }
}

void initBarriers() {
    // Posiciona as barreiras de forma espaçada no mapa
    int barrierY = ALTURA_MAPA - 5;
    // Calcula o espaçamento para que as barreiras fiquem centralizadas
    const int GAP_BETWEEN_BARRIERS = 5; // Espaço fixo entre as barreiras
    int totalBarriersAndSpacing = NUM_BARRIERS * BARRIER_WIDTH + (NUM_BARRIERS - 1) * GAP_BETWEEN_BARRIERS;
    int startX = (LARGURA_MAPA - totalBarriersAndSpacing) / 2;

    for (int i = 0; i < NUM_BARRIERS; ++i) {
        barriers[i].x = startX + i * (BARRIER_WIDTH + GAP_BETWEEN_BARRIERS);
        barriers[i].y = barrierY;
        // Preenche a forma da barreira com um caractere sólido
        std::fill(&barriers[i].shape[0][0], &barriers[i].shape[0][0] + sizeof(barriers[i].shape), char(219));
    }
}

void game(){
    gameOver = false;
    playerWon = false; // Reseta a flag de vitoria
    bossFightActive = false; // Reseta o chefe
    
    // Initialize players
    players[0].init(1, LARGURA_MAPA / 4);
    players[1].init(2, (LARGURA_MAPA / 4) * 3);
    
    // Define os parâmetros do jogo com base na dificuldade selecionada
    switch (currentDifficulty) {
        case Difficulty::FACIL:
            currentItemDropChance = BASE_ITEM_DROP_CHANCE + 15; // 40%
            currentEnemyMoveInterval = BASE_ENEMY_MOVE_INTERVAL + std::chrono::milliseconds(150); // Mais lento
            break;
        case Difficulty::NORMAL:
            currentItemDropChance = BASE_ITEM_DROP_CHANCE; // 25%
            currentEnemyMoveInterval = BASE_ENEMY_MOVE_INTERVAL; // Normal
            break;
        case Difficulty::DIFICIL:
            currentItemDropChance = BASE_ITEM_DROP_CHANCE - 10; // 15%
            currentEnemyMoveInterval = BASE_ENEMY_MOVE_INTERVAL - std::chrono::milliseconds(150); // Mais rápido
            break;
        case Difficulty::AUTO:
            currentItemDropChance = BASE_ITEM_DROP_CHANCE + 25; // 50%, para a IA se divertir mais
            currentEnemyMoveInterval = BASE_ENEMY_MOVE_INTERVAL - std::chrono::milliseconds(150); // Inimigos rápidos para um bom desafio
            break;
    }

    // Aplica a configuração da nave escolhida
    for(int i = 0; i < 2; ++i) {
        players[i].maxBulletsAllowed = players[i].shipConfig.initialMaxBullets;
        players[i].multiShotActive = players[i].shipConfig.initialMultiShotActive;
        players[i].lastMoveTime = high_resolution_clock::now();
    }

    itemDropActive = false;
    itemDropType = ItemType::EXTRA_LIFE; // Reset

    enemiesDefeatedCount = 0; // Reseta a contagem de inimigos derrotados
    enemyMoveInterval = currentEnemyMoveInterval; // Define a velocidade inicial dos inimigos para esta partida
    originalEnemyMoveIntervalMs = currentEnemyMoveInterval; // Salva a velocidade base da dificuldade

    // Reseta os timers de power-ups para um estado que indica que não estão ativos
    auto now_for_init = high_resolution_clock::now(); 
    speedBoostEndTime = now_for_init;
    extraShotEndTime = now_for_init;
    multiShotEndTime = now_for_init;
    enemyFreezeEndTime = now_for_init;

    initEnemy();
    initBarriers();

    cleanScreen();
    string p1_name = "Jogador1";
    string p2_name = "Jogador2";
    if (currentDifficulty == Difficulty::AUTO) {
        if (currentGameMode == GameMode::TWO_PLAYER) {
            cout << "Digite o nome do Jogador 1: ";
            cin >> p1_name;
            p2_name = "Robo-Player";
            cout << "Modo AUTO (2P) ativado. Jogador 2 e " << p2_name << ".\n";
        } else { // SINGLE_PLAYER
            p1_name = "Robo-Player";
            cout << "Modo AUTO (1P) ativado. O jogador e " << p1_name << ".\n";
        }
        Sleep(2000); // Pausa para o usuário ler
        cin.ignore(numeric_limits<streamsize>::max(), '\n'); // Limpa o buffer de input
    } else { // Dificuldade não é AUTO
        if (currentGameMode == GameMode::SINGLE_PLAYER) {
            cout << "Digite seu nome: ";
            cin >> p1_name;
        } else { // TWO_PLAYER
            cout << "Digite o nome do Jogador 1: ";
            cin >> p1_name;
            cout << "Digite o nome do Jogador 2: ";
            cin >> p2_name;
        }
        cin.ignore();
    }
    auto gameStartTime = high_resolution_clock::now();
    auto lastFrameTime = high_resolution_clock::now();
    auto lastEnemyMoveTime = high_resolution_clock::now();
    auto lastItemDropMoveTime = high_resolution_clock::now();

    const std::chrono::milliseconds enemyShotInterval(800);

    while (!gameOver) {
        auto now = high_resolution_clock::now();
        duration<float> elapsedTime = now - gameStartTime;
        float tempoDecorrido = elapsedTime.count();

        // Check and manage active power-up timers for both players
        for(int i = 0; i < 2; ++i) {
            if (players[i].multiShotActive && players[i].multiShotActive != players[i].shipConfig.initialMultiShotActive && now >= players[i].multiShotEndTime) {
                players[i].multiShotActive = players[i].shipConfig.initialMultiShotActive;
            }
            if (players[i].maxBulletsAllowed > players[i].shipConfig.initialMaxBullets && now >= players[i].extraShotEndTime) {
                players[i].maxBulletsAllowed = players[i].shipConfig.initialMaxBullets;
            }
        }

        if (now - lastFrameTime >= frameDuration) {
            // --- LÓGICA DE UPDATE ---
            // A input() sempre escuta, mas a lógica interna agora previne o controle do bot
            input(players[0], players[1]);

            // A IA controla o jogador apropriado no modo AUTO
            if (currentDifficulty == Difficulty::AUTO) {
                if (currentGameMode == GameMode::SINGLE_PLAYER) {
                    autoPlayInput(players[0]); // Bot joga como P1
                } else { // TWO_PLAYER
                    autoPlayInput(players[1]); // Bot joga como P2
                }
            }
            updatePlayerBullets();
            updateTiroInimigo();
            if (bossFightActive) {
                updateBoss();
                updateBossBullets();
            }
            updateExplosions(); // Atualiza o estado das explosões
            checkCollisions();
            checkEndOfGame();

            // --- RENDERIZAÇÃO ---
            resetCursorPosition(); // Usa o reset de cursor para evitar piscar durante o jogo
            render(players, tempoDecorrido);
            lastFrameTime = now;
        }
        
        if (now - lastEnemyMoveTime >= enemyMoveInterval) {
            // moveEnemies will check for freeze internally
            moveEnemies(); 
            lastEnemyMoveTime = now;
        }

        if (itemDropActive && now - lastItemDropMoveTime >= ITEM_MOVE_INTERVAL) {
            updateItemDrop();
            lastItemDropMoveTime = now;
        }

        while (high_resolution_clock::now() - lastFrameTime < frameDuration) {
        }
    }

    auto gameEndTime = high_resolution_clock::now();
    duration<float> finalDuration = gameEndTime - gameStartTime;

    // Salva o score de ambos os jogadores
    if (currentGameMode == GameMode::SINGLE_PLAYER || currentDifficulty == Difficulty::AUTO) {
        saveScore(p1_name, players[0].score, finalDuration.count());
    } else { // TWO_PLAYER
        saveScore(p1_name, players[0].score, finalDuration.count());
        saveScore(p2_name, players[1].score, finalDuration.count());
    }

    // Exibe a tela de vitória ou derrota
    if (playerWon) {
        showVictoryScreen(players[0].score + players[1].score, finalDuration.count());
    } else {
        showDefeatScreen(players[0].score + players[1].score, finalDuration.count());
    }

    // Espera por input do usuário antes de voltar ao menu
    Sleep(1500); // Adiciona uma pausa de 1.5s antes de aceitar input
    cout << "Pressione qualquer tecla para voltar ao menu...";
    _getch(); // Usa _getch() para não precisar de ENTER
}

int main() {
    srand(time(NULL));
    hideCursor(); // Garante que o cursor está escondido

    // Inicializa com uma nave padrão. selectShip() irá sobrescrever isso.
    // Inicializa com NONE para indicar que nenhuma nave foi selecionada ainda.
    chosenShipConfig = {ShipType::NONE, "", "", std::chrono::milliseconds(0), 0, "", 0, ' ', false};

    while (true) { // Loop principal do jogo
        menu(); // Função do menu/menu.h.
                // Assume-se que se "Jogar" for escolhido, menu() retorna.
                // Se "Sair" for escolhido, menu() chama exit(0).

        // Ship selection for both players
        selectShip(0);

        // Only select for P2 if in two-player mode and P1 didn't press ESC
        if (currentGameMode == GameMode::TWO_PLAYER && players[0].shipConfig.type != ShipType::NONE) {
            if (currentDifficulty == Difficulty::AUTO) {
                // Assign a default balanced ship to the bot (P2)
                players[1].shipConfig = AllShipOptions[1]; // Assign "Nave Tática"
            } else {
                // Human P2 selects a ship
                selectShip(1);
            }
        }

        // Check if selection was completed (no ESC pressed)
        bool p1_selected = players[0].shipConfig.type != ShipType::NONE;
        bool p2_is_bot = (currentGameMode == GameMode::TWO_PLAYER && currentDifficulty == Difficulty::AUTO);
        bool p2_selected = (currentGameMode == GameMode::SINGLE_PLAYER || p2_is_bot) ? true : (players[1].shipConfig.type != ShipType::NONE);
        if (p1_selected && p2_selected) {
            game(); // Inicia o jogo com a(s) nave(s) escolhida(s).
        }
        // Se ESC foi pressionado em selectShip(), o loop volta para o menu principal.
    }

    return 0; // Inalcançável se menu() trata a saída.
}