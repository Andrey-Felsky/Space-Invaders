#pragma once
using namespace std;
#include <windows.h>
#include <string>

struct GameElements {
    //Simbolos
    char spaceship = 'A';  
    char wall = char(219);   
    char path = ' '; 
    char enemy = 'W'; 
    char shoot = '|';

    // Cores
    int spaceshipColor = 10;  // verde claro
    int wallColor = 9;        // azul claro
    int pathColor = 7;        // branco padrão
    int enemyColor = 12;      // vermelho claro
    int shootColor = 14;      // amarelo claro
};

struct PlayerRegistrationAndScore {
    string nome;
    int score;
    string data;
    float tempo;
};