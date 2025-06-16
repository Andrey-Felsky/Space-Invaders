#include "mapa.h"
#include "../utils/gameElements.h"
#include <iostream>
#include <windows.h>
#include <iomanip> //usado para tratar casas decimais to tempo
using namespace std;

GameElements gameIcons;
HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);

void render(int score, float tempo)
{   
    //renderiza o score e tempo no começo da tela
    cout << gameIcons.wall;
    for (int x = 0; x < largura; x++)
        cout << gameIcons.wall;
    cout << gameIcons.wall << "\n";

    cout << gameIcons.wall << "Score: " << score 
     << "           Tempo: " << fixed << setprecision(1) << tempo << "s" << gameIcons.wall
     << endl;

    // limpa o conteúdo do mapa com o "chão"
    for (int y = 0; y < altura; y++)
    {
        for (int x = 0; x < largura; x++)
        {
            mapa[y][x] = gameIcons.path;
        }
    }

    // renderiza os inimigos
    for (int i = 0; i < 25; i++)
    {
        if (inimigoVivo[i])
        {
            mapa[inimigos[i][1]][inimigos[i][0]] = gameIcons.enemy;
        }
    }

    // renderiza o tiro do jogador
    if (tiroAtivo && tiroY >= 0)
        mapa[tiroY][tiroX] = gameIcons.shoot;

    // renderiza o tiro do inimigo
    if (tiroInimigoAtivo && tiroInimigoY >= 0 && tiroInimigoY < altura)
        mapa[tiroInimigoY][tiroInimigoX] = '|'; // símbolo diferente do player

    // renderiza a nave
    mapa[altura - 1][naveX] = gameIcons.spaceship;

    // renderiza o topo do mapa
    cout << gameIcons.wall;
    for (int x = 0; x < largura; x++)
        cout << gameIcons.wall;
    cout << gameIcons.wall << "\n";

    // renderiza o mapa com as bordas laterais e cores
    for (int y = 0; y < altura; y++)
    {
        cout << gameIcons.wall;
        for (int x = 0; x < largura; x++)
        {
            char c = mapa[y][x];

            // define cor com base no caractere
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
            else if (c == '|') // tiro do inimigo
            {
                SetConsoleTextAttribute(hConsole, 13); 
            }
            else
            {
                SetConsoleTextAttribute(hConsole, 7); 
            }

            cout << c;
        }
        SetConsoleTextAttribute(hConsole, 7); // reset cor da borda lateral
        cout << gameIcons.wall << "\n";
    }

    // renderiza a base do mapa
    cout << gameIcons.wall;
    for (int x = 0; x < largura; x++)
        cout << gameIcons.wall;
    cout << gameIcons.wall << "\n";

    // garante que a cor padrão volte após o frame
    SetConsoleTextAttribute(hConsole, 7);
}
