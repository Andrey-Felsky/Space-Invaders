#ifndef MAPA_H
#define MAPA_H

#include "../utils/constants.h"
#include <vector>
#include <utility> // For std::pair

extern char mapa[ALTURA_MAPA][LARGURA_MAPA];
extern std::vector<std::pair<int, int>> playerBullets;
extern bool inimigoVivo[ENEMY_ARRAY_MAX_SIZE];
extern int inimigos[ENEMY_ARRAY_MAX_SIZE][2];
 
extern bool tiroInimigoAtivo;
extern int tiroInimigoX, tiroInimigoY;

extern int naveX;

extern bool itemDropActive;
extern int itemDropX, itemDropY;
extern ItemType itemDropType; // Make sure ItemType is known (include constants.h or forward declare)

// Extern declarations for power-up timers for display purposes
extern std::chrono::high_resolution_clock::time_point speedBoostEndTime;
extern std::chrono::high_resolution_clock::time_point extraShotEndTime;
extern std::chrono::high_resolution_clock::time_point multiShotEndTime;
extern std::chrono::high_resolution_clock::time_point freezeEndTime;
extern int maxPlayerBulletsAllowed; // For displaying status of extra shot

void render(int score, float tempo, int currentVidas);

#endif