#include "mapa.h"
#include <windows.h>
#include "../utils/gameElements.h"
#include <iostream>

using namespace std;

GameElements gameIcons;
HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);

void render() {
    //limpa o conteudo
    for (int y = 0; y < altura; y++) {
        for (int x = 0; x < largura; x++) {
            mapa[y][x] = gameIcons.path;
        }
    }

    // renderiza os inimigos
    for (int i = 0; i < 5; i++) {
        if (inimigoVivo[i]) {
            mapa[inimigos[i][1]][inimigos[i][0]] = gameIcons.enemy;
        } 
    }

    // renderiza os tiros
    if (tiroAtivo && tiroY >= 0)
        mapa[tiroY][tiroX] = gameIcons.shoot;

    if (tiroInimigoAtivo && tiroInimigoY <= altura)
        mapa[tiroInimigoY][tiroInimigoX] = gameIcons.shoot;

    // renderiza a nave
    mapa[altura - 1][naveX] = gameIcons.spaceship;

    // renderiza topo do mapa
    cout << gameIcons.wall;
    for (int x = 0; x < largura; x++) cout << gameIcons.wall;
    cout << gameIcons.wall << "\n";

    // renderiza mapa com laterais
    for (int y = 0; y < altura; y++) {
        cout << gameIcons.wall;
        for (int x = 0; x < largura; x++) {
            char c = mapa[y][x];

            //altera a cor dos simbolos
            if (c == gameIcons.enemy) {
                SetConsoleTextAttribute(hConsole, gameIcons.enemyColor); 
            } else if (c == gameIcons.shoot) {
                SetConsoleTextAttribute(hConsole, gameIcons.shootColor); 
            } else if (c == gameIcons.spaceship) {
                SetConsoleTextAttribute(hConsole, gameIcons.spaceshipColor); 
            } else {
                SetConsoleTextAttribute(hConsole, 7);  
            }

            cout << c;
        }
        cout << gameIcons.wall << "\n";
    }

    // renderiza base do mapa
    cout << gameIcons.wall;
    for (int x = 0; x < largura; x++) cout << gameIcons.wall;
    cout << gameIcons.wall << "\n";
}
