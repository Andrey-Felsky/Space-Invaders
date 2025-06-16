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

//variaceis mapa
const int largura = 30;
const int altura = 20;
char mapa[altura][largura];

int naveX = largura / 2; // começa no meio do mapa

bool inimigoVivo[25];
int inimigos[25][2];

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

    auto inicio = high_resolution_clock::now(); //comeca a contar o tempo da partida.

    // loop principal do jogo
    while (!gameOver) {

        //calculamos o tempo que passou desde que o jogo começou.
        auto agora = high_resolution_clock::now();
        duration<float> duracao = agora - inicio;
        float tempoDecorrido = duracao.count();

        cleanScreen(); //limpa a tela
        render(score, tempoDecorrido); //renderiza o mapa, enimigos e tiros
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
    }
    system("cls");
    //marca o fim da partida.
    auto fim = high_resolution_clock::now();
    duration<float> duracao = fim - inicio;

    resetColor(); //necessario para o menu nao mudar de cor
    cout << "\nScore final: " << score << endl;

    saveScore(nome, score, duracao.count());

    cout << "\nPressione ENTER para voltar ao menu...";
    cin.get();

    system("cls");
}

int main() {
    system("cls");
    hideCursor();

    while (true) {
        menu();         
        game();  
    }

    return 0;
}
