#include <iostream>
#include <windows.h>
#include <conio.h>
#include "../ranking/score.h"
#include "../utils/cleanScreen/cleanScreen.h"
using namespace std;

const int N_OPCOES = 4;
string opcoes[N_OPCOES] = { "  Jogar   ", "Instrucoes", " Ranking  ", "  Sair    " };
int selectedItem = 0;
void exibirInstrucoes() {
    SetConsoleOutputCP(CP_UTF8);
    system("cls");
    cout << "===== INSTRUÇÕES =====\n";
    cout << "- Use as teclas A e D para mover a nave\n";
    cout << "- Pressione ESPACO para atirar\n";
    cout << "- Derrote todos os inimigos antes que eles cheguem até sua base\n";
    cout << "- Evite ser atingido pelos inimigos\n\n";
    cout << "Pressione ENTER para voltar ao menu...";
    cin.get();
    system("cls");
}

void exibirRanking() {
    system("cls");
    cout << "===== RANKING =====\n";
    drawnRanking();
    //falta fazer
    cin.get();
    system("cls");
}

void renderMenu() {
    ClearConsole();
    cout << " ____  ____   _    ____ _____                   \n"
"/ ___||  _ \\ / \\  / ___| ____|                  \n"
"\\___ \\| |_) / _ \\| |   |  _|                    \n"
" ___) |  __/ ___ \\ |___| |___                   \n"
"|____/|_|_/_/   \\_\\____|_____|_____ ____  ____  \n"
"|_ _| \\ | \\ \\   / / \\  |  _ \\| ____|  _ \\/ ___| \n"
" | ||  \\| |\\ \\ / / _ \\ | | | |  _| | |_) \\___ \\ \n"
" | || |\\  | \\ V / ___ \\| |_| | |___|  _ < ___) |\n"
"|___|_| \\_|  \\_/_/   \\_\\____/|_____|_| \\_\\____/ \n\n";
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
        if (tecla == 'w' || tecla == 72) { // cima
            selectedItem = (selectedItem - 1 + N_OPCOES) % N_OPCOES;
        } else if (tecla == 's' || tecla == 80) { // baixo
            selectedItem = (selectedItem + 1) % N_OPCOES;
        } else if (tecla == 13) { // Enter
            return selectedItem;
        }
    }
}

void menu() {
    system("cls");
    while (true) {
        int opcao = navegarMenu();

        switch (opcao) {
            case 0: return; 
            case 1: exibirInstrucoes(); break;
            case 2: exibirRanking(); break;
            case 3: exit(0); 
        }
    }
}
