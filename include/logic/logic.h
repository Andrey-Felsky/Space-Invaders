#pragma once

extern bool tiroAtivo;
extern int tiroX, tiroY;

extern int naveX;
extern int score;
extern bool gameOver;

void updateTire();
void checkCollisions();
void checkEndOfGame();