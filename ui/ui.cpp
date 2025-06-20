#include "ui.h"
#include <iostream>
#include <windows.h>
#include <iomanip>
#include "../utils/cleanScreen/cleanScreen.h"
#include "../ranking/score.h" // Para resetColor()

// Função auxiliar para definir a cor do console
void setColor(int color)
{
    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), color);
}

void showVictoryScreen(int score, float time)
{
    cleanScreen();
    setColor(10); // Verde Claro
    std::cout << R"(
 __   __  _______  __   __  _______  _______  ______    _______  _______
|  | |  ||       ||  | |  ||       ||   _   ||    _ |  |       ||       |
|  |_|  ||   _   ||  | |  ||    ___||  |_|  ||   | ||  |_     _||   _   |
|       ||  | |  ||  |_|  ||   |___ |       ||   |_||_   |   |  |  | |  |
|       ||  |_|  ||       ||    ___||       ||    __  |  |   |  |  |_|  |
|   _   ||       ||       ||   |    |   _   ||   |  | |  |   |  |       |
|__| |__||_______||_______||___|    |__| |__||___|  |_|  |___|  |_______|

)";
    setColor(14); // Amarelo
    std::cout << "            Voce defendeu a galaxia com sucesso!\n\n";

    setColor(7); // Branco
    std::cout << "                  Score Final: " << score << "\n";
    std::cout << "                  Tempo: " << std::fixed << std::setprecision(1) << time << "s\n\n";

    resetColor();
}

void showDefeatScreen(int score, float time)
{
    cleanScreen();
    setColor(12); // Vermelho Claro
    std::cout << R"(
 _____   _______  _______  _______  _______  _______  _______
|  _  | |       ||   _   ||       ||       ||   _   ||       |
| | | | |    ___||  |_|  ||    ___||    ___||  |_|  ||_     _|
| | | | |   |___ |       ||   | __ |   |___ |       |  |   |
| |_| | |    ___||       ||   ||  ||    ___||       |  |   |
|     | |   |    |   _   ||   |_| ||   |    |   _   |  |   |
|_____| |___|    |__| |__||_______||___|    |__| |__|  |___|

)";
    setColor(14); // Amarelo
    std::cout << "                A frota inimiga dominou...\n\n";

    setColor(7); // Branco
    std::cout << "                  Score Final: " << score << "\n";
    std::cout << "                  Tempo: " << std::fixed << std::setprecision(1) << time << "s\n\n";

    resetColor();
}