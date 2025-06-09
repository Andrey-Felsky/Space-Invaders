#include "utils/gameElements.h"
#include "ranking/score.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <ctime>
#include <iomanip>
#include <algorithm>
using namespace std;

const int MAX_RANKING = 100; // limite de registros
const string path = "saves/scores.txt";

void saveScore(const string &nome, int score, float tempo)
{
    ofstream arq(path, ios::app); 
    if (!arq.is_open())
        return;

    // data atual
    time_t t = time(0);
    tm *now = localtime(&t);
    char buffer[11];
    strftime(buffer, sizeof(buffer), "%d/%m/%Y", now);

    arq << nome << "," << score << "," << buffer << "," << tempo << "\n";
    arq.close();
}

void drawnRanking()
{
    ifstream arq(path);
    if (!arq.is_open())
    {
        cout << "Sem ranking salvo ainda.\n";
        return;
    }

    PlayerRegistrationAndScore registros[MAX_RANKING];
    int total = 0;
    string linha;

    while (getline(arq, linha) && total < MAX_RANKING)
    {
        stringstream ss(linha);
        string nome, sscore, data, stempo;
        getline(ss, nome, ',');
        getline(ss, sscore, ',');
        getline(ss, data, ',');
        getline(ss, stempo);

        registros[total].nome = nome;
        registros[total].score = stoi(sscore);
        registros[total].data = data;
        registros[total].tempo = stof(stempo);
        total++;
    }
    arq.close();

    // ordena manualmente (bubble sort para score decrescente)
    for (int i = 0; i < total - 1; i++)
    {
        for (int j = 0; j < total - i - 1; j++)
        {
            if (registros[j].score < registros[j + 1].score)
            {
                PlayerRegistrationAndScore temp = registros[j];
                registros[j] = registros[j + 1];
                registros[j + 1] = temp;
            }
        }
    }

    // imprime
    system("cls");
    cout << "\n=============== RANKING ===============\n";
    cout << left << setw(12) << "Nome" << setw(10) << "Score" << setw(12) << "Data" << "Tempo\n";
    for (int i = 0; i < total && i < 10; i++)
    {
        cout << setw(12) << registros[i].nome
             << setw(10) << registros[i].score
             << setw(12) << registros[i].data
             << fixed << setprecision(2) << registros[i].tempo << "s\n";
    }
}
