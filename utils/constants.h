#ifndef CONSTANTS_H
#define CONSTANTS_H

#include <chrono>

const int LARGURA_MAPA = 30;
const int ALTURA_MAPA = 20;

const int NUM_ENEMY_COLS_INIT = 7;
const int NUM_ENEMY_ROWS_INIT = 3;
const int ENEMY_INIT_SPACING_X = 4;
const int ENEMY_INIT_SPACING_Y = 2;
const int TOTAL_INITIAL_ENEMIES = NUM_ENEMY_COLS_INIT * NUM_ENEMY_ROWS_INIT;

const int ENEMY_ARRAY_MAX_SIZE = 25;

const std::chrono::milliseconds EXPLOSION_DURATION(100);

const int ITEM_DROP_CHANCE = 20;
const std::chrono::milliseconds ITEM_MOVE_INTERVAL(100);

#endif