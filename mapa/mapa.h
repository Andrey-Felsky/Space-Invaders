#ifndef MAPA_H
#define MAPA_H

extern const int largura;
extern const int altura;
extern char mapa[20][30]; // tamanho fixo conforme sua definição

extern bool inimigoVivo[5];
extern int inimigos[5][2];

extern bool tiroAtivo;
extern int tiroX, tiroY;

extern int naveX;

void render();

#endif
