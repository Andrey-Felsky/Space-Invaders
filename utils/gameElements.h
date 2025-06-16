#pragma once
#define _WIN32_WINNT 0x0600 // **ADICIONE ESTA LINHA AQUI!**
using namespace std;
#include <string>
#include <windows.h> // Mantenha este para outras funcionalidades como Sleep, GetStdHandle

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

    // Cores (Numéricas para SetConsoleTextAttribute - Podem ser removidas se não for usar mais SetConsoleTextAttribute)
    int spaceshipColor = 10;  // verde claro
    int wallColor = 9;        // azul claro
    int pathColor = 7;        // branco padrão
    int enemyColor = 12;      // vermelho claro
    int shootColor = 14;      // amarelo claro
    int enemyShotColor = 13;  // magenta para tiro inimigo

    // **CORES ANSI ESCAPE CODES (ADICIONE OU VERIFIQUE ESTAS!)**
    const char* ANSI_RESET = "\x1b[0m";
    const char* ANSI_RED = "\x1b[31m";
    const char* ANSI_GREEN = "\x1b[32m";
    const char* ANSI_YELLOW = "\x1b[33m";
    const char* ANSI_BLUE = "\x1b[34m";
    const char* ANSI_MAGENTA = "\x1b[35m";
    const char* ANSI_CYAN = "\x1b[36m";
    const char* ANSI_WHITE = "\x1b[37m"; // Para o path/texto padrão
    const char* ANSI_BRIGHT_GREEN = "\x1b[92m"; // Verde claro para a nave
    const char* ANSI_BRIGHT_BLUE = "\x1b[94m"; // Azul claro para a parede
    const char* ANSI_BRIGHT_RED = "\x1b[91m";  // Vermelho claro para o inimigo
    const char* ANSI_BRIGHT_YELLOW = "\x1b[93m"; // Amarelo claro para o tiro
    const char* ANSI_BRIGHT_MAGENTA = "\x1b[95m"; // Magenta para tiro inimigo
};

struct PlayerRegistrationAndScore {
    string nome;
    int score;
    string data;
    float tempo;
};