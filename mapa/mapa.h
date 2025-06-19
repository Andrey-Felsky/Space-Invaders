#ifndef MAPA_H
#define MAPA_H

#include "../utils/constants.h"

extern char mapa[ALTURA_MAPA][LARGURA_MAPA];

extern bool inimigoVivo[ENEMY_ARRAY_MAX_SIZE];
extern int inimigos[ENEMY_ARRAY_MAX_SIZE][2];

extern bool tiroAtivo;
extern int tiroX, tiroY;

extern bool tiroInimigoAtivo;
extern int tiroInimigoX, tiroInimigoY;

extern int naveX;

void render(int score, float tempo, int currentVidas);

#endif