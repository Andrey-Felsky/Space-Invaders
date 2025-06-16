#include "mapa.h"
#include "../utils/gameElements.h"
#include <iostream>
#include <windows.h>
#include <cmath> // Para roundf
#include <string> // Para construir a string do frame
#include <sstream> // Para construir a string do frame
#include <iomanip> // Para setprecision

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
            mapa[tiroInimigoY_int][tiroInimigoX_int] = '|'; // Usar o mesmo símbolo do jogador, ou outro se quiser diferenciar
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

    // *** MODIFICAÇÃO AQUI: INÍCIO DA BORDAR SUPERIOR DO HUD ***
    SetConsoleTextAttribute(hConsole, gameIcons.wallColor); // Cor da parede
    frameBuffer << gameIcons.wall; // Parede esquerda
    for (int i = 0; i < largura; ++i) {
        frameBuffer << gameIcons.wall;
    }
    frameBuffer << gameIcons.wall << "\n"; // Parede direita e quebra de linha
    // ************************************************************

    // Renderiza o cabeçalho do HUD
    SetConsoleTextAttribute(hConsole, gameIcons.wallColor); // Cor da parede para a borda
    frameBuffer << gameIcons.wall; // Parede esquerda do HUD
    SetConsoleTextAttribute(hConsole, gameIcons.pathColor); // Cor padrão para o texto do HUD
    // Espaçamento para centralizar/ajustar o HUD
    frameBuffer << "SCORE: " << score << "   TIME: " << fixed << setprecision(1) << tempoDecorrido << "s";
    // Preencher o restante da linha do HUD com espaços para manter o alinhamento da borda
    int hudTextLength = string("SCORE: ").length() + to_string(score).length() +
                        string("   TIME: ").length() + to_string(static_cast<int>(tempoDecorrido)).length() + 1; // +1 para 's'
    
    // Ajuste aqui para considerar a largura total desejada do HUD, que é largura do mapa.
    // O texto do HUD tem um tamanho variável, então preenchemos o espaço restante.
    // Contagem aproximada do texto para preencher corretamente.
    // Lembre-se que 'largura' é a largura interna do mapa. As bordas adicionam 2.
    // Ajuste o padding conforme necessário para o layout exato.
    // Este cálculo é uma estimativa e pode precisar de ajuste fino.
    int remainingSpace = largura - hudTextLength;
    if (remainingSpace > 0) {
        for (int i = 0; i < remainingSpace; ++i) {
            frameBuffer << ' ';
        }
    }
    
    SetConsoleTextAttribute(hConsole, gameIcons.wallColor); // Cor da parede
    frameBuffer << gameIcons.wall << "\n"; // Parede direita do HUD e quebra de linha

    // Linha separadora do HUD (mesmo caractere da borda)
    SetConsoleTextAttribute(hConsole, gameIcons.wallColor); // Cor da parede
    frameBuffer << gameIcons.wall; // Parede esquerda
    for (int i = 0; i < largura; ++i) {
        frameBuffer << gameIcons.wall;
    }
    frameBuffer << gameIcons.wall << "\n"; // Parede direita e quebra de linha
    // *******************************************************************


    // Renderiza as bordas laterais e o conteúdo do mapa
    for (int y = 0; y < altura; y++)
    {
        SetConsoleTextAttribute(hConsole, gameIcons.wallColor); // Cor da parede
        frameBuffer << gameIcons.wall;
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

    cout << frameBuffer.str();
}