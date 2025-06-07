#include <iostream>
#include <windows.h>
#include <conio.h>

#include "mapa/mapa.h"
#include "enemy/enemy.h"
#include "logic/logic.h"
#include "menu/menu.h"
#include "utils/cleanScreen/cleanScreen.h"

using namespace std;

//COMANDO PARA COMPILAR: 
//g++ main.cpp mapa/mapa.cpp enemy/enemy.cpp logic/logic.cpp menu/menu.cpp utils/cleanScreen/cleanScreen.cpp -o output/main.exe

//ATENCAO:
// criar um arquivo novo ou pasta incluir no comando

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

void input() {
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
    menu(); // inicializa o menu
    initEnemy(); // inicializa os inimigos
    hideCursor(); // limpa o cursor

    // loop principal do jogo
    while (!gameOver) {
        cleanScreen(); //limpa a tela
        render(); //renderiza o mapa, enimigos e tiros
        input(); // recebe os dados do player
        updateTire(); // atualiza os tiros
        checkCollisions(); // checa as colisões 
        moveEnemies(); // logica para movimentaçao dos inimigos
        checkEndOfGame(); // funcao que checa se chegou no final do jogo

        Sleep(30); 
        // usar essa variavel para icrementar a dificuldade 
        //ex: quanto maior o numero dentro do Sleep() mais divagar fica o jogo
    }

    //falta iplementar o gameOver e score
    if(gameOver){
        cout << "\nScore final: " << score << endl;
        cout << "Obrigado por jogar!\n";
    }

    return 0;
}
