#include <iostream>
#include <windows.h>
#include <conio.h>

#include "mapa/mapa.h"
#include "enemy/enemy.h"
#include "logic/logic.h"

using namespace std;

// posição inicial da nave
const int largura = 30;
const int altura = 20;
char mapa[altura][largura];

int naveX = largura / 2;

bool inimigoVivo[5];
int inimigos[5][2];

bool tiroAtivo = false;
int tiroX = 0, tiroY = 0;

int score = 0;
bool gameOver = false;


void cleanScreen() {
    COORD coord = {0, 0};
    SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), coord);
}

void entrada() {
    if (_kbhit()) {
        char tecla = _getch();
        if (tecla == 'a' || tecla == 'A') {
            if (naveX > 0) naveX--;
        }
        else if (tecla == 'd' || tecla == 'D') {
            if (naveX < largura - 1) naveX++;
        }
        else if (tecla == ' ') {
            if (!tiroAtivo) {
                tiroAtivo = true;
                tiroX = naveX;
                tiroY = altura - 2;
            }
        }
    }
}

int main() {
    // inicializa os inimigos
    inicializarInimigos();

    cout << "SPACE INVADERS TERMINAL" << endl;
    cout << "Use A/D para mover e ESPACO para atirar" << endl;
    cout << "Pressione ENTER para comecar...";
    cin.get();

    // loop principal do jogo
    while (!gameOver) {
        cleanScreen();
        render();
        entrada();
        updateTire();
        checkCollisions();
        moveEnemies();
        checkEndOfGame();
        Sleep(50);
    }

    // desenha última tela e mostra score
    cleanScreen();
    render();
    cout << "\nScore final: " << score << endl;
    cout << "Obrigado por jogar!\n";

    return 0;
}
