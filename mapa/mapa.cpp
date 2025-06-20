#include "mapa.h"
#include "../utils/gameElements.h"
#include "../logic/logic.h"
#include "../utils/constants.h"
#include <iostream>
#include <windows.h>
#include <iomanip>
#include <string>
#include <sstream>
#include <chrono>

using namespace std;
using namespace std::chrono;

// Presumindo que ShipConfig e chosenShipConfig são declarados externamente
// e acessíveis aqui, conforme configurado anteriormente (em mapa.h e main.cpp)
extern ShipConfig chosenShipConfig; 

GameElements gameIcons;
HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);

void render(int score, float tempo, int currentVidas)
{
    auto now = high_resolution_clock::now();

    cout << gameIcons.wall;
    for (int x = 0; x < LARGURA_MAPA; x++)
        cout << gameIcons.wall;
    cout << gameIcons.wall << "\n";

    stringstream ssScore;
    ssScore << "Score: " << score;
    string scoreStr = ssScore.str();

    stringstream ssTempo;
    ssTempo << fixed << setprecision(1) << tempo << "s";
    string tempoStr = "Tempo: " + ssTempo.str();

    int occupiedLength = scoreStr.length() + tempoStr.length();
    int padding = LARGURA_MAPA - occupiedLength;

    cout << gameIcons.wall;
    SetConsoleTextAttribute(hConsole, 7);
    cout << scoreStr;

    for (int i = 0; i < padding; ++i) {
        cout << " ";
    }
    
    SetConsoleTextAttribute(hConsole, 7);
    cout << tempoStr;
    
    cout << gameIcons.wall << "\n";

    cout << gameIcons.wall;
    for (int x = 0; x < LARGURA_MAPA; x++)
        cout << gameIcons.wall;
    cout << gameIcons.wall << "\n";

    for (int y = 0; y < ALTURA_MAPA; y++)
    {
        for (int x = 0; x < LARGURA_MAPA; x++)
        {
            mapa[y][x] = gameIcons.path;
        }
    }

    for (int i = 0; i < TOTAL_INITIAL_ENEMIES; i++)
    {
        if (inimigoVivo[i])
        {
            mapa[inimigos[i][1]][inimigos[i][0]] = gameIcons.enemy;
        }
    }

    // Draw player bullets
    for (const auto& bullet : playerBullets) {
        if (bullet.second >= 0 && bullet.second < ALTURA_MAPA && bullet.first >=0 && bullet.first < LARGURA_MAPA)
            mapa[bullet.second][bullet.first] = chosenShipConfig.bulletChar; // Usa o caractere de tiro da nave escolhida
    }

    if (tiroInimigoAtivo && tiroInimigoY >= 0 && tiroInimigoY < ALTURA_MAPA)
        mapa[tiroInimigoY][tiroInimigoX] = '|';

    // --- Renderiza a skin da nave escolhida ---
    if (!chosenShipConfig.skin.empty()) { // Verifica se a skin não está vazia
        int skinLength = chosenShipConfig.skin.length();
        int startX = naveX - skinLength / 2; 

        for (int i = 0; i < skinLength; ++i) {
            int currentX = startX + i;
            if (currentX >= 0 && currentX < LARGURA_MAPA) {
                mapa[ALTURA_MAPA - 1][currentX] = chosenShipConfig.skin[i];
            }
        }
    } // --- Fim da renderização da skin da nave ---

    if (explosionActiveEnemy) {
        if (now - explosionStartTime < EXPLOSION_DURATION) {
            mapa[explosionEnemyY][explosionEnemyX] = 'X';
        } else {
            explosionActiveEnemy = false;
        }
    }

    if (explosionActivePlayer) {
        if (now - explosionStartTime < EXPLOSION_DURATION) {
            mapa[explosionPlayerY][explosionPlayerX] = '@';
        } else {
            explosionActivePlayer = false;
        }
    }

    if (itemDropActive && itemDropY < ALTURA_MAPA && itemDropX < LARGURA_MAPA) {
        char itemChar = '?';
        // Colors can be associated with item types for better visual feedback
        switch (itemDropType) {
            case ItemType::EXTRA_LIFE: itemChar = 'L'; break; // Life
            case ItemType::SPEED_BOOST: itemChar = 'S'; break; // Speed
            case ItemType::EXTRA_SHOT: itemChar = 'E'; break; // Extra shot
            case ItemType::MULTI_SHOT: itemChar = 'M'; break; // Multi shot
            case ItemType::BONUS_POINTS: itemChar = '$'; break; // Points
            case ItemType::ENEMY_FREEZE: itemChar = 'F'; break; // Freeze
            default: itemChar = '+'; break;
        }
        if (itemDropX >= 0 && itemDropX < LARGURA_MAPA && itemDropY >=0 && itemDropY < ALTURA_MAPA)
            mapa[itemDropY][itemDropX] = itemChar;
    }

    // Clear explosion if duration passed (moved here for clarity, can be in logic)
    if (explosionActiveEnemy && now - explosionStartTime >= EXPLOSION_DURATION) {
        explosionActiveEnemy = false;
    }
    if (explosionActivePlayer && now - explosionStartTime >= EXPLOSION_DURATION) {
        explosionActivePlayer = false;
    }

    for (int y = 0; y < ALTURA_MAPA; y++)
    {
        cout << gameIcons.wall;
        for (int x = 0; x < LARGURA_MAPA; x++)
        {
            char c = mapa[y][x];
            
            // A verificação da nave do jogador deve vir primeiro, pois se baseia na posição, não apenas no caractere.
            bool isPlayerShipChar = false;
            if (y == ALTURA_MAPA - 1 && !chosenShipConfig.skin.empty()) {
                int skinLength = chosenShipConfig.skin.length();
                int startX = naveX - skinLength / 2;
                if (x >= startX && x < startX + skinLength) {
                    isPlayerShipChar = true;
                }
            }
            
            // Lógica de coloração unificada para evitar que uma cor sobrescreva a outra.
            if (isPlayerShipChar) {
                SetConsoleTextAttribute(hConsole, gameIcons.spaceshipColor);
            } else if (c == gameIcons.enemy) {
                SetConsoleTextAttribute(hConsole, gameIcons.enemyColor); // Agora esta cor será aplicada corretamente.
            } else if (c == chosenShipConfig.bulletChar) {
                SetConsoleTextAttribute(hConsole, chosenShipConfig.bulletColor);
            } else if (c == '|') {
                SetConsoleTextAttribute(hConsole, 12); // Tiro do inimigo (Vermelho)
            } else if (c == 'X' || c == '@') {
                SetConsoleTextAttribute(hConsole, 6);  // Explosão (Amarelo escuro/Laranja)
            } else if (c == 'L') {
                SetConsoleTextAttribute(hConsole, 10); // Vida (Verde)
            } else if (c == 'S') {
                SetConsoleTextAttribute(hConsole, 14); // Velocidade (Amarelo)
            } else if (c == '$') {
                SetConsoleTextAttribute(hConsole, 12); // Pontos (Vermelho claro)
            } else if (c == 'F') {
                SetConsoleTextAttribute(hConsole, 3);  // Freeze (Ciano)
            } else if (c == 'E' || c == 'M' || c == '+' || c == '?') {
                SetConsoleTextAttribute(hConsole, 11); // Outros itens (Ciano claro)
            }
            else
            {
                SetConsoleTextAttribute(hConsole, 7); // Cor padrão para o caminho/fundo
            }

            cout << c;
        }
        SetConsoleTextAttribute(hConsole, 7);
        cout << gameIcons.wall << "\n";
    }

    cout << gameIcons.wall;
    for (int x = 0; x < LARGURA_MAPA; x++)
        cout << gameIcons.wall;
    cout << gameIcons.wall << "\n";

    // Display active power-ups
    cout << gameIcons.wall;
    SetConsoleTextAttribute(hConsole, 7); // Default color
    string activeItemsStatus = "Ativos: ";
    bool hasActiveItem = false;

    // Salva a cor padrão para restaurar
    CONSOLE_SCREEN_BUFFER_INFO consoleInfo;
    GetConsoleScreenBufferInfo(hConsole, &consoleInfo);
    WORD originalColors = consoleInfo.wAttributes;

    cout << activeItemsStatus; // Imprime "Ativos: "

    if (std::chrono::duration_cast<std::chrono::seconds>(speedBoostEndTime - now).count() > 0) {
        SetConsoleTextAttribute(hConsole, 14); // Amarelo Claro para Velocidade
        cout << "[Velocidade:" << std::chrono::duration_cast<std::chrono::seconds>(speedBoostEndTime - now).count() << "s] ";
        hasActiveItem = true;
        SetConsoleTextAttribute(hConsole, originalColors); // Restaura cor
    }
    // Verifica se o tiro extra está ativo por um power-up (não pela configuração base da nave)
    bool extraShotPowerUpActive = (maxPlayerBulletsAllowed > chosenShipConfig.initialMaxBullets || (chosenShipConfig.initialMaxBullets > 1 && chosenShipConfig.type != ShipType::TYPE_2_BALANCED_EXTRA /*Exemplo, ajuste se necessário*/)) &&
                                  std::chrono::duration_cast<std::chrono::seconds>(extraShotEndTime - now).count() > 0;
    if (extraShotPowerUpActive) {
        SetConsoleTextAttribute(hConsole, 11); // Ciano Claro para Tiro Extra
        cout << "[Tiro Extra:" << std::chrono::duration_cast<std::chrono::seconds>(extraShotEndTime - now).count() << "s (" << maxPlayerBulletsAllowed << ")] ";
        hasActiveItem = true;
        SetConsoleTextAttribute(hConsole, originalColors);
    }
    // Verifica se o tiro múltiplo está ativo por um power-up (não pela configuração base da nave)
    bool multiShotPowerUpActive = multiShotActive && !chosenShipConfig.initialMultiShotActive &&
                                 std::chrono::duration_cast<std::chrono::seconds>(multiShotEndTime - now).count() > 0;
    if (multiShotPowerUpActive) {
        SetConsoleTextAttribute(hConsole, 13); // Roxo Claro para Tiro Múltiplo
        cout << "[Tiro Multi:" << std::chrono::duration_cast<std::chrono::seconds>(multiShotEndTime - now).count() << "s] ";
        hasActiveItem = true;
        SetConsoleTextAttribute(hConsole, originalColors);
    }
    if (std::chrono::duration_cast<std::chrono::seconds>(enemyFreezeEndTime - now).count() > 0) {
        SetConsoleTextAttribute(hConsole, 9);  // Azul Claro para Congelamento
        cout << "[Congelado:" << std::chrono::duration_cast<std::chrono::seconds>(enemyFreezeEndTime - now).count() << "s] ";
        hasActiveItem = true;
        SetConsoleTextAttribute(hConsole, originalColors);
    }

    if (!hasActiveItem) {
        cout << "Nenhum";
    }

    // Calcula o espaço restante na linha de status para preenchimento
    // Esta parte é um pouco mais complexa porque o cout já aconteceu.
    // Para um alinhamento perfeito, seria melhor construir toda a string de status primeiro.
    // Por simplicidade, vamos apenas garantir que não exceda a largura.
    // O ideal seria usar std::ostringstream para construir a linha de status completa e depois imprimir.
    // Para agora, o preenchimento será após a última mensagem de status.
    int currentConsolePos = 0; // Precisaria de GetConsoleCursorPosition ou similar
                               // Ou, mais simples, calcular o comprimento do que foi impresso.
    // Deixando o preenchimento como estava, mas a colorização é feita por partes.
    // A linha abaixo que usa activeItemsStatus.substr() não vai funcionar bem com a impressão colorida por partes.
    // Removendo o preenchimento complexo por enquanto, focando nas cores.
    // cout << left << setw(LARGURA_MAPA - activeItemsStatus.length()) << ""; // Preenchimento
    cout << gameIcons.wall << "\n";

    cout << gameIcons.wall;
    SetConsoleTextAttribute(hConsole, 7);
    cout << "Vidas: ";

    SetConsoleTextAttribute(hConsole, 12); // Vermelho claro para vidas (corações/símbolos)
    for (int i = 0; i < currentVidas; ++i) {
        cout << char(254) << " "; // char(254) é um bloco '■'. char(3) é '♥' em CP437.
    }
    // Calcula o comprimento do conteúdo das vidas para preenchimento
    // (considerando "Vidas: " e cada vida como "■ ")
    int vidasContentLength = static_cast<int>(string("Vidas: ").length()) + (currentVidas * 2);
    int remainingSpaceForLives = LARGURA_MAPA - vidasContentLength;
    for (int i = 0; i < remainingSpaceForLives; ++i) {
        cout << " ";
    }
    SetConsoleTextAttribute(hConsole, 7);
    cout << gameIcons.wall << "\n";

    cout << gameIcons.wall;
    for (int x = 0; x < LARGURA_MAPA; x++)
        cout << gameIcons.wall;
    cout << gameIcons.wall << "\n";

    SetConsoleTextAttribute(hConsole, 7);
}