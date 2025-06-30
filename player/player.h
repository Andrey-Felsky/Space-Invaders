#ifndef PLAYER_H
#define PLAYER_H

#include <vector>
#include <chrono>
#include <string>
#include "../mapa/mapa.h" // For ShipConfig and ALTURA_MAPA

struct Bullet {
    int x, y;
    int dx, dy; // Direction vectors for movement
};

struct Player {
    int id;
    int x, y;
    int vidas;
    int score;
    std::vector<Bullet> bullets;
    int maxBulletsAllowed;
    bool multiShotActive;
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
        y = ALTURA_MAPA - 1;
        vidas = 3;
        score = 0;
        bullets.clear();
        explosionActive = false;
        speedBoostEndTime = std::chrono::high_resolution_clock::now();
        extraShotEndTime = std::chrono::high_resolution_clock::now();
        multiShotEndTime = std::chrono::high_resolution_clock::now();
    }
};

#endif // PLAYER_H