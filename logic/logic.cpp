#include "logic.h"
#include "../enemy/enemy.h" 
#include <iostream>

void updateTire() {
    if (tiroAtivo) {
        tiroY--;
        if (tiroY < 0) {
            tiroAtivo = false;
        }
    }
}

void checkCollisions() {
    for (int i = 0; i < 5; i++) {
        if (inimigoVivo[i] && tiroAtivo &&
            tiroX == inimigos[i][0] && tiroY == inimigos[i][1]) {
            inimigoVivo[i] = false;
            tiroAtivo = false;
            score += 10;
        }
    }
}

void checkEndOfGame() {
    // derrota se algum inimigo chegou na linha da nave
    for (int i = 0; i < 5; i++) {
        if (inimigoVivo[i] && inimigos[i][1] >= 19) {
            gameOver = true;
            std::cout << "Você perdeu! Inimigos invadiram a base.\n";
        }
    }

    // vitória se todos mortos
    bool venceu = true;
    for (int i = 0; i < 5; i++) {
        if (inimigoVivo[i]) {
            venceu = false;
            break;
        }
    }

    if (venceu) {
        gameOver = true;
        std::cout << "Parabéns, você venceu!\n";
    }
}
