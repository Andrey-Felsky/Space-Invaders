#ifndef PLAYER_H
#define PLAYER_H

#include <vector>
#include <chrono>
#include <string>
#include "../mapa/mapa.h" // For ShipConfig - This is okay now because mapa.h no longer includes player.h

struct Player {
    int id;
    int x;
    int vidas;
    int score;
    int maxBulletsAllowed;
    bool multiShotActive;
    std::vector<std::pair<int, int>> bullets;
    ShipConfig shipConfig;
    std::chrono::high_resolution_clock::time_point lastMoveTime;

    // Power-up timers
    std::chrono::high_resolution_clock::time_point speedBoostEndTime;
    std::chrono::high_resolution_clock::time_point extraShotEndTime;
    std::chrono::high_resolution_clock::time_point multiShotEndTime;

    // Explosion state
    bool explosionActive;
    int explosionX, explosionY;
    std::chrono::high_resolution_clock::time_point explosionStartTime;

    void init(int playerId, int startX) {
        id = playerId;
        x = startX;
        vidas = 3;
        score = 0;
        bullets.clear();
        explosionActive = false;
    }
};

#endif // PLAYER_H