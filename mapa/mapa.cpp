#include "mapa.h"
#include "../utils/gameElements.h"
#include <iostream>
#include <windows.h>
#include <iomanip>
#include <string>
#include <sstream>
using namespace std;

GameElements gameIcons;
HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);

void render(int score, float tempo, int currentVidas)
{
    // Desenha a borda superior do painel de status
    cout << gameIcons.wall;
    for (int x = 0; x < largura; x++)
        cout << gameIcons.wall;
    cout << gameIcons.wall << "\n";

    // --- Construção da linha de status Superior (Score e Tempo) ---
    stringstream ssScore;
    ssScore << "Score: " << score;
    string scoreStr = ssScore.str();

    stringstream ssTempo;
    ssTempo << fixed << setprecision(1) << tempo << "s";
    string tempoStr = "Tempo: " + ssTempo.str();

    // Largura total ocupada pelo texto Score e Tempo
    int occupiedLength = scoreStr.length() + tempoStr.length();
    // Espaço disponível para preencher entre Score e Tempo
    int padding = largura - occupiedLength;

    cout << gameIcons.wall; // Borda esquerda
    SetConsoleTextAttribute(hConsole, 7); // Cor padrão para score
    cout << scoreStr;

    // Preenche o espaço entre Score e Tempo
    for (int i = 0; i < padding; ++i) {
        cout << " ";
    }
    
    SetConsoleTextAttribute(hConsole, 7); // Cor padrão para tempo
    cout << tempoStr;
    
    cout << gameIcons.wall << "\n"; // Borda direita e quebra de linha

    // Desenha a linha divisória do status
    cout << gameIcons.wall;
    for (int x = 0; x < largura; x++)
        cout << gameIcons.wall;
    cout << gameIcons.wall << "\n";

    // --- Fim da Construção da linha de status Superior ---


    // Limpa o conteúdo do mapa com o "chão"
    for (int y = 0; y < altura; y++)
    {
        for (int x = 0; x < largura; x++)
        {
            mapa[y][x] = gameIcons.path;
        }
    }

    // Renderiza os inimigos
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

    // Renderiza o mapa com as bordas laterais e cores
    for (int y = 0; y < altura; y++)
    {
        cout << gameIcons.wall;
        for (int x = 0; x < largura; x++)
        {
            char c = mapa[y][x];

            // Define cor com base no caractere
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
                SetConsoleTextAttribute(hConsole, 13); // Cor para o tiro inimigo
            }
            else
            {
                SetConsoleTextAttribute(hConsole, 7); // Cor padrão
            }

            cout << c;
        }
        SetConsoleTextAttribute(hConsole, 7); // Reset cor da borda lateral
        cout << gameIcons.wall << "\n";
    }

    // Renderiza a borda inferior do mapa
    cout << gameIcons.wall;
    for (int x = 0; x < largura; x++)
        cout << gameIcons.wall;
    cout << gameIcons.wall << "\n";


    // --- Nova linha para as Vidas (abaixo do mapa) ---
    cout << gameIcons.wall; // Borda esquerda
    SetConsoleTextAttribute(hConsole, 7); // Cor padrão para "Vidas:"
    cout << "Vidas: ";

    SetConsoleTextAttribute(hConsole, gameIcons.enemyColor); // Cor vermelha para os blocos de vida
    for (int i = 0; i < currentVidas; ++i) {
        cout << char(254) << " "; // Bloco preenchido + espaço
    }
    // Preenche o restante da linha para alinhar com a borda
    int vidasContentLength = string("Vidas: ").length() + (currentVidas * 2); // 2 por char(254) + espaço
    int remainingSpaceForLives = largura - vidasContentLength;
    for (int i = 0; i < remainingSpaceForLives; ++i) {
        cout << " ";
    }
    SetConsoleTextAttribute(hConsole, 7); // Reset cor
    cout << gameIcons.wall << "\n"; // Borda direita e quebra de linha

    // Borda final da tela
    cout << gameIcons.wall;
    for (int x = 0; x < largura; x++)
        cout << gameIcons.wall;
    cout << gameIcons.wall << "\n";
    // --- Fim da nova linha para as Vidas ---


    SetConsoleTextAttribute(hConsole, 7); // Resetar cor final
}