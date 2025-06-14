#ifndef GAMEELEMENTS_H
#define GAMEELEMENTS_H

// Impede conflito do 'byte'
#define NOMINMAX
#define byte win_byte_override
#include <windows.h>
#undef byte

#include <string>

struct PlayerRegistrationAndScore {
    std::string nome;
    int score;
    std::string data;
    float tempo;
};

struct GameElements {
    char enemy = 'X';
    char shoot = '|';
    char spaceship = '^';
    char path = ' ';
    char wall = '#';

    int enemyColor = 12;
    int shootColor = 11;
    int spaceshipColor = 10;
};

#endif
