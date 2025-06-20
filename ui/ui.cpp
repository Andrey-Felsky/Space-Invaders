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
 _     _  ___   __    _  __    _  _______  ______   
| | _ | ||   | |  |  | ||  |  | ||       ||    _ |  
| || || ||   | |   |_| ||   |_| ||    ___||   | ||  
|       ||   | |       ||       ||   |___ |   |_||_ 
|       ||   | |  _    ||  _    ||    ___||    __  |
|   _   ||   | | | |   || | |   ||   |___ |   |  | |
|__| |__||___| |_|  |__||_|  |__||_______||___|  |_|

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
 ______   _______  _______  _______  _______  _______ 
|      | |       ||       ||       ||   _   ||       |
|  _    ||    ___||    ___||    ___||  |_|  ||_     _|
| | |   ||   |___ |   |___ |   |___ |       |  |   |  
| |_|   ||    ___||    ___||    ___||       |  |   |  
|       ||   |___ |   |    |   |___ |   _   |  |   |  
|______| |_______||___|    |_______||__| |__|  |___|  

)";
    setColor(14); // Amarelo
    std::cout << "                A frota inimiga dominou...\n\n";

    setColor(7); // Branco
    std::cout << "                  Score Final: " << score << "\n";
    std::cout << "                  Tempo: " << std::fixed << std::setprecision(1) << time << "s\n\n";

    resetColor();
}