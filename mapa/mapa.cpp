#define _WIN32_WINNT 0x0600 // **ADICIONE ESTA LINHA AQUI se não estiver no topo!**

#include "mapa.h"
#include "../utils/gameElements.h"
#include <iostream>
#include <windows.h> // Mantenha este include, mas não usaremos SetConsoleTextAttribute
#include <cmath>     // Para roundf
#include <string>    // Para construir a string do frame
#include <sstream>   // Para construir a string do frame
#include <iomanip>   // Para setprecision

using namespace std;

GameElements gameIcons;
// HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE); // Não é mais necessário para SetConsoleTextAttribute

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
            // Arredonda as posições float para int para desenhar no mapa de caracteres
            int inimigoX_int = static_cast<int>(roundf(inimigos[i][0]));
            int inimigoY_int = static_cast<int>(roundf(inimigos[i][1]));

            // Garante que o inimigo esteja dentro dos limites do mapa antes de desenhar
            if (inimigoX_int >= 0 && inimigoX_int < largura &&
                inimigoY_int >= 0 && inimigoY_int < altura)
            {
                mapa[inimigoY_int][inimigoX_int] = gameIcons.enemy;
            }
        }
    }

    // Renderiza o tiro do jogador
    if (tiroAtivo)
    {
        int tiroX_int = static_cast<int>(roundf(tiroX));
        int tiroY_int = static_cast<int>(roundf(tiroY));
        if (tiroX_int >= 0 && tiroX_int < largura &&
            tiroY_int >= 0 && tiroY_int < altura)
        {
            mapa[tiroY_int][tiroX_int] = gameIcons.shoot;
        }
    }

    // Renderiza o tiro do inimigo
    if (tiroInimigoAtivo)
    {
        int tiroInimigoX_int = static_cast<int>(roundf(tiroInimigoX));
        int tiroInimigoY_int = static_cast<int>(roundf(tiroInimigoY));
        if (tiroInimigoX_int >= 0 && tiroInimigoX_int < largura &&
            tiroInimigoY_int >= 0 && tiroInimigoY_int < altura)
        {
            mapa[tiroInimigoY_int][tiroInimigoX_int] = '|'; // Usar '|' para o tiro inimigo, diferenciar para colorir
        }
    }

    // Renderiza a nave do jogador
    int naveX_int = static_cast<int>(roundf(naveX));
    if (naveX_int >= 0 && naveX_int < largura)
    {
        mapa[altura - 1][naveX_int] = gameIcons.spaceship;
    }

    // Cria um stringstream para construir o frame completo
    stringstream frameBuffer;

    // Borda superior do HUD
    frameBuffer << gameIcons.ANSI_BRIGHT_BLUE << gameIcons.wall; // Cor da parede
    for (int i = 0; i < largura; ++i) {
        frameBuffer << gameIcons.wall; // A cor já está definida pela linha acima
    }
    frameBuffer << gameIcons.wall << gameIcons.ANSI_RESET << "\n"; // Parede direita e reset de cor

    // Renderiza o cabeçalho do HUD
    frameBuffer << gameIcons.ANSI_BRIGHT_BLUE << gameIcons.wall; // Cor da parede para a borda
    frameBuffer << gameIcons.ANSI_WHITE; // Cor padrão para o texto do HUD (branco)
    
    // Texto do HUD
    string scoreStr = to_string(score);
    string timeStr = to_string(static_cast<int>(tempoDecorrido)) + "s";
    string hudDisplayText = "SCORE: " + scoreStr + "   TIME: " + timeStr;

    // Adiciona o texto do HUD ao buffer com a cor desejada
    frameBuffer << gameIcons.ANSI_BRIGHT_GREEN << hudDisplayText << gameIcons.ANSI_RESET;

    // Preencher o restante da linha do HUD com espaços para manter o alinhamento da borda
    int visualHudTextLength = hudDisplayText.length();
    int remainingSpace = largura - visualHudTextLength;
    
    if (remainingSpace > 0) {
        for (int i = 0; i < remainingSpace; ++i) {
            frameBuffer << ' ';
        }
    }
    
    frameBuffer << gameIcons.ANSI_BRIGHT_BLUE << gameIcons.wall << gameIcons.ANSI_RESET << "\n"; // Parede direita do HUD e reset de cor

    // Linha separadora do HUD (mesmo caractere da borda)
    frameBuffer << gameIcons.ANSI_BRIGHT_BLUE << gameIcons.wall; // Cor da parede
    for (int i = 0; i < largura; ++i) {
        frameBuffer << gameIcons.wall;
    }
    frameBuffer << gameIcons.wall << gameIcons.ANSI_RESET << "\n"; // Parede direita e reset de cor


    // Renderiza as bordas laterais e o conteúdo do mapa com cores
    for (int y = 0; y < altura; y++)
    {
        frameBuffer << gameIcons.ANSI_BRIGHT_BLUE << gameIcons.wall; // Cor da parede para a borda esquerda
        for (int x = 0; x < largura; x++)
        {
            char c = mapa[y][x];
            if (c == gameIcons.enemy)
            {
                frameBuffer << gameIcons.ANSI_BRIGHT_RED; // Define a cor para o inimigo
            }
            else if (c == gameIcons.shoot)
            {
                frameBuffer << gameIcons.ANSI_BRIGHT_YELLOW; // Define a cor para o tiro do jogador
            }
            else if (c == gameIcons.spaceship)
            {
                frameBuffer << gameIcons.ANSI_BRIGHT_GREEN; // Define a cor para a nave
            }
            else if (c == '|') // Tiro do inimigo (usando '|' como símbolo)
            {
                frameBuffer << gameIcons.ANSI_BRIGHT_MAGENTA; // Define a cor para o tiro do inimigo
            }
            else
            {
                frameBuffer << gameIcons.ANSI_WHITE; // Cor padrão para o caminho/vazio (branco)
            }
            frameBuffer << c; // Adiciona o caractere ao buffer
            frameBuffer << gameIcons.ANSI_RESET; // **Importante: Resetar a cor após CADA caractere**
        }
        frameBuffer << gameIcons.ANSI_BRIGHT_BLUE << gameIcons.wall << gameIcons.ANSI_RESET << "\n"; // Parede direita e reset de cor
    }

    // Renderiza a base do mapa
    frameBuffer << gameIcons.ANSI_BRIGHT_BLUE << gameIcons.wall;
    for (int x = 0; x < largura; x++)
        frameBuffer << gameIcons.wall;
    frameBuffer << gameIcons.wall << gameIcons.ANSI_RESET << "\n";
    
    // Garante que a cor padrão volte após o frame completo para qualquer texto futuro
    frameBuffer << gameIcons.ANSI_RESET;

    cout << frameBuffer.str(); // Imprime o frame completo de uma vez
}