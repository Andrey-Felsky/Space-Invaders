#ifndef CONSTANTS_H
#define CONSTANTS_H

#include <chrono>

const int LARGURA_MAPA = 30;
const int ALTURA_MAPA = 20;

const int NUM_ENEMY_COLS_INIT = 7;
const int NUM_ENEMY_ROWS_INIT = 3;
const int ENEMY_INIT_SPACING_X = 4;
const int ENEMY_INIT_SPACING_Y = 2;
const int TOTAL_INITIAL_ENEMIES = NUM_ENEMY_COLS_INIT * NUM_ENEMY_ROWS_INIT;

const int ENEMY_ARRAY_MAX_SIZE = 25;

const std::chrono::milliseconds EXPLOSION_DURATION(100);

const std::chrono::milliseconds ITEM_MOVE_INTERVAL(100);

// Enum for item types
enum class ItemType {
    EXTRA_LIFE,
    SPEED_BOOST,    // Player ship speed or enemy slow down? User asked for enemy freeze, so this is likely enemy speed.
                    // The request was "Aumento de Velocidade", which is ambiguous.
                    // Given "Congela movimento dos inimigos", "Aumento de Velocidade" could mean player shoot speed, player move speed, or enemy move speed increase (making it harder).
                    // Let's assume it means the enemies speed up temporarily, making it a challenge pickup.
                    // Re-reading: "Aumento de Velocidade por 8 segundos" - this sounds like a player buff. Let's make it player fire rate or bullet speed.
                    // The user's previous request was "Ao matar naves inimigas os itens devem cair... o Aumento de Velocidade por 8 segundos".
                    // Let's interpret "Aumento de Velocidade" as faster player bullets for now, or faster player movement.
                    // The prompt also says "Cogela movimento dos inimigos por 2 segundos".
                    // Let's make "Aumento de Velocidade" increase player's bullet speed.
    EXTRA_SHOT,     // More bullets on screen at once
    MULTI_SHOT,     // Multiple bullets fired at once (e.g., spread shot)
    BONUS_POINTS,
    ENEMY_FREEZE
};

// Item effect constants
const int BONUS_POINTS_AMOUNT = 100;
const int EXTRA_LIFE_AMOUNT = 1;

const std::chrono::seconds SPEED_BOOST_DURATION(8);
const std::chrono::seconds EXTRA_SHOT_DURATION(6);
const int EXTRA_SHOT_MAX_BULLETS = 3; // Max bullets on screen with this power-up
const std::chrono::seconds MULTI_SHOT_DURATION(5);
const std::chrono::seconds ENEMY_FREEZE_DURATION(2);

// Difficulty settings
enum class Difficulty {
    FACIL,
    NORMAL,
    DIFICIL,
    AUTO
};

// Base values that will be modified by difficulty
const std::chrono::milliseconds BASE_ENEMY_MOVE_INTERVAL(500);
const int BASE_ITEM_DROP_CHANCE = 25; // % chance

#endif