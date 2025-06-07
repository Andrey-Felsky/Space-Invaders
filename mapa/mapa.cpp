#include "mapa.h"
#include <iostream>
using namespace std;

void render() {
    for (int y = 0; y < altura; y++) {
        for (int x = 0; x < largura; x++) {
            mapa[y][x] = ' ';
        }
    }

    // desenhar inimigos
    for (int i = 0; i < 5; i++) {
        if (inimigoVivo[i]) {
            mapa[inimigos[i][1]][inimigos[i][0]] = 'W';
        }
    }

    // desenhar tiro
    if (tiroAtivo && tiroY >= 0)
        mapa[tiroY][tiroX] = '|';

    // desenhar nave
    mapa[altura - 1][naveX] = '^';

    // imprimir mapa
    for (int y = 0; y < altura; y++) {
        for (int x = 0; x < largura; x++) {
            cout << mapa[y][x];
        }
        cout << endl;
    }
}
