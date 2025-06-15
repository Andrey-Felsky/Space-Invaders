#ifndef MAPA_H
#define MAPA_H

#include "../utils/gameElements.h" // Adicione esta linha aqui para que MAX_ENEMIES seja reconhecido

extern const int largura;
extern const int altura; // Corrected line
extern char mapa[20][30]; // tamanho fixo conforme sua definição

extern bool inimigoVivo[MAX_ENEMIES]; // Agora MAX_ENEMIES será reconhecido
extern int inimigos[MAX_ENEMIES][2];  // Agora MAX_ENEMIES será reconhecido

extern bool tiroAtivo;
extern int tiroX, tiroY;

extern bool tiroInimigoAtivo;
extern int tiroInimigoX, tiroInimigoY;

extern int naveX;

void render();

#endif