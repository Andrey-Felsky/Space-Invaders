#include <iostream>
#include <windows.h>
#include <conio.h>
#include <limits>
#include "../ranking/score.h"
#include "../utils/cleanScreen/cleanScreen.h"
using namespace std;

const int N_OPCOES = 4;
string opcoes[N_OPCOES] = { "  Jogar   ", "Instrucoes", "  Ranking ", "  Sair    " };
int selectedItem = 0;

void exibirInstrucoes() {
    system("cls");
    cout << "=== INSTRUCOES ===\n";
    cout << "- Use as teclas A e D para mover a nave\n";
    cout << "- Pressione ESPACO para atirar\n";
    cout << "- Derrote todos os inimigos antes que eles cheguem até sua base\n";
    cout << "- Evite ser atingido pelos inimigos\n\n";
    cout << "Pressione ENTER para voltar ao menu...";
    cin.ignore(numeric_limits<streamsize>::max(), '\n');
    cin.get();
    system("cls");
}

void exibirRanking() {
    system("cls");
    cout << "=== RANKING ===\n";
    drawnRanking();
    cin.ignore(numeric_limits<streamsize>::max(), '\n');
    cin.get();
    system("cls");
}

void renderMenu() {
    cleanScreen();
    cout << " ____  ____   _    ____ _____                   \n"\
            "/ ___||  _ \\\\ / \\\\  / ___| ____|                  \n"\
            "\\\\___ \\\\| |_) / _ \\\\| |   |  _|                    \n"\
            " ___) |  __/ ___ \\\\ |___| |___                   \n"\
            "|____/|_|_/_/   \\\\_\\\\____|_____|_____ ____  ____  \n"\
            "|_ _| \\\\ | \\\\ \\\\   / / \\\\  |  _ \\\\| ____|  _ \\\\/ ___| \n"\
            " | ||  \\\\| |\\\\ \\\\ / / _ \\\\ | | | |  _| | |_) \\\\___ \\\\ \n"\
            " | || |\\\\  | \\\\ V / ___ \\\\| |_| | |___|  _ < ___) |\n"\
            "|___|_| \\\\_|  \\\\_/_/   \\\\_\\\\____/|_____|_| \\\\_\\\\____/ \n\n";
    for (int i = 0; i < N_OPCOES; i++) {
        if (i == selectedItem) {
            cout << "|>> " << opcoes[i] << " <<|\n";
        } else {
            cout << "|   " << opcoes[i] << "   |\n";
        }
    }
}

int navegarMenu() {
    system("cls");
    while (true) {
        renderMenu();
        char tecla = _getch();
        if (tecla == 'w' || tecla == 72) {
            selectedItem = (selectedItem == 0) ? N_OPCOES - 1 : selectedItem - 1;
        } else if (tecla == 's' || tecla == 80) {
            selectedItem = (selectedItem == N_OPCOES - 1) ? 0 : selectedItem + 1;
        } else if (tecla == 13) {
            return selectedItem;
        }
    }
}

void menu() {
    int escolha = navegarMenu();
    switch (escolha) {
        case 0:
            break;
        case 1:
            exibirInstrucoes();
            break;
        case 2:
            exibirRanking();
            break;
        case 3:
            exit(0);
    }
}