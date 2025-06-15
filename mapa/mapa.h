#ifndef MAPA_H
#define MAPA_H

#include "../utils/gameElements.h" // Adicione esta linha aqui para que MAX_ENEMIES seja reconhecido

extern const int largura;
extern const int altura;
extern char mapa[20][30]; // tamanho fixo conforme sua definição

extern bool inimigoVivo[MAX_ENEMIES];
extern float inimigos[MAX_ENEMIES][2];  // Alterado para float

extern bool tiroAtivo;
extern float tiroX, tiroY; // Alterado para float

extern bool tiroInimigoAtivo;
extern float tiroInimigoX, tiroInimigoY; // Alterado para float

extern float naveX; // Alterado para float

void render();

#endif