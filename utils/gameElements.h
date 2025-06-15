#pragma once
using namespace std;
#include <string>
#include <windows.h>

// New constant for maximum enemies
const int MAX_ENEMIES = 8; // Increased from 5 to 8

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