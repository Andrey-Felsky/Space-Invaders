#include "ui.h"
#include <iostream>
#include <windows.h>
#include "../utils/console_utils.h"
#include <iomanip>
#include "../utils/cleanScreen/cleanScreen.h"

void showVictoryScreen(int score, float time)
{
    cleanScreen();
    setConsoleColor(10); // Verde Claro
    std::cout << R"(
 _     _  ___   __    _  __    _  _______  ______   
| | _ | ||   | |  |  | ||  |  | ||       ||    _ |  
| || || ||   | |   |_| ||   |_| ||    ___||   | ||  
|       ||   | |       ||       ||   |___ |   |_||_ 
|       ||   | |  _    ||  _    ||    ___||    __  |
|   _   ||   | | | |   || | |   ||   |___ |   |  | |
|__| |__||___| |_|  |__||_|  |__||_______||___|  |_|

)";
    setConsoleColor(14); // Amarelo
    std::cout << "            Voce defendeu a galaxia com sucesso!\n\n";

    setConsoleColor(7); // Branco
    std::cout << "                  Score Final: " << score << "\n";
    std::cout << "                  Tempo: " << std::fixed << std::setprecision(1) << time << "s\n\n";

    resetConsoleColor();
}

void showDefeatScreen(int score, float time)
{
    cleanScreen();
    setConsoleColor(12); // Vermelho Claro
    std::cout << R"(
 ______   _______  _______  _______  _______  _______ 
|      | |       ||       ||       ||   _   ||       |
|  _    ||    ___||    ___||    ___||  |_|  ||_     _|
| | |   ||   |___ |   |___ |   |___ |       |  |   |  
| |_|   ||    ___||    ___||    ___||       |  |   |  
|       ||   |___ |   |    |   |___ |   _   |  |   |  
|______| |_______||___|    |_______||__| |__|  |___|  

)";
    setConsoleColor(14); // Amarelo
    std::cout << "                A frota inimiga dominou...\n\n";

    setConsoleColor(7); // Branco
    std::cout << "                  Score Final: " << score << "\n";
    std::cout << "                  Tempo: " << std::fixed << std::setprecision(1) << time << "s\n\n";

    resetConsoleColor();
}