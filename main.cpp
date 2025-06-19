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

    auto gameStartTime = high_resolution_clock::now();
    auto lastFrameTime = high_resolution_clock::now();
    auto lastEnemyMoveTime = high_resolution_clock::now();
    auto lastEnemyShotTime = high_resolution_clock::now();

    const std::chrono::milliseconds enemyMoveInterval(300); // Intervalo para o movimento dos inimigos
    const std::chrono::milliseconds enemyShotInterval(800); // Intervalo para os inimigos atirarem

    while (!gameOver) {
        auto now = high_resolution_clock::now();
        duration<float> elapsedTime = now - gameStartTime;
        float tempoDecorrido = elapsedTime.count();

        // Lógica de controle de FPS (roda a cada frameDuration)
        if (now - lastFrameTime >= frameDuration) {
            cleanScreen();
            render(score, tempoDecorrido);
            input();
            updateTire(); // Tiro do player atualiza a cada frame
            updateTiroInimigo(); // Tiro do inimigo atualiza a cada frame (movimento fluido)
            checkCollisions();
            checkEndOfGame();
            lastFrameTime = now;
        }

        // Lógica de movimento dos inimigos (timer próprio)
        if (now - lastEnemyMoveTime >= enemyMoveInterval) {
            moveEnemies();
            lastEnemyMoveTime = now;
        }

        // Lógica de disparo dos inimigos (timer próprio)
        if (!tiroInimigoAtivo && now - lastEnemyShotTime >= enemyShotInterval) {
            updateTiroInimigo(); // Tenta gerar um novo tiro se o intervalo passou
            lastEnemyShotTime = now;
        }

        // --- Substituição de std::this_thread::sleep_until ---
        // Este loop ativo (spin-lock) consome CPU, mas garante que o FPS não ultrapasse o limite
        // para compiladores sem std::thread ou std::sleep_until.
        // A duração mínima é garantida pelo if(now - lastFrameTime >= frameDuration) acima.
        // Se a máquina for muito rápida, o loop vai esperar aqui até o próximo frame.
        while (high_resolution_clock::now() - lastFrameTime < frameDuration) {
            // Espera ocupada (busy-wait). Ajustar FPS para um valor razoável (ex: 30-60)
            // ajuda a reduzir o consumo de CPU.
        }
        // --- Fim da Substituição ---
    }
    system("cls");
    auto gameEndTime = high_resolution_clock::now();
    duration<float> finalDuration = gameEndTime - gameStartTime;

    resetColor();
    cout << "\nScore final: " << score << endl;

    saveScore(nome, score, finalDuration.count());

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