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
            mapa[bullet.second][bullet.first] = gameIcons.shoot;
    }

    if (tiroInimigoAtivo && tiroInimigoY >= 0 && tiroInimigoY < ALTURA_MAPA)
        mapa[tiroInimigoY][tiroInimigoX] = '|';

    mapa[ALTURA_MAPA - 1][naveX] = gameIcons.spaceship;

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

            if (c == gameIcons.enemy)
            {
                SetConsoleTextAttribute(hConsole, gameIcons.enemyColor);
            }
            else if (c == gameIcons.shoot)
            {
                SetConsoleTextAttribute(hConsole, gameIcons.shootColor);
            }
            else if (c == gameIcons.spaceship)
            {
                SetConsoleTextAttribute(hConsole, gameIcons.spaceshipColor);
            }
            else if (c == '|')
            {
                SetConsoleTextAttribute(hConsole, 13); 
            }
            else if (c == 'X' || c == '@')
            {
                SetConsoleTextAttribute(hConsole, 6);
            }
            else if (c == 'L' || c == 'S' || c == 'E' || c == 'M' || c == '$' || c == 'F' || c == '+' || c == '?') // Item characters
            {
                // Assign specific colors per item type if desired, e.g., green for Life, yellow for Speed
                if (c == 'L') SetConsoleTextAttribute(hConsole, 10); // Green
                else if (c == 'S') SetConsoleTextAttribute(hConsole, 14); // Yellow
                else if (c == '$') SetConsoleTextAttribute(hConsole, 12); // Light Red for points
                else if (c == 'F') SetConsoleTextAttribute(hConsole, 3); // Cyan for Freeze
                else SetConsoleTextAttribute(hConsole, 11); // Light Cyan for others
                
            }
            else
            {
                SetConsoleTextAttribute(hConsole, 7); 
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

    if (std::chrono::duration_cast<std::chrono::seconds>(speedBoostEndTime - now).count() > 0) {
        activeItemsStatus += "[Velocidade:" + to_string(std::chrono::duration_cast<std::chrono::seconds>(speedBoostEndTime - now).count()) + "s] ";
        hasActiveItem = true;
    }
    if (maxPlayerBulletsAllowed > 1 && std::chrono::duration_cast<std::chrono::seconds>(extraShotEndTime - now).count() > 0) {
        activeItemsStatus += "[Tiro Extra:" + to_string(std::chrono::duration_cast<std::chrono::seconds>(extraShotEndTime - now).count()) + "s (" + to_string(maxPlayerBulletsAllowed) + ")] ";
        hasActiveItem = true;
    }
    if (multiShotActive && std::chrono::duration_cast<std::chrono::seconds>(multiShotEndTime - now).count() > 0) {
        activeItemsStatus += "[Tiro Multi:" + to_string(std::chrono::duration_cast<std::chrono::seconds>(multiShotEndTime - now).count()) + "s] ";
        hasActiveItem = true;
    }
    if (std::chrono::duration_cast<std::chrono::seconds>(enemyFreezeEndTime - now).count() > 0) {
        activeItemsStatus += "[Inimigos Congelados:" + to_string(std::chrono::duration_cast<std::chrono::seconds>(enemyFreezeEndTime - now).count()) + "s] ";
        hasActiveItem = true;
    }

    if (!hasActiveItem) {
        activeItemsStatus += "Nenhum";
    }
    
    cout << left << setw(LARGURA_MAPA) << activeItemsStatus.substr(0, LARGURA_MAPA); // Ensure it fits
    cout << gameIcons.wall << "\n";

    cout << gameIcons.wall;
    SetConsoleTextAttribute(hConsole, 7);
    cout << "Vidas: ";

    SetConsoleTextAttribute(hConsole, gameIcons.enemyColor);
    for (int i = 0; i < currentVidas; ++i) {
        cout << char(254) << " ";
    }
    int vidasContentLength = string("Vidas: ").length() + (currentVidas * 2);
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