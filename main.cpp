#include <iostream>
#include <windows.h> // Para Sleep()
#include <conio.h>
#include <chrono>
#include <cstdlib>
#include <ctime>
// #include <thread> // REMOVA ESTA LINHA

#include "mapa/mapa.h"
#include "enemy/enemy.h"
#include "logic/logic.h"
#include "menu/menu.h"
#include "ranking/score.h"
#include "utils/cleanScreen/cleanScreen.h"
#include "utils/gameElements.h"

using namespace std;
using namespace std::chrono;

//COMANDO PARA COMPILAR:
//g++ main.cpp mapa/mapa.cpp enemy/enemy.cpp logic/logic.cpp menu/menu.cpp ranking/score.cpp utils/cleanScreen/cleanScreen.cpp -o output/main.exe -std=c++17
// OU APENAS:
//g++ main.cpp mapa/mapa.cpp enemy/enemy.cpp logic/logic.cpp menu/menu.cpp ranking/score.cpp utils/cleanScreen/cleanScreen.cpp -o output/main.exe

const int largura = 30;
const int altura = 20;
char mapa[altura][largura];

float naveX = largura / 2.0f;

float inimigos[MAX_ENEMIES][2];
bool inimigoVivo[MAX_ENEMIES];

bool tiroAtivo = false;
float tiroX = 0.0f, tiroY = 0.0f;

int score = 0;
bool gameOver = false;

bool tiroInimigoAtivo = false;
float tiroInimigoX = 0.0f, tiroInimigoY = 0.0f;

float deltaTime = 0.0f;

float playerMoveDirection = 0.0f;

void input() {
    while (_kbhit()) {
        char tecla = _getch();
        if (tecla == 'a' || tecla == 'A') {
            playerMoveDirection = -1.0f;
        }
        else if (tecla == 'd' || tecla == 'D') {
            playerMoveDirection = 1.0f;
        }
        else if (tecla == ' ') {
            if (!tiroAtivo) {
                tiroAtivo = true;
                tiroX = naveX;
                tiroY = altura - 2.0f;
            }
        }
    }
}

void updatePlayerPosition() {
    if (playerMoveDirection != 0.0f) {
        naveX += playerMoveDirection * PLAYER_SPEED * deltaTime;

        if (naveX < 0.0f) naveX = 0.0f;
        if (naveX > largura - 1.0f) naveX = largura - 1.0f;

        playerMoveDirection = 0.0f;
    }
}


void game(){
    gameOver = false;
    tiroAtivo = false;
    score = 0;
    naveX = largura / 2.0f;
    initEnemy();

    system("cls");
    string nome;
    cout << "Digite seu nome: ";
    cin >> nome;

    auto lastFrameTime = high_resolution_clock::now();
    auto inicio = high_resolution_clock::now(); // Definir 'inicio' aqui

    // Objetivo de FPS
    const int targetFPS = 60; // Por exemplo, 60 quadros por segundo
    // Duração de cada frame em milissegundos
    const int targetFrameDurationMs = 1000 / targetFPS; 

    while (!gameOver) {
        auto currentFrameTime = high_resolution_clock::now();
        duration<float> frameDuration = currentFrameTime - lastFrameTime;
        deltaTime = frameDuration.count(); // Tempo em segundos
        lastFrameTime = currentFrameTime;

        if (deltaTime == 0.0f) deltaTime = 0.0001f;

        cleanScreen();
        
        input();
        updatePlayerPosition();
        updateTire();
        updateTiroInimigo();
        checkCollisions();
        moveEnemies();
        checkEndOfGame();

        render();

        // Limita o FPS usando Sleep()
        auto endFrameTime = high_resolution_clock::now();
        duration<float, milli> elapsedMs = endFrameTime - currentFrameTime; // Tempo decorrido em milissegundos

        // Calcula quanto tempo ainda precisamos dormir
        int sleepTimeMs = targetFrameDurationMs - static_cast<int>(elapsedMs.count());
        
        if (sleepTimeMs > 0) {
            Sleep(sleepTimeMs);
        }
    }
    system("cls");
    auto fim = high_resolution_clock::now();
    duration<float> duracao = fim - inicio;

    resetColor();
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