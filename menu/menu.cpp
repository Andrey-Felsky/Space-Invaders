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

// Função auxiliar para posicionar o cursor
void setCursorPosition(int x, int y)
{
    COORD coord;
    coord.X = x;
    coord.Y = y;
    SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), coord);
}

// Função auxiliar para definir a cor do console
void setMenuColor(int color)
{
    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), color);
}

void printMenuTitle(int shinePosition)
{
    const std::vector<std::string> titleLines = {
        "        ___ ___  _   ___ ___            ",
        "       / __| _ \\/_\\ / __| __|             ",
        "       \\__ \\  _/ _ \\ (__| _|               ",
        "   ___ |___/_|/_/ \\_\\___|___|             ",
        "  |_ _|_ ___ ____ _ __| |___ _ _ ___    ",
        "   | || ' \\ V / _` / _` / -_) '_(_-<     ",
        "  |___|_||_\\_/\\__,_\\__,_\\___|_| /__/     "};

    const int shineWidth = 4;
    const int shineColor = 15; // Branco Brilhante

    std::cout << "\n"; // Adiciona o espaço que o R"()" original tinha
    for (size_t i = 0; i < titleLines.size(); ++i)
    {
        int baseColor = (i < 4) ? 11 : 12; // Ciano para "SPACE", Vermelho para "INVADERS"

        for (size_t j = 0; j < titleLines[i].length(); ++j)
        {
            // Se o caractere atual estiver na área do brilho, muda a cor
            if (j >= shinePosition && j < shinePosition + shineWidth)
            {
                setMenuColor(shineColor);
            }
            else
            {
                setMenuColor(baseColor);
            }
            std::cout << titleLines[i][j];
        }
        std::cout << std::endl;
    }
    std::cout << "\n";
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
    int prev_selected_option = -1; // Para detectar mudança na seleção
    char key;

    int animation_pos = -10;                                         // Posição inicial do brilho (fora da tela)
    const int title_width = 60;                                      // Largura aproximada do título para o reset da animação
    const int shine_width = 5;                                       // Largura do brilho
    auto last_anim_time = std::chrono::high_resolution_clock::now(); // Controle de tempo da animação
    const std::chrono::milliseconds anim_interval(40);               // Intervalo de atualização (~25 FPS)
    bool redrawTitle = true;
    bool redrawOptions = true;

    cleanScreen(); // Limpa a tela inteira apenas uma vez, no início.

    while (true)
    {
        // --- Lógica da Animação ---
        auto now = std::chrono::high_resolution_clock::now();
        if (now - last_anim_time >= anim_interval)
        {
            last_anim_time = now;
            animation_pos++;
            if (animation_pos > title_width)
            {
                animation_pos = -shine_width; // Reseta a animação para a esquerda
            }

            redrawTitle = true; // Marca o título para ser redesenhado no próximo quadro
        }

        // Se a seleção mudou, marca as opções para serem redesenhadas
        if (selected_option != prev_selected_option) {
            redrawOptions = true;
            prev_selected_option = selected_option;
        }

        // --- Lógica de Renderização (desenha apenas o que mudou) ---
        if (redrawTitle) {
            setCursorPosition(0, 0); // Move o cursor para o topo para redesenhar o título
            printMenuTitle(animation_pos);
            redrawTitle = false;
        }
        if (redrawOptions) {
            setCursorPosition(0, 10); // Posiciona o cursor abaixo do título (que tem 9 linhas)
            for (size_t i = 0; i < options.size(); ++i) {
                printMenuItem(options[i], i == selected_option);
            }
            resetColor();
            redrawOptions = false;
        }

        // --- Lógica de Input (Não-bloqueante) ---
        if (_kbhit())
        {
            key = _getch();
            if (key == 0 || key == -32)
            {                   // Teclas de seta
                key = _getch(); // Pega o segundo byte
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
                    return;
                case 1: // Dificuldade
                    selectDifficulty();
                    cleanScreen(); // Após sair de um submenu, limpa a tela
                    redrawTitle = true;   // e força o redesenho de tudo.
                    redrawOptions = true;
                    break;
                case 2: // Instrucoes
                    exibirInstrucoes();
                    cleanScreen();
                    redrawTitle = true;
                    redrawOptions = true;
                    break;
                case 3: // Ranking
                    showRanking();
                    std::cout << "\n\nPressione qualquer tecla para voltar ao menu...";
                    _getch();
                    cleanScreen();
                    redrawTitle = true;
                    redrawOptions = true;
                    break;
                case 4: // Sair
                    cleanScreen();
                    setMenuColor(7); // Define a cor para branco
                    const std::vector<std::string> gameOverLines = {
                        "::::::::::: :::    :::     :::     ::::    ::: :::    ::: ::::::::  ",
                        "    :+:     :+:    :+:   :+: :+:   :+:+:   :+: :+:   :+: :+:    :+: ",
                        "    +:+     +:+    +:+  +:+   +:+  :+:+:+  +:+ +:+  +:+  +:+        ",
                        "    +#+     +#++:++#++ +#++:++#++: +#+ +:+ +#+ +#++:++   +#++:++#++ ",
                        "    +#+     +#+    +#+ +#+     +#+ +#+  +#+#+# +#+  +#+         +#+ ",
                        "    #+#     #+#    #+# #+#     #+# #+#   #+#+# #+#   #+# #+#    #+# ",
                        "    ###     ###    ### ###     ### ###    #### ###    ### ########  "};

                    for (const auto& line : gameOverLines)
                    {
                        std::cout << line << std::endl;
                    }
                    std::cout << "\n\n";
                    exit(0);
                }
            }
        }
        else
        {
            // Evita o uso excessivo de CPU quando o programa está ocioso
            Sleep(1);
        }
    }
}