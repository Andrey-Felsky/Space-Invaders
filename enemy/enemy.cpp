#include "enemy.h"
#include "../mapa/mapa.h" // Adicione esta linha
#include "../utils/gameElements.h" // Ensure this path is correct

int dirInimigo = 1;

void initEnemy() {
    // Initial positioning for MAX_ENEMIES enemies
    // Adjust this logic if you want different enemy layouts
    for (int i = 0; i < MAX_ENEMIES; i++) {
        inimigos[i][0] = 2 + (i % (largura - 4)) * (largura / (MAX_ENEMIES > (largura-4) ? (largura-4) : MAX_ENEMIES)); // Distribute X evenly, avoiding edges
        inimigos[i][1] = 2 + (i / (largura - 4)); // Start enemies on different rows if MAX_ENEMIES > (largura-4)
        if (inimigos[i][0] >= largura - 1) inimigos[i][0] = largura - 2; // Prevent going out of bounds
        if (inimigos[i][0] <= 0) inimigos[i][0] = 1; // Prevent going out of bounds
        inimigoVivo[i] = true;
    }
}

void moveEnemies() {
    for (int i = 0; i < MAX_ENEMIES; i++) { // Changed from 5 to MAX_ENEMIES
        inimigos[i][0] += dirInimigo;
    }

    // inverter direção se algum bater na borda
    for (int i = 0; i < MAX_ENEMIES; i++) { // Changed from 5 to MAX_ENEMIES
        if (inimigoVivo[i] && (inimigos[i][0] <= 0 || inimigos[i][0] >= largura - 1)) { // Adjusted boundary check
            dirInimigo *= -1;
            for (int j = 0; j < MAX_ENEMIES; j++) { // Changed from 5 to MAX_ENEMIES
                inimigos[j][1]++; // desce todos
            }
            break; // Break after first collision to prevent multiple direction changes in one frame
        }
    }
}