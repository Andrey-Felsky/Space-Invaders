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

    if (tiroAtivo && tiroY >= 0)
        mapa[tiroY][tiroX] = gameIcons.shoot;

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