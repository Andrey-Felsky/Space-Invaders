#include "mapa.h"
#include "../utils/gameElements.h"
#include <iostream>
#include <windows.h>
#include <iomanip>
using namespace std;

GameElements gameIcons;
HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);

void render(int score, float tempo)
{
    cout << gameIcons.wall;
    for (int x = 0; x < largura; x++)
        cout << gameIcons.wall;
    cout << gameIcons.wall << "\n";

    cout << gameIcons.wall << "Score: " << score
     << "           Tempo: " << fixed << setprecision(1) << tempo << "s" << gameIcons.wall
     << endl;

    for (int y = 0; y < altura; y++)
    {
        for (int x = 0; x < largura; x++)
        {
            mapa[y][x] = gameIcons.path;
        }
    }

    for (int i = 0; i < 25; i++)
    {
        if (inimigoVivo[i])
        {
            mapa[inimigos[i][1]][inimigos[i][0]] = gameIcons.enemy;
        }
    }

    if (tiroAtivo && tiroY >= 0)
        mapa[tiroY][tiroX] = gameIcons.shoot;

    if (tiroInimigoAtivo && tiroInimigoY >= 0 && tiroInimigoY < altura)
        mapa[tiroInimigoY][tiroInimigoX] = '|';

    mapa[altura - 1][naveX] = gameIcons.spaceship;

    cout << gameIcons.wall;
    for (int x = 0; x < largura; x++)
        cout << gameIcons.wall;
    cout << gameIcons.wall << "\n";

    for (int y = 0; y < altura; y++)
    {
        cout << gameIcons.wall;
        for (int x = 0; x < largura; x++)
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
    for (int x = 0; x < largura; x++)
        cout << gameIcons.wall;
    cout << gameIcons.wall << "\n";

    SetConsoleTextAttribute(hConsole, 7);
}