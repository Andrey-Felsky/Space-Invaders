#ifndef LOGICA_H
#define LOGICA_H

extern const int altura;

extern bool tiroAtivo;
extern bool tiroInimigoAtivo;
extern int tiroX, tiroY;
extern int tiroInimigoX, tiroInimigoY;

extern int naveX;
extern int score;
extern bool gameOver;

void updateTire();
void dispararTiroInimigo();
void checkCollisions();
void checkEndOfGame();

#endif
