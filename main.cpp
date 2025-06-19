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

const int largura = 30;
const int altura = 20;
char mapa[altura][largura];

int naveX = largura / 2;

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

const int FPS = 30;
const std::chrono::milliseconds frameDuration(1000 / FPS);

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
    gameOver = false;
    tiroAtivo = false;
    score = 0;
    naveX = largura / 2;
    initEnemy();

    system("cls");
    string nome;
    cout << "Digite seu nome: ";
    cin >> nome;
    cin.ignore();

    auto inicio = high_resolution_clock::now();
    auto lastFrameTime = high_resolution_clock::now();
    auto lastEnemyMoveTime = high_resolution_clock::now();
    auto lastEnemyShotTime = high_resolution_clock::now();

    const std::chrono::milliseconds enemyMoveInterval(300);
    const std::chrono::milliseconds enemyShotInterval(1000);


    while (!gameOver) {
        auto now = high_resolution_clock::now();
        duration<float> duracao = now - inicio;
        float tempoDecorrido = duracao.count();

        if (now - lastFrameTime >= frameDuration) {
            cleanScreen();
            render(score, tempoDecorrido);
            input();
            updateTire();
            checkCollisions();

            if (now - lastEnemyMoveTime >= enemyMoveInterval) {
                moveEnemies();
                lastEnemyMoveTime = now;
            }

            if (now - lastEnemyShotTime >= enemyShotInterval) {
                updateTiroInimigo();
                lastEnemyShotTime = now;
            }

            checkEndOfGame();
            lastFrameTime = now;
        }
    }
    system("cls");
    auto fim = high_resolution_clock::now();
    duration<float> duracaoFinal = fim - inicio;

    resetColor();
    cout << "\nScore final: " << score << endl;

    saveScore(nome, score, duracaoFinal.count());

    cout << "\nPressione ENTER para voltar ao menu...";
    cin.get();
    system("cls");
}

int main() {
    srand(time(NULL));
    system("cls");
    hideCursor();

    while (true) {
        menu();
        game();
    }

    return 0;
}