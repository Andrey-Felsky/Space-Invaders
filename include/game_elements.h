#pragma once

using namespace std;

#include <sstream>
#include <string>
#include <windows.h>

const int w_height = 44;
const int w_width = 88;

bool gameRunning = true;

int key;

struct GameElements {
    char person = char(36);
    char wall = char(219);
    char path = ' ';
    char dark = '.';
    char newPhase = 'H';
    char door = char(35);
    char door2 = char(35);
    char bossMap = char(33); 
    char key = char(42); //*
    char item = char(38); //&
    char trap = char(63); //?
    char enemy = char(64); //@
    char bossIcon = char(37);
};