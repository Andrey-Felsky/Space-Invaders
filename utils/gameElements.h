#pragma once
#include <string>
#include <windows.h>

struct GameElements
{
    char spaceship = 'A';
    char wall = char(219);
    char path = ' ';
    char enemy = 'W';
    char shoot = '|';

    int spaceshipColor = 10;
    int wallColor = 9;
    int pathColor = 7;
    int enemyColor = 13; // Alterado para 13 (Light Magenta/Roxo)
    int shootColor = 14;
};

struct PlayerRegistrationAndScore
{
    std::string nome;
    int score;
    std::string data;
    float tempo;
    std::string resultado;
};