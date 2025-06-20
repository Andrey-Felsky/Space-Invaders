#ifndef MAPA_H
#define MAPA_H
#include <windows.h> // Adicionado para o tipo WORD

#include "../utils/constants.h"
#include <vector>
#include <string>
#include <utility> // For std::pair

struct Player; // Forward declaration to break circular dependency
 
extern char mapa[ALTURA_MAPA][LARGURA_MAPA];
extern bool inimigoVivo[ENEMY_ARRAY_MAX_SIZE];
extern int inimigos[ENEMY_ARRAY_MAX_SIZE][2];

extern bool tiroInimigoAtivo;
extern int tiroInimigoX, tiroInimigoY;

// --- Declaração externa para ShipConfig e seu tipo ---
// Estas definições DEVEM ser idênticas às de main.cpp
enum class ShipType
{
    NONE,
    TYPE_1_FAST_SINGLE,
    TYPE_2_BALANCED_EXTRA,
    TYPE_3_BALANCED_MULTI
};
struct ShipConfig
{
    ShipType type;
    std::string name;
    std::string description;
    std::chrono::milliseconds moveCooldown;
    int initialMaxBullets;
    std::string skin; // Crucial para a skin da nave
    WORD bulletColor; // Cor do tiro da nave do jogador (WORD é o tipo para SetConsoleTextAttribute)
    char bulletChar;  // Caractere do tiro da nave do jogador
    bool initialMultiShotActive;
};
extern ShipConfig chosenShipConfig;

extern bool itemDropActive;
extern int itemDropX, itemDropY;
extern ItemType itemDropType; // Make sure ItemType is known (include constants.h or forward declare)

// Extern declarations for power-up timers for display purposes
extern std::chrono::high_resolution_clock::time_point speedBoostEndTime;
extern std::chrono::high_resolution_clock::time_point extraShotEndTime;
extern std::chrono::high_resolution_clock::time_point multiShotEndTime;
extern std::chrono::high_resolution_clock::time_point enemyFreezeEndTime;
extern GameMode currentGameMode;

void render(const Player players[2], float tempo);

#endif