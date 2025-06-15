#ifndef LOGICA_H
#define LOGICA_H

#include "../utils/gameElements.h" // Para as constantes de velocidade e MAX_ENEMIES

extern bool tiroAtivo;
extern float tiroX, tiroY; // Alterado para float

extern float naveX; // Alterado para float
extern int score;
extern bool gameOver;

extern bool tiroInimigoAtivo;
extern float tiroInimigoX, tiroInimigoY; // Alterado para float

extern const int largura;
extern const int altura;

extern float deltaTime; // Adicionar para acessar deltaTime

void updateTire();
void checkCollisions();
void checkEndOfGame();
void updateTiroInimigo();

#endif