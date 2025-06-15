#include <iostream>
#include <windows.h>
#include <conio.h>
#include <chrono>
#include <cstdlib>
#include <ctime> 

#include "mapa/mapa.h"
#include "enemy/enemy.h"
#include "logic/logic.h"
#include "menu/menu.h"
#include "ranking/score.h"
#include "utils/cleanScreen/cleanScreen.h"

using namespace std;
using namespace std::chrono;

//COMANDO PARA COMPILAR: 
//g++ main.cpp mapa/mapa.cpp enemy/enemy.cpp logic/logic.cpp menu/menu.cpp ranking/score.cpp utils/cleanScreen/cleanScreen.cpp -o output/main.exe

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

bool tiroInimigoAtivo = false;
int tiroInimigoX = 0, tiroInimigoY = 0;

int contadorMovimento = 0;
int intervaloTempo = 15; 

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

void game(){
    // zera estado do jogo
    gameOver = false;
    tiroAtivo = false;
    score = 0;
    naveX = largura / 2;
    initEnemy(); // inicializa os inimigos

    system("cls");
    string nome;
    cout << "Digite seu nome: ";
    cin >> nome;

    auto inicio = high_resolution_clock::now();

    // loop principal do jogo
    while (!gameOver) {
        cleanScreen(); //limpa a tela
        render(); //renderiza o mapa, enimigos e tiros
        input(); // recebe os dados do player
        updateTire(); // atualiza os tiros
        updateTiroInimigo(); // atualiza os tiros do inimigo
        checkCollisions(); // checa as colisões 

        contadorMovimento++;
        if (contadorMovimento >= intervaloTempo) {
            moveEnemies();
            contadorMovimento = 0;
        }

        checkEndOfGame(); // funcao que checa se chegou no final do jogo

        // usar essa variavel para icrementar a dificuldade 
        //ex: quanto maior o numero dentro do Sleep() mais divagar fica o jogo
    }
    system("cls");
    auto fim = high_resolution_clock::now();
    duration<float> duracao = fim - inicio;

    //render();
    resetColor(); //necessario para o menu nao mudar de cor
    cout << "\nScore final: " << score << endl;

    saveScore(nome, score, duracao.count());

    cout << "\nPressione ENTER para voltar ao menu...";
    cin.ignore();
    cin.get();

    system("cls");
}

int main() {
    system("cls");
    hideCursor();
    srand(time(NULL));

    while (true) {
        menu();         
        game();  
    }

    return 0;
}
