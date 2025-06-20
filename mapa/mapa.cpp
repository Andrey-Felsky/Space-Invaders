#include "mapa.h"
#include "../utils/gameElements.h"
#include "../logic/logic.h"
#include "../utils/constants.h"
#include <iostream>
#include <windows.h>
#include "../utils/console_utils.h" // Inclui as funções utilitárias de console
#include <iomanip>
#include <string>
#include <sstream>
#include <chrono>

using namespace std;
using namespace std::chrono;

// Presumindo que ShipConfig e chosenShipConfig são declarados externamente
// e acessíveis aqui, conforme configurado anteriormente (em mapa.h e main.cpp)

GameElements gameIcons;
HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);

void render(const Player players[2], float tempo)
{
    resetConsoleColor(); // Garante que a cor padrão seja usada no início da renderização
    auto now = high_resolution_clock::now();

    // --- Linha de Score e Tempo ---
    stringstream ssTempo;
    ssTempo << fixed << setprecision(1) << tempo << "s"; // Use fixed and setprecision for consistent float formatting
    string tempoStr = "Tempo: " + ssTempo.str();

    cout << gameIcons.wall;
    if (currentGameMode == GameMode::SINGLE_PLAYER) {
        string score_str = "Score: " + to_string(players[0].score);
        string mid_section = " " + tempoStr + " ";
        int padding_len = LARGURA_MAPA - score_str.length() - mid_section.length();
        string padding(padding_len > 0 ? padding_len : 0, ' ');
        cout << score_str << padding << mid_section;
    } else { // TWO_PLAYER
        string p1_score_str = "P1: " + to_string(players[0].score);
        string p2_score_str = "P2: " + to_string(players[1].score);
        string mid_section = " " + tempoStr + " ";
        int padding_len = LARGURA_MAPA - p1_score_str.length() - mid_section.length() - p2_score_str.length();
        string padding(padding_len > 0 ? padding_len : 0, ' ');
        cout << p1_score_str << padding << mid_section << p2_score_str;
    }
    cout << gameIcons.wall << "\n";

    cout << gameIcons.wall;
    for (int x = 0; x < LARGURA_MAPA; x++)
        cout << gameIcons.wall;
    cout << gameIcons.wall << "\n";

    for (int y = 0; y < ALTURA_MAPA; y++)
    {
        for (int x = 0; x < LARGURA_MAPA; x++)
        {
            mapa[y][x] = gameIcons.path;
        }
    }

    // Desenha as barreiras no mapa
    for (int i = 0; i < NUM_BARRIERS; ++i) {
        for (int r = 0; r < BARRIER_HEIGHT; ++r) {
            for (int c = 0; c < BARRIER_WIDTH; ++c) {
                // Só desenha a parte da barreira que não foi destruída (' ') ou que não é nula
                if (barriers[i].shape[r][c] != ' ' && barriers[i].shape[r][c] != '\0') {
                    int mapX = barriers[i].x + c;
                    int mapY = barriers[i].y + r;
                    if (mapY >= 0 && mapY < ALTURA_MAPA && mapX >= 0 && mapX < LARGURA_MAPA) {
                        mapa[mapY][mapX] = barriers[i].shape[r][c];
                    }
                }
            }
        }
    }

    for (int i = 0; i < TOTAL_INITIAL_ENEMIES; i++)
    {
        if (inimigoVivo[i])
        {
            mapa[inimigos[i][1]][inimigos[i][0]] = gameIcons.enemy;
        }
    }

    // Draw bullets for both players
    for (int i = 0; i < 2; ++i) {
        for (const auto &bullet : players[i].bullets) {
            if (bullet.second >= 0 && bullet.second < ALTURA_MAPA && bullet.first >= 0 && bullet.first < LARGURA_MAPA)
                mapa[bullet.second][bullet.first] = players[i].shipConfig.bulletChar;
        }
    }

    if (tiroInimigoAtivo && tiroInimigoY >= 0 && tiroInimigoY < ALTURA_MAPA)
        mapa[tiroInimigoY][tiroInimigoX] = '|';

    // Render player ships
    int numPlayersToRender = (currentGameMode == GameMode::SINGLE_PLAYER) ? 1 : 2;
    for (int i = 0; i < numPlayersToRender; ++i) {
        if (players[i].vidas > 0) { // Only render if alive
            if (!players[i].shipConfig.skin.empty()) {
                int startX = players[i].x - players[i].shipConfig.skin.length() / 2;
                for (size_t j = 0; j < players[i].shipConfig.skin.length(); ++j) {
                    if (startX + j >= 0 && startX + j < LARGURA_MAPA) {
                        mapa[ALTURA_MAPA - 1][startX + j] = players[i].shipConfig.skin[j];
                    }
                }
            }
        }
    } // --- Fim da renderização da skin da nave ---

    if (explosionActiveEnemy)
    {
        // A lógica de desativar a explosão foi movida para updateExplosions() em logic.cpp
        mapa[explosionEnemyY][explosionEnemyX] = 'X';
    }

    // Render player explosions
    for (int i = 0; i < 2; ++i) {
        if (players[i].explosionActive) {
            mapa[players[i].explosionY][players[i].explosionX] = '@';
        }
    }

    if (itemDropActive && itemDropY < ALTURA_MAPA && itemDropX < LARGURA_MAPA)
    {
        char itemChar = '?';
        // Colors can be associated with item types for better visual feedback
        switch (itemDropType)
        {
        case ItemType::EXTRA_LIFE:
            itemChar = 'L';
            break; // Life
        case ItemType::SPEED_BOOST:
            itemChar = 'S';
            break; // Speed
        case ItemType::EXTRA_SHOT:
            itemChar = 'E';
            break; // Extra shot
        case ItemType::MULTI_SHOT:
            itemChar = 'M';
            break; // Multi shot
        case ItemType::BONUS_POINTS:
            itemChar = '$';
            break; // Points
        case ItemType::ENEMY_FREEZE:
            itemChar = 'F';
            break; // Freeze
        default:
            itemChar = '+';
            break;
        }
        if (itemDropX >= 0 && itemDropX < LARGURA_MAPA && itemDropY >= 0 && itemDropY < ALTURA_MAPA)
            mapa[itemDropY][itemDropX] = itemChar;
    }

    for (int y = 0; y < ALTURA_MAPA; y++)
    {
        cout << gameIcons.wall;
        for (int x = 0; x < LARGURA_MAPA; x++)
        {
            char c = mapa[y][x];

            int player_index_at_char = -1;
            // Check if the character is part of a player's ship
            for (int i = 0; i < 2; ++i) {
                if (players[i].vidas > 0 && y == ALTURA_MAPA - 1 && !players[i].shipConfig.skin.empty()) {
                    int skinLength = players[i].shipConfig.skin.length();
                    int startX = players[i].x - skinLength / 2;
                    if (x >= startX && x < startX + skinLength) {
                        player_index_at_char = i;
                        break;
                    }
                }
            }


            // Lógica de coloração unificada para evitar que uma cor sobrescreva a outra. (Prioridade: Nave > Inimigo > Tiro > Item)
            if (player_index_at_char != -1) {
                setConsoleColor(players[player_index_at_char].shipConfig.bulletColor); // Cor da nave
            } else if (c == gameIcons.enemy) {
                setConsoleColor(gameIcons.enemyColor); // Agora esta cor será aplicada corretamente.
            } else if (c == players[0].shipConfig.bulletChar) { // Tiro do P1
                setConsoleColor(players[0].shipConfig.bulletColor);
            } else if (currentGameMode == GameMode::TWO_PLAYER && c == players[1].shipConfig.bulletChar) { // Tiro do P2 (apenas em multiplayer)
                setConsoleColor(players[1].shipConfig.bulletColor);
            }
            else if (c == '|')
            {
                setConsoleColor(12); // Tiro do inimigo (Vermelho)
            }
            else if (c == 'X' || c == '@')
            {
                setConsoleColor(6); // Explosão (Amarelo escuro/Laranja)
            }
            else if (c == (char)219) // Caractere da barreira
            {
                setConsoleColor(10); // Verde para as barreiras
            }
            else if (c == 'L')
            {
                setConsoleColor(10); // Vida (Verde)
            }
            else if (c == 'S')
            {
                setConsoleColor(14); // Velocidade (Amarelo)
            }
            else if (c == '$')
            {
                setConsoleColor(12); // Pontos (Vermelho claro)
            }
            else if (c == 'F')
            {
                setConsoleColor(3); // Freeze (Ciano)
            }
            else if (c == 'E' || c == 'M' || c == '+' || c == '?')
            {
                setConsoleColor(11); // Outros itens (Ciano claro)
            }
            else
            {
                setConsoleColor(7); // Cor padrão para o caminho/fundo
            }

            cout << c;
        }
        resetConsoleColor(); // Reseta a cor para o padrão após cada linha
        cout << gameIcons.wall << "\n";
    }

    cout << gameIcons.wall;
    for (int x = 0; x < LARGURA_MAPA; x++)
        cout << gameIcons.wall;
    cout << gameIcons.wall << "\n";

    // --- Status Bar ---
    if (currentGameMode == GameMode::SINGLE_PLAYER) {
        // --- Vidas (Single Player) ---
        cout << gameIcons.wall;
        int line_len = 0;
        string vidas_label = "Vidas: ";
        setConsoleColor(12); cout << vidas_label; line_len += vidas_label.length();
        for(int i = 0; i < players[0].vidas; ++i) {
            cout << string(1, char(254)) << " ";
            line_len += 2;
        }
        resetConsoleColor();
        cout << string(LARGURA_MAPA - line_len > 0 ? LARGURA_MAPA - line_len : 0, ' ');
        cout << gameIcons.wall << "\n";

        // --- Power-ups (Single Player) ---
        cout << gameIcons.wall;
        resetConsoleColor(); // Cor padrão
        stringstream ss; // Usado para construir partes da string
        string label = "Ativos: ";
        cout << label;
        int powerup_line_length = label.length();
        bool hasActiveItem = false;

        long speed_seconds = duration_cast<seconds>(players[0].speedBoostEndTime - now).count();
        if (speed_seconds > 0) {
            ss.str(""); ss << "[Velocidade:" << speed_seconds << "s] "; string item_str = ss.str();
            setConsoleColor(14); cout << item_str; resetConsoleColor(); powerup_line_length += item_str.length(); hasActiveItem = true;
        }
        long extra_shot_seconds = duration_cast<seconds>(players[0].extraShotEndTime - now).count();
        if (players[0].maxBulletsAllowed > players[0].shipConfig.initialMaxBullets && extra_shot_seconds > 0) {
            ss.str(""); ss << "[Tiro Extra:" << extra_shot_seconds << "s] "; string item_str = ss.str();
            setConsoleColor(11); cout << item_str; resetConsoleColor(); powerup_line_length += item_str.length(); hasActiveItem = true;
        }
        long multi_shot_seconds = duration_cast<seconds>(players[0].multiShotEndTime - now).count();
        if (players[0].multiShotActive && !players[0].shipConfig.initialMultiShotActive && multi_shot_seconds > 0) {
            ss.str(""); ss << "[Tiro Multi:" << multi_shot_seconds << "s] "; string item_str = ss.str();
            setConsoleColor(13); cout << item_str; resetConsoleColor(); powerup_line_length += item_str.length(); hasActiveItem = true;
        }
        long freeze_seconds = duration_cast<seconds>(enemyFreezeEndTime - now).count();
        if (freeze_seconds > 0) {
            if (hasActiveItem) { ss.str(""); ss << " "; string item_str = ss.str(); cout << item_str; powerup_line_length += item_str.length(); }
            string freeze_status = "[Congelado:" + to_string(freeze_seconds) + "s]";
            powerup_line_length += freeze_status.length();
            setConsoleColor(9); cout << freeze_status; resetConsoleColor();
            hasActiveItem = true;
        }
        if (!hasActiveItem) {
            string none_str = "Nenhum";
            cout << none_str;
            powerup_line_length += none_str.length();
        }
        cout << string(LARGURA_MAPA - powerup_line_length > 0 ? LARGURA_MAPA - powerup_line_length : 0, ' ');
        cout << gameIcons.wall << "\n";

    } else { // TWO_PLAYER
        // --- Player 1 Status Line ---
        cout << gameIcons.wall;
        int p1_line_len = 0;
        string p1_vidas_label = "P1 Vidas: ";
        setConsoleColor(12); cout << p1_vidas_label; p1_line_len += p1_vidas_label.length();
        for(int i = 0; i < players[0].vidas; ++i) { cout << string(1, char(254)) << " "; p1_line_len += 2; }
        resetConsoleColor();
        long p1_extra_shot_seconds = duration_cast<seconds>(players[0].extraShotEndTime - now).count();
        if (players[0].maxBulletsAllowed > players[0].shipConfig.initialMaxBullets && p1_extra_shot_seconds > 0) {
            string s = "| "; setConsoleColor(7); cout << s; p1_line_len += s.length();
            s = "Tiro Extra:" + to_string(p1_extra_shot_seconds) + "s "; setConsoleColor(11); cout << s; p1_line_len += s.length(); resetConsoleColor();
        }
        long p1_multi_shot_seconds = duration_cast<seconds>(players[0].multiShotEndTime - now).count();
        if (players[0].multiShotActive && !players[0].shipConfig.initialMultiShotActive && p1_multi_shot_seconds > 0) {
            string s = "| "; setConsoleColor(7); cout << s; p1_line_len += s.length();
            s = "Tiro Multi:" + to_string(p1_multi_shot_seconds) + "s "; setConsoleColor(13); cout << s; p1_line_len += s.length(); resetConsoleColor();
        }
        cout << string(LARGURA_MAPA - p1_line_len > 0 ? LARGURA_MAPA - p1_line_len : 0, ' ');
        cout << gameIcons.wall << "\n";

        // --- Player 2 Status Line ---
        cout << gameIcons.wall;
        int p2_line_len = 0;
        string p2_vidas_label = "P2 Vidas: ";
        setConsoleColor(12); cout << p2_vidas_label; p2_line_len += p2_vidas_label.length();
        for(int i = 0; i < players[1].vidas; ++i) { cout << string(1, char(254)) << " "; p2_line_len += 2; }
        resetConsoleColor();
        long p2_extra_shot_seconds = duration_cast<seconds>(players[1].extraShotEndTime - now).count();
        if (players[1].maxBulletsAllowed > players[1].shipConfig.initialMaxBullets && p2_extra_shot_seconds > 0) {
            string s = "| "; setConsoleColor(7); cout << s; p2_line_len += s.length();
            s = "Tiro Extra:" + to_string(p2_extra_shot_seconds) + "s "; setConsoleColor(11); cout << s; p2_line_len += s.length(); resetConsoleColor();
        }
        long p2_multi_shot_seconds = duration_cast<seconds>(players[1].multiShotEndTime - now).count();
        if (players[1].multiShotActive && !players[1].shipConfig.initialMultiShotActive && p2_multi_shot_seconds > 0) {
            string s = "| "; setConsoleColor(7); cout << s; p2_line_len += s.length();
            s = "Tiro Multi:" + to_string(p2_multi_shot_seconds) + "s "; setConsoleColor(13); cout << s; p2_line_len += s.length(); resetConsoleColor();
        }
        cout << string(LARGURA_MAPA - p2_line_len > 0 ? LARGURA_MAPA - p2_line_len : 0, ' ');
        cout << gameIcons.wall << "\n";
    }

    // --- Global Status Line (Freeze) ---
    cout << gameIcons.wall;
    int global_line_len = 0;
    long freeze_seconds = duration_cast<seconds>(enemyFreezeEndTime - now).count();
    if (freeze_seconds > 0) {
        string freeze_status = "Inimigos Congelados: " + to_string(freeze_seconds) + "s";
        setConsoleColor(9); cout << freeze_status; resetConsoleColor();
        global_line_len += freeze_status.length();
    }
    cout << string(LARGURA_MAPA - global_line_len > 0 ? LARGURA_MAPA - global_line_len : 0, ' ');
    cout << gameIcons.wall << "\n";

    cout << gameIcons.wall;
    for (int x = 0; x < LARGURA_MAPA; x++)
        cout << gameIcons.wall;
    cout << gameIcons.wall << "\n";

    resetConsoleColor();
}