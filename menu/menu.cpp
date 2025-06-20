#include "menu.h"
#include <iostream>
#include <windows.h>
#include <conio.h>
#include <vector>
#include <string>
#include "../ranking/score.h"
#include "../utils/cleanScreen/cleanScreen.h"
#include "../utils/constants.h" // Para a enum Difficulty

// Variável global de dificuldade definida em main.cpp
extern Difficulty currentDifficulty;

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
       ___ |___/_|/_/ \_\___|___|)";
    setMenuColor(12); // Vermelho Claro
    std::cout << R"(    
      |_ _|_ ___ ____ _ __| |___ _ _ ___
       | || ' \ V / _` / _` / -_) '_(_-<
      |___|_||_\_/\__,_\__,_\___|_| /__/)";
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

void selectDifficulty()
{
    const std::vector<std::pair<std::string, Difficulty>> difficultyOptions = {
        {"Facil", Difficulty::FACIL},
        {"Normal", Difficulty::NORMAL},
        {"Dificil", Difficulty::DIFICIL},
        {"AUTO", Difficulty::AUTO}};

    int selected_option = 0;
    // Encontra o índice inicial com base na dificuldade global atual
    for (size_t i = 0; i < difficultyOptions.size(); ++i)
    {
        if (difficultyOptions[i].second == currentDifficulty)
        {
            selected_option = i;
            break;
        }
    }

    char key;

    while (true)
    {
        cleanScreen();
        setMenuColor(7);
        std::cout << "\n\n   === SELECIONE A DIFICULDADE ===\n\n";

        for (size_t i = 0; i < difficultyOptions.size(); ++i)
        {
            // Imprime o cursor de seleção (amarelo > <)
            if (i == selected_option)
            {
                setMenuColor(14); // Yellow
                std::cout << "                > ";
            }
            else {
                std::cout << "                  ";
            }

            // Imprime o texto da opção, colorindo a que está ativa em verde
            if (difficultyOptions[i].second == currentDifficulty)
            {
                setMenuColor(10); // Green
            }
            else {
                setMenuColor(7); // White
            }
            std::cout << difficultyOptions[i].first;

            // Imprime o cursor de seleção
            if (i == selected_option)
            {
                setMenuColor(14); // Yellow
                std::cout << " <\n";
            }
            else {
                std::cout << "  \n";
            }
        }

        setMenuColor(7);
        std::cout << "\n\n   Use as setas para navegar e ENTER para confirmar.";
        std::cout << "\n   Pressione ESC para voltar ao menu.";

        key = _getch();
        if (key == 0 || key == -32)
        { // Arrow keys
            key = _getch();
            if (key == 72) { // Up
                selected_option = (selected_option == 0) ? difficultyOptions.size() - 1 : selected_option - 1;
            } else if (key == 80) { // Down
                selected_option = (selected_option + 1) % difficultyOptions.size();
            }
        }
        else if (key == 13)
        { // Enter
            currentDifficulty = difficultyOptions[selected_option].second;
        }
        else if (key == 27)
        { // ESC
            return; // Return to main menu
        }
    }
}

void menu()
{
    hideCursor();
    std::vector<std::string> options = {"Jogar", "Dificuldade", "Instrucoes", "Ranking", "Sair"};
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
            case 0: // Jogar
                return; // Retorna para main para iniciar o jogo
            case 1: // Dificuldade
                selectDifficulty();
                break;
            case 2: // Instrucoes
                exibirInstrucoes();
                break;
            case 3: // Ranking
                showRanking();
                std::cout << "\n\nPressione qualquer tecla para voltar ao menu...";
                _getch(); // Espera por qualquer tecla
                break;
            case 4: // Sair
                cleanScreen();
                std::cout << R"(
::::::::::: :::    :::     :::     ::::    ::: :::    ::: ::::::::  
    :+:     :+:    :+:   :+: :+:   :+:+:   :+: :+:   :+: :+:    :+: 
    +:+     +:+    +:+  +:+   +:+  :+:+:+  +:+ +:+  +:+  +:+        
    +#+     +#++:++#++ +#++:++#++: +#+ +:+ +#+ +#++:++   +#++:++#++ 
    +#+     +#+    +#+ +#+     +#+ +#+  +#+#+# +#+  +#+         +#+ 
    #+#     #+#    #+# #+#     #+# #+#   #+#+# #+#   #+# #+#    #+# 
    ###     ###    ### ###     ### ###    #### ###    ### ########
    
    
    )";
                exit(0);
            }
        }
    }
}