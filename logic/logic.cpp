#include "logic.h"
#include "../enemy/enemy.h"
#include <iostream>

void updateTire()
{
    if (tiroAtivo)
    {
        tiroY--;
        if (tiroY < 0)
        {
            tiroAtivo = false;
        }
    }
    if (tiroInimigoAtivo)
    {
        tiroInimigoY++;
        if (tiroInimigoY >= altura)
        {
            tiroAtivo = false;
        }
    }
}

void dispararTiroInimigo()
{
    // só dispara se não houver outro tiro inimigo ativo
    if (tiroInimigoAtivo)
        return;

    // conta quantos inimigos vivos existem
    int vivos[5];
    int count = 0;
    for (int i = 0; i < 5; i++)
    {
        if (inimigoVivo[i])
        {
            vivos[count++] = i; // salva o índice do inimigo vivo
        }
    }

    if (count == 0)
        return; // nenhum inimigo vivo, não dispara

    // sorteia um dos vivos
    int sorteado = vivos[rand() % count];

    // define a posição do tiro inimigo com base na posição do inimigo sorteado
    tiroInimigoX = inimigos[sorteado][0];
    tiroInimigoY = inimigos[sorteado][1] + 1; // logo abaixo do inimigo
    tiroInimigoAtivo = true;
}

void checkCollisions()
{
    for (int i = 0; i < 5; i++)
    {
        if (inimigoVivo[i] && tiroAtivo &&
            tiroX == inimigos[i][0] && tiroY == inimigos[i][1])
        {
            inimigoVivo[i] = false;
            tiroAtivo = false;
            score += 10;
        }
    }
}

void checkEndOfGame()
{
    // derrota se algum inimigo chegou na linha da nave
    for (int i = 0; i < 5; i++)
    {
        if (inimigoVivo[i] && inimigos[i][1] >= 19)
        {
            gameOver = true;
            // std::cout << "Você perdeu! Inimigos invadiram a base.\n";
        }
    }

    // vitória se todos mortos
    bool venceu = true;
    for (int i = 0; i < 5; i++)
    {
        if (inimigoVivo[i])
        {
            venceu = false;
            break;
        }
    }

    if (venceu)
    {
        gameOver = true;
        std::cout << "Parabéns, você venceu!\n";
    }
}
