#include "enemy.h"
#include "../mapa/mapa.h"
#include "../utils/gameElements.h"
#include <cmath>

extern float deltaTime; // Acessa o deltaTime global

int dirInimigo = 1;

// Inicializa o timer de descida
float enemyDescendTimer = 0.0f;
// Define o intervalo de tempo para os inimigos descerem uma linha (em segundos)
// Por exemplo, 2.0f significa que eles descem a cada 2 segundos.
const float ENEMY_DESCEND_INTERVAL = 1.0f; // Ajuste este valor (menor = mais frequente)


void initEnemy() {
    const int enemiesPerLogicalRow = 10;
    const float startXOffset = 2.0f;
    const float startYOffset = 2.0f;
    const float rowSpacing = 2.0f;

    for (int i = 0; i < MAX_ENEMIES; i++) {
        int col = i % enemiesPerLogicalRow;
        int row = i / enemiesPerLogicalRow;

        inimigos[i][0] = startXOffset + col * ((largura - startXOffset * 2) / (float)enemiesPerLogicalRow);
        inimigos[i][1] = startYOffset + row * rowSpacing;

        if (inimigos[i][0] >= largura - 1) inimigos[i][0] = largura - 1.01f;
        if (inimigos[i][0] <= 0) inimigos[i][0] = 0.01f;

        inimigoVivo[i] = true;
    }
    enemyDescendTimer = 0.0f; // Reinicia o timer ao iniciar o jogo
}

void moveEnemies() {
    bool hitEdge = false;
    float moveDistance = ENEMY_SPEED_X * deltaTime;

    for (int i = 0; i < MAX_ENEMIES; i++) {
        if (inimigoVivo[i]) {
            inimigos[i][0] += dirInimigo * moveDistance;

            if (inimigos[i][0] <= 0.0f || inimigos[i][0] >= largura - 1.0f) {
                hitEdge = true;
            }
        }
    }

    if (hitEdge) {
        dirInimigo *= -1; // Inverte a direção de todos
        // A descida AGORA É GERENCIADA PELO TIMER abaixo, não mais aqui diretamente.
        // Se você quiser que eles desçam imediatamente ao bater na parede *além* do timer,
        // pode adicionar um pequeno incremento aqui:
        // float immediateDescend = 0.5f; // Desce 0.5 unidades imediatamente
        // for (int i = 0; i < MAX_ENEMIES; i++) {
        //     if (inimigoVivo[i]) {
        //         inimigos[i][1] += immediateDescend;
        //     }
        // }
    }

    // Lógica de descida baseada em tempo
    enemyDescendTimer += deltaTime;
    if (enemyDescendTimer >= ENEMY_DESCEND_INTERVAL) {
        float descendDistance = 1.0f; // Desce 1 unidade completa
        for (int i = 0; i < MAX_ENEMIES; i++) {
            if (inimigoVivo[i]) {
                inimigos[i][1] += descendDistance;
            }
        }
        enemyDescendTimer -= ENEMY_DESCEND_INTERVAL; // Reseta o timer (subtrai para manter o excesso de tempo)
    }
}