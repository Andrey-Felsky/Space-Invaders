#include "menu.h"
#include <iostream>
#include <windows.h>
#include <conio.h>
#include <vector>
#include <string>
#include "../ranking/score.h"
#include "../utils/cleanScreen/cleanScreen.h"

// Função auxiliar para definir a cor do console
void setMenuColor(int color)
{
    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), color);
}

void printMenuTitle()
{
    setMenuColor(11); // Ciano Claro
    std::cout << R"(
  ___ ___  _   ___ ___          
 / __| _ \/_\ / __| __|         
 \__ \  _/ _ \ (__| _|          
 |___/_|/_/ \_\___|___|)";
    setMenuColor(12); // Vermelho Claro
    std::cout << R"(
 |_ _|_ ___ ____ _ __| |___ _ _ 
  | || ' \ V / _` / _` / -_) '_|
 |___|_||_\_/\__,_\__,_\___|_|  
)";
    std::cout << "\n\n";
}

void printMenuItem(const std::string &text, bool selected)
{
    if (selected)
    {
        setMenuColor(14); // Amarelo
        std::cout << "                > " << text << " <\n";
    }
    else
    {
        setMenuColor(7); // Branco
        std::cout << "                  " << text << "  \n";
    }
}

void exibirInstrucoes()
{
    cleanScreen();
    setMenuColor(7);
    std::cout << "\n\n";
    std::cout << "   === INSTRUCOES ===\n\n";
    std::cout << "   - Use as teclas 'A' e 'D' para mover a nave.\n";
    std::cout << "   - Pressione 'ESPACO' para atirar.\n";
    std::cout << "   - Derrote todos os inimigos para vencer.\n";
    std::cout << "   - Evite ser atingido pelos inimigos ou deixar que eles cheguem a sua base.\n";
    std::cout << "   - Colete power-ups para obter vantagens temporarias.\n\n";
    std::cout << "   Pressione qualquer tecla para voltar ao menu...";
    _getch(); // Espera por qualquer tecla
}

void menu()
{
    hideCursor();
    std::vector<std::string> options = {"Jogar", "Instrucoes", "Ranking", "Sair"};
    int selected_option = 0;
    char key;

    while (true)
    {
        cleanScreen();
        printMenuTitle();

        for (size_t i = 0; i < options.size(); ++i)
        {
            printMenuItem(options[i], i == selected_option);
        }
        resetColor(); // Garante que a cor é resetada após a impressão do menu

        key = _getch();
        if (key == 0 || key == -32)
        {                   // Teclas de seta (primeiro byte 0 ou -32)
            key = _getch(); // Segundo byte da tecla de seta
            if (key == 72)
            { // Seta para cima
                selected_option = (selected_option - 1 + options.size()) % options.size();
            }
            else if (key == 80)
            { // Seta para baixo
                selected_option = (selected_option + 1) % options.size();
            }
        }
        else if (key == 13)
        { // Enter
            switch (selected_option)
            {
            case 0:     // Jogar
                return; // Retorna para main para iniciar o jogo
            case 1:     // Instrucoes
                exibirInstrucoes();
                break;
            case 2: // Ranking
                showRanking();
                std::cout << "\n\nPressione qualquer tecla para voltar ao menu...";
                _getch(); // Espera por qualquer tecla
                break;
            case 3: // Sair
                cleanScreen();
                std::cout << "Obrigado por jogar!\n";
                exit(0);
            }
        }
    }
}