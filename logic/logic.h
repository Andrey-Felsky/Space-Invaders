#ifndef LOGICA_H
#define LOGICA_H

extern bool tiroAtivo;
extern int tiroX, tiroY;

extern int naveX;
extern int score;
extern bool gameOver;


extern bool tiroInimigoAtivo;
extern int tiroInimigoX, tiroInimigoY;

extern const int largura;
extern const int altura;


void updateTire();
void checkCollisions();
void checkEndOfGame();
void updateTiroInimigo();

#endif
