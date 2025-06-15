#include "mapa.h"
#include "../utils/gameElements.h"
#include <iostream>
#include <windows.h>
#include <cmath> // Para roundf
#include <string> // Para construir a string do frame
#include <sstream> // Para construir a string do frame

using namespace std;

GameElements gameIcons;
HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);

// Cores ANSI para terminais modernos (opcional, mas mais limpo se suportado)
// #define ANSI_COLOR_RED     "\x1b[31m"
// #define ANSI_COLOR_GREEN   "\x1b[32m"
// #define ANSI_COLOR_YELLOW  "\x1b[33m"
// #define ANSI_COLOR_BLUE    "\x1b[34m"
// #define ANSI_COLOR_MAGENTA "\x1b[35m"
// #define ANSI_COLOR_CYAN    "\x1b[36m"
// #define ANSI_COLOR_RESET   "\x1b[0m"

void render()
{
    // Limpa o conteúdo do mapa com o "chão"
    for (int y = 0; y < altura; y++)
    {
        for (int x = 0; x < largura; x++)
        {
            mapa[y][x] = gameIcons.path;
        }
    }

    // renderiza os inimigos
    for (int i = 0; i < MAX_ENEMIES; i++)
    {
        if (inimigoVivo[i])
        {
            int enemyDrawX = roundf(inimigos[i][0]);
            int enemyDrawY = roundf(inimigos[i][1]);

            if (enemyDrawX >= 0 && enemyDrawX < largura &&
                enemyDrawY >= 0 && enemyDrawY < altura) {
                mapa[enemyDrawY][enemyDrawX] = gameIcons.enemy;
            }
        }
    }

    // renderiza o tiro do jogador
    if (tiroAtivo) {
        int shootDrawX = roundf(tiroX);
        int shootDrawY = roundf(tiroY);
        if (shootDrawY >= 0 && shootDrawY < altura &&
            shootDrawX >= 0 && shootDrawX < largura) {
            mapa[shootDrawY][shootDrawX] = gameIcons.shoot;
        }
    }

    // renderiza o tiro do inimigo
    if (tiroInimigoAtivo) {
        int enemyShootDrawX = roundf(tiroInimigoX);
        int enemyShootDrawY = roundf(tiroInimigoY);
        if (enemyShootDrawY >= 0 && enemyShootDrawY < altura &&
            enemyShootDrawX >= 0 && enemyShootDrawX < largura) {
            mapa[enemyShootDrawY][enemyShootDrawX] = '|'; // símbolo diferente do player
        }
    }

    // renderiza a nave
    int naveDrawX = roundf(naveX);
    if (naveDrawX >= 0 && naveDrawX < largura) {
        mapa[altura - 1][naveDrawX] = gameIcons.spaceship;
    }

    // --- Construção do Frame para Impressão Única ---
    stringstream frameBuffer;
    
    // Renderiza o topo do mapa
    SetConsoleTextAttribute(hConsole, gameIcons.wallColor); // Cor da parede
    frameBuffer << gameIcons.wall;
    for (int x = 0; x < largura; x++)
        frameBuffer << gameIcons.wall;
    frameBuffer << gameIcons.wall << "\n";
    SetConsoleTextAttribute(hConsole, gameIcons.pathColor); // Volta para cor do caminho

    // Renderiza o mapa com as bordas laterais e cores dos elementos
    for (int y = 0; y < altura; y++)
    {
        SetConsoleTextAttribute(hConsole, gameIcons.wallColor); // Cor da parede
        frameBuffer << gameIcons.wall;
        SetConsoleTextAttribute(hConsole, gameIcons.pathColor); // Volta para cor do caminho

        for (int x = 0; x < largura; x++)
        {
            char c = mapa[y][x];

            // Define cor com base no caractere, sem adicionar ao stringstream
            // A cor será aplicada ao caractere individualmente
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
                SetConsoleTextAttribute(hConsole, 13); // magenta
            }
            else
            {
                SetConsoleTextAttribute(hConsole, gameIcons.pathColor); // cor padrão
            }
            frameBuffer << c; // Adiciona o caractere ao buffer
        }
        SetConsoleTextAttribute(hConsole, gameIcons.wallColor); // Cor da parede
        frameBuffer << gameIcons.wall << "\n";
        SetConsoleTextAttribute(hConsole, gameIcons.pathColor); // Volta para cor do caminho
    }

    // Renderiza a base do mapa
    SetConsoleTextAttribute(hConsole, gameIcons.wallColor); // Cor da parede
    frameBuffer << gameIcons.wall;
    for (int x = 0; x < largura; x++)
        frameBuffer << gameIcons.wall;
    frameBuffer << gameIcons.wall << "\n";
    SetConsoleTextAttribute(hConsole, gameIcons.pathColor); // Volta para cor do caminho

    // Garante que a cor padrão volte após o frame
    SetConsoleTextAttribute(hConsole, gameIcons.pathColor);

    // Escreve todo o buffer de uma vez
    cout << frameBuffer.str();
}