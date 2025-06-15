#pragma once
using namespace std;
#include <string>
#include <windows.h>

const int MAX_ENEMIES = 20; // Manter o MAX_ENEMIES para 2 fileiras

// Novas constantes para velocidade (unidades por segundo)
const float PLAYER_SPEED = 15.0f; // Ex: 15 células por segundo
const float ENEMY_SPEED_X = 5.0f; // Ex: 5 células por segundo horizontalmente
const float ENEMY_SPEED_Y = 1.0f; // Ex: 1 célula por segundo para descer
const float PLAYER_SHOT_SPEED = 25.0f; // Ex: 25 células por segundo
const float ENEMY_SHOT_SPEED = 10.0f; // Ex: 10 células por segundo

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