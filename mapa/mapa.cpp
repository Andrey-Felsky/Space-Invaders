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

    for (int i = 0; i < padding; ++i)
    {
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
    for (const auto &bullet : playerBullets)
    {
        if (bullet.second >= 0 && bullet.second < ALTURA_MAPA && bullet.first >= 0 && bullet.first < LARGURA_MAPA)
            mapa[bullet.second][bullet.first] = chosenShipConfig.bulletChar; // Usa o caractere de tiro da nave escolhida
    }

    if (tiroInimigoAtivo && tiroInimigoY >= 0 && tiroInimigoY < ALTURA_MAPA)
        mapa[tiroInimigoY][tiroInimigoX] = '|';

    // --- Renderiza a skin da nave escolhida ---
    if (!chosenShipConfig.skin.empty())
    { // Verifica se a skin não está vazia
        int skinLength = chosenShipConfig.skin.length();
        int startX = naveX - skinLength / 2;

        for (int i = 0; i < skinLength; ++i)
        {
            int currentX = startX + i;
            if (currentX >= 0 && currentX < LARGURA_MAPA)
            {
                mapa[ALTURA_MAPA - 1][currentX] = chosenShipConfig.skin[i];
            }
        }
    } // --- Fim da renderização da skin da nave ---

    if (explosionActiveEnemy)
    {
        if (now - explosionStartTime < EXPLOSION_DURATION)
        {
            mapa[explosionEnemyY][explosionEnemyX] = 'X';
        }
        else
        {
            explosionActiveEnemy = false;
        }
    }

    if (explosionActivePlayer)
    {
        if (now - explosionStartTime < EXPLOSION_DURATION)
        {
            mapa[explosionPlayerY][explosionPlayerX] = '@';
        }
        else
        {
            explosionActivePlayer = false;
        }
    }

    if (itemDropActive && itemDropY < ALTURA_MAPA && itemDropX < LARGURA_MAPA)
    {
        char itemChar = '?';
        // Colors can be associated with item types for better visual feedback
        switch (itemDropType)
        {
        case ItemType::EXTRA_LIFE:
            itemChar = 'L';
            break; // Life
        case ItemType::SPEED_BOOST:
            itemChar = 'S';
            break; // Speed
        case ItemType::EXTRA_SHOT:
            itemChar = 'E';
            break; // Extra shot
        case ItemType::MULTI_SHOT:
            itemChar = 'M';
            break; // Multi shot
        case ItemType::BONUS_POINTS:
            itemChar = '$';
            break; // Points
        case ItemType::ENEMY_FREEZE:
            itemChar = 'F';
            break; // Freeze
        default:
            itemChar = '+';
            break;
        }
        if (itemDropX >= 0 && itemDropX < LARGURA_MAPA && itemDropY >= 0 && itemDropY < ALTURA_MAPA)
            mapa[itemDropY][itemDropX] = itemChar;
    }

    // Clear explosion if duration passed (moved here for clarity, can be in logic)
    if (explosionActiveEnemy && now - explosionStartTime >= EXPLOSION_DURATION)
    {
        explosionActiveEnemy = false;
    }
    if (explosionActivePlayer && now - explosionStartTime >= EXPLOSION_DURATION)
    {
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
            if (y == ALTURA_MAPA - 1 && !chosenShipConfig.skin.empty())
            {
                int skinLength = chosenShipConfig.skin.length();
                int startX = naveX - skinLength / 2;
                if (x >= startX && x < startX + skinLength)
                {
                    isPlayerShipChar = true;
                }
            }

            // Lógica de coloração unificada para evitar que uma cor sobrescreva a outra.
            if (isPlayerShipChar)
            {
                SetConsoleTextAttribute(hConsole, gameIcons.spaceshipColor);
            }
            else if (c == gameIcons.enemy)
            {
                SetConsoleTextAttribute(hConsole, gameIcons.enemyColor); // Agora esta cor será aplicada corretamente.
            }
            else if (c == chosenShipConfig.bulletChar)
            {
                SetConsoleTextAttribute(hConsole, chosenShipConfig.bulletColor);
            }
            else if (c == '|')
            {
                SetConsoleTextAttribute(hConsole, 12); // Tiro do inimigo (Vermelho)
            }
            else if (c == 'X' || c == '@')
            {
                SetConsoleTextAttribute(hConsole, 6); // Explosão (Amarelo escuro/Laranja)
            }
            else if (c == 'L')
            {
                SetConsoleTextAttribute(hConsole, 10); // Vida (Verde)
            }
            else if (c == 'S')
            {
                SetConsoleTextAttribute(hConsole, 14); // Velocidade (Amarelo)
            }
            else if (c == '$')
            {
                SetConsoleTextAttribute(hConsole, 12); // Pontos (Vermelho claro)
            }
            else if (c == 'F')
            {
                SetConsoleTextAttribute(hConsole, 3); // Freeze (Ciano)
            }
            else if (c == 'E' || c == 'M' || c == '+' || c == '?')
            {
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

    // Salva a cor padrão para restaurar
    CONSOLE_SCREEN_BUFFER_INFO consoleInfo;
    GetConsoleScreenBufferInfo(hConsole, &consoleInfo);
    WORD originalColors = consoleInfo.wAttributes;

    // --- Linha de Status: Itens Ativos ---
    cout << gameIcons.wall;
    SetConsoleTextAttribute(hConsole, 7); // Cor padrão

    int line_length = 0;
    stringstream ss; // Usado para construir partes da string

    // Imprime o rótulo e conta seu tamanho
    string label = "Ativos: ";
    cout << label;
    line_length += label.length();

    bool hasActiveItem = false;

    // Verifica e imprime cada power-up ativo
    long speed_seconds = duration_cast<seconds>(speedBoostEndTime - now).count();
    if (speed_seconds > 0)
    {
        ss.str(""); // Limpa o stringstream
        ss << "[Velocidade:" << speed_seconds << "s] ";
        string item_str = ss.str();

        SetConsoleTextAttribute(hConsole, 14); // Amarelo
        cout << item_str;
        SetConsoleTextAttribute(hConsole, originalColors); // Restaura cor

        line_length += item_str.length();
        hasActiveItem = true;
    }

    bool extraShotPowerUpActive = (maxPlayerBulletsAllowed > chosenShipConfig.initialMaxBullets) &&
                                  duration_cast<seconds>(extraShotEndTime - now).count() > 0;
    if (extraShotPowerUpActive)
    {
        ss.str("");
        ss << "[Tiro Extra:" << duration_cast<seconds>(extraShotEndTime - now).count() << "s (" << maxPlayerBulletsAllowed << ")] ";
        string item_str = ss.str();

        SetConsoleTextAttribute(hConsole, 11); // Ciano Claro
        cout << item_str;
        SetConsoleTextAttribute(hConsole, originalColors);

        line_length += item_str.length();
        hasActiveItem = true;
    }

    bool multiShotPowerUpActive = multiShotActive && !chosenShipConfig.initialMultiShotActive &&
                                  duration_cast<seconds>(multiShotEndTime - now).count() > 0;
    if (multiShotPowerUpActive)
    {
        ss.str("");
        ss << "[Tiro Multi:" << duration_cast<seconds>(multiShotEndTime - now).count() << "s] ";
        string item_str = ss.str();

        SetConsoleTextAttribute(hConsole, 13); // Roxo Claro
        cout << item_str;
        SetConsoleTextAttribute(hConsole, originalColors);

        line_length += item_str.length();
        hasActiveItem = true;
    }

    long freeze_seconds = duration_cast<seconds>(enemyFreezeEndTime - now).count();
    if (freeze_seconds > 0)
    {
        ss.str("");
        ss << "[Congelado:" << freeze_seconds << "s] ";
        string item_str = ss.str();

        SetConsoleTextAttribute(hConsole, 9); // Azul Claro
        cout << item_str;
        SetConsoleTextAttribute(hConsole, originalColors);

        line_length += item_str.length();
        hasActiveItem = true;
    }

    if (!hasActiveItem)
    {
        string none_str = "Nenhum";
        cout << none_str;
        line_length += none_str.length();
    }

    // Calcula e imprime o preenchimento para alinhar a borda direita
    padding = LARGURA_MAPA - line_length;
    if (padding > 0)
    {
        cout << string(padding, ' ');
    }
    cout << gameIcons.wall << "\n";

    // --- Linha de Status: Vidas ---
    cout << gameIcons.wall;
    SetConsoleTextAttribute(hConsole, 7); // Cor padrão

    line_length = 0; // Reseta o contador para a nova linha

    label = "Vidas: ";
    cout << label;
    line_length += label.length();

    string life_symbol = string(1, char(254)) + " "; // "■ "
    line_length += currentVidas * life_symbol.length();

    SetConsoleTextAttribute(hConsole, 12); // Vermelho claro para vidas
    for (int i = 0; i < currentVidas; ++i)
    {
        cout << life_symbol;
    }
    SetConsoleTextAttribute(hConsole, originalColors); // Restaura cor

    // Calcula e imprime o preenchimento para a linha de vidas
    padding = LARGURA_MAPA - line_length;
    if (padding > 0)
    {
        cout << string(padding, ' ');
    }
    cout << gameIcons.wall << "\n";

    cout << gameIcons.wall;
    for (int x = 0; x < LARGURA_MAPA; x++)
        cout << gameIcons.wall;
    cout << gameIcons.wall << "\n";

    SetConsoleTextAttribute(hConsole, 7);
}