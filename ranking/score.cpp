#include "../utils/gameElements.h"
#include "score.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <ctime>
#include <iomanip>
#include <algorithm>

using namespace std; 

const int MAX_RANKING = 100;

void saveScore(const string& nome, int score, float tempo) {
    ofstream arq("../utils/scores/scores.txt", ios::app);
    if (!arq.is_open()) return;

    time_t t = time(0);
    tm* now = localtime(&t);
    char buffer[11];
    strftime(buffer, sizeof(buffer), "%d/%m/%Y", now);

    string resultado = (score >= 240) ? "VITÓRIA" : "DERROTA";

    arq << nome << "," << score << "," << buffer << "," << tempo << "," << resultado << "\n";
    arq.close();
}

void drawnRanking() {
    ifstream arq("../utils/scores/scores.txt");
    if (!arq.is_open()) {
        cout << "Sem ranking salvo ainda.\n";
        return;
    }

    PlayerRegistrationAndScore registros[MAX_RANKING];
    int total = 0;
    string line;
    while (getline(arq, line) && total < MAX_RANKING) {
        stringstream ss(line);
        string nome, sscore, data, stempo, resultado; 

        getline(ss, nome, ',');
        getline(ss, sscore, ',');
        getline(ss, data, ',');
        getline(ss, stempo, ',');
        getline(ss, resultado, ','); 

        registros[total].nome = nome;
        registros[total].score = stoi(sscore);
        registros[total].data = data;
        registros[total].tempo = stof(stempo);
        registros[total].resultado = resultado;
        total++;
    }
    arq.close();

    for (int i = 0; i < total - 1; i++) {
        for (int j = 0; j < total - i - 1; j++) {
            if (registros[j].score < registros[j + 1].score) {
                PlayerRegistrationAndScore temp = registros[j];
                registros[j] = registros[j + 1];
                registros[j + 1] = temp;
            }
        }
    }

    system("cls");
    cout << "\n====================== RANKING ======================\n";
    cout << left << setw(12) << "Nome" << setw(10) << "Score" << setw(12) << "Data" << setw(8) << "Tempo" << "Resultado\n";
    for (int i = 0; i < min(total, 10); i++) {
        cout << left << setw(12) << registros[i].nome
             << setw(10) << registros[i].score
             << setw(12) << registros[i].data
             << setw(8) << fixed << setprecision(1) << registros[i].tempo
             << registros[i].resultado << "\n";
    }
    cout << "=====================================================\n";
}