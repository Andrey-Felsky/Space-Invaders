#include "menu.h"
#include <iostream>
#include <windows.h>
#include <conio.h>
#include <chrono> // Necessário para chrono::high_resolution_clock
#include <thread> // Necessário para this_thread::sleep_for
#include <vector> // Necessário para vector
#include <string> // Necessário para string
#include "../ranking/score.h"
#include "../utils/cleanScreen/cleanScreen.h"
#include "../utils/constants.h" // Para a enum Difficulty
#include "../utils/console_utils.h" // Inclui as funções utilitárias de console
using namespace std;

void printMenuTitle(int shinePosition)
{
    const vector<string> titleLines = {
        "        ___ ___  _   ___ ___            ",
        "       / __| _ \\/_\\ / __| __|             ",
        "       \\__ \\  _/ _ \\ (__| _|               ",
        "   ___ |___/_|/_/ \\_\\___|___|             ",
        "  |_ _|_ ___ ____ _ __| |___ _ _ ___    ",
        "   | || ' \\ V / _` / _` / -_) '_(_-<     ",
        "  |___|_||_\\_/\\__,_\\__,_\\___|_| /__/     "};

    const int shineWidth = 4;
    const int shineColor = 15; // Branco Brilhante

    cout << "\n"; // Adiciona o espaço que o R"()" original tinha
    for (size_t i = 0; i < titleLines.size(); ++i)
    {
        int baseColor = (i < 4) ? 11 : 12; // Ciano para "SPACE", Vermelho para "INVADERS"

        for (size_t j = 0; j < titleLines[i].length(); ++j)
        {
            // Se o caractere atual estiver na área do brilho, muda a cor
            if (j >= shinePosition && j < shinePosition + shineWidth)
            {
                setConsoleColor(shineColor);
            }
            else
            {
                setConsoleColor(baseColor);
            }
            cout << titleLines[i][j];
        }
        cout << endl;
    }
    cout << "\n";
}

// Variável global de dificuldade definida em main.cpp
extern Difficulty currentDifficulty;
extern GameMode currentGameMode;


void exibirInstrucoes()
{
    cleanScreen();
    setConsoleColor(11); // Ciano claro para o título
    cout << "\n\n";
    cout << "   ==================== INSTRUCOES ====================\n\n";
    setConsoleColor(14); // Amarelo para subtítulos
    cout << "   OBJETIVO:\n";
    setConsoleColor(7); // Branco para texto normal
    cout << "   - Derrote todos os inimigos para vencer a partida.\n";
    cout << "   - Nao deixe os inimigos invadirem sua base (chegarem ao chao).\n";
    cout << "   - O jogo termina se ambos os jogadores perderem todas as vidas.\n\n";
    
    setConsoleColor(14);
    cout << "   CONTROLES - JOGADOR 1:\n";
    setConsoleColor(7);
    cout << "   - Mover: Teclas 'A' e 'D'   |   Atirar: Tecla 'ESPACO'\n\n";

    setConsoleColor(14);
    cout << "   CONTROLES - JOGADOR 2:\n";
    setConsoleColor(7);
    cout << "   - Mover: Setas Esquerda/Direita   |   Atirar: Seta para Cima\n\n";

    setConsoleColor(14);
    cout << "   POWER-UPS:\n";
    setConsoleColor(7);
    cout << "   - Mova sua nave para coletar os itens que caem dos inimigos!\n\n";

    setConsoleColor(11);
    cout << "   ====================================================\n\n";
    setConsoleColor(7);
    cout << "   Pressione qualquer tecla para voltar ao menu...";
    _getch(); // Espera por qualquer tecla
}

void selectDifficulty()
{
    const vector<pair<string, Difficulty>> difficultyOptions = {
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
        setConsoleColor(7);
        cout << "\n\n   === SELECIONE A DIFICULDADE ===\n\n";

        for (size_t i = 0; i < difficultyOptions.size(); ++i)
        {
            // Imprime o cursor de seleção (amarelo > <)
            if (i == selected_option)
            {
                setConsoleColor(14); // Yellow
                cout << "                > ";
            }
            else {
                cout << "                  ";
            }

            // Imprime o texto da opção, colorindo a que está ativa em verde
            if (difficultyOptions[i].second == currentDifficulty)
            {
                setConsoleColor(10); // Green
            }
            else {
                setConsoleColor(7); // White
            }
            cout << difficultyOptions[i].first;

            // Imprime o cursor de seleção
            if (i == selected_option)
            {
                setConsoleColor(14); // Yellow
                cout << " <\n";
            }
            else {
                cout << "  \n";
            }
        }

        setConsoleColor(7);
        cout << "\n\n   Use as setas para navegar e ENTER para confirmar.";
        cout << "\n   Pressione ESC para voltar ao menu.";

        key = _getch();
        if (key == 0 || key == -32)
        { // Arrow keys
            key = _getch();
            if (key == 72) { // Up
                Beep(600, 50); // Som de navegação
                selected_option = (selected_option == 0) ? difficultyOptions.size() - 1 : selected_option - 1;
            } else if (key == 80) { // Down
                Beep(600, 50); // Som de navegação
                selected_option = (selected_option + 1) % difficultyOptions.size();
            }
        }
        else if (key == 13)
        { // Enter
            Beep(900, 100); // Som de confirmação
            currentDifficulty = difficultyOptions[selected_option].second;
        }
        else if (key == 27)
        { // ESC
            Beep(400, 80); // Som de cancelamento/volta
            return; // Return to main menu
        }
    }
}

void menu()
{
    hideCursor(); // Usa a função de console_utils.h
    vector<string> options = {"Jogar (1P)", "Jogar (2P)", "Dificuldade", "Instrucoes", "Ranking", "Sair"};
    int selected_option = 0;
    int prev_selected_option = -1; // Para detectar mudança na seleção
    char key;

    int animation_pos = -10;                                         // Posição inicial do brilho (fora da tela)
    const int title_width = 60;                                      // Largura aproximada do título para o reset da animação
    const int shine_width = 5;                                       // Largura do brilho
    auto last_anim_time = chrono::high_resolution_clock::now(); // Controle de tempo da animação
    const chrono::milliseconds anim_interval(40);               // Intervalo de atualização (~25 FPS)
    bool redrawTitle = true;
    bool redrawOptions = true;

    cleanScreen(); // Limpa a tela inteira apenas uma vez, no início.

    while (true)
    {
        // --- Lógica da Animação ---
        auto now = chrono::high_resolution_clock::now();
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
            resetConsoleColor(); // Usa a função de console_utils.h
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
                    Beep(600, 50); // Som de navegação
                    selected_option = (selected_option - 1 + options.size()) % options.size();
                }
                else if (key == 80)
                { // Seta para baixo
                    Beep(600, 50); // Som de navegação
                    selected_option = (selected_option + 1) % options.size();
                }
            }
            else if (key == 13)
            { // Enter
                Beep(900, 100); // Som de confirmação
                switch (selected_option)
                {
                case 0: // Jogar (1P)
                    currentGameMode = GameMode::SINGLE_PLAYER;
                    resetConsoleColor();
                    return;
                case 1: // Jogar (2P)
                    currentGameMode = GameMode::TWO_PLAYER;
                    resetConsoleColor(); // Garante que a cor seja resetada ao sair do menu
                    return;
                case 2: // Dificuldade
                    resetConsoleColor(); // Garante que a cor seja resetada antes de entrar no submenu
                    selectDifficulty();
                    cleanScreen(); // Após sair de um submenu, limpa a tela
                    redrawTitle = true;   // e força o redesenho de tudo.
                    redrawOptions = true;
                    break;
                case 3: // Instrucoes
                    exibirInstrucoes();
                    resetConsoleColor(); // Garante que a cor seja resetada antes de entrar no submenu
                    cleanScreen();
                    redrawTitle = true;
                    redrawOptions = true;
                    break;
                case 4: // Ranking
                    showRanking();
                    cout << "\n\nPressione qualquer tecla para voltar ao menu...";
                    resetConsoleColor(); // Garante que a cor seja resetada antes de esperar input
                    _getch();
                    cleanScreen();
                    redrawTitle = true;
                    redrawOptions = true;
                    break;
                case 5: // Sair
                    Beep(300, 200); // Som de saída
                    cleanScreen();
                    setConsoleColor(7); // Define a cor para branco
                    const vector<string> gameOverLines = {
                        "::::::::::: :::    :::     :::     ::::    ::: :::    ::: ::::::::  ",
                        "    :+:     :+:    :+:   :+: :+:   :+:+:   :+: :+:   :+: :+:    :+: ",
                        "    +:+     +:+    +:+  +:+   +:+  :+:+:+  +:+ +:+  +:+  +:+        ",
                        "    +#+     +#++:++#++ +#++:++#++: +#+ +:+ +#+ +#++:++   +#++:++#++ ",
                        "    +#+     +#+    +#+ +#+     +#+ +#+  +#+#+# +#+  +#+         +#+ ",
                        "    #+#     #+#    #+# #+#     #+# #+#   #+#+# #+#   #+# #+#    #+# ",
                        "    ###     ###    ### ###     ### ###    #### ###    ### ########  "};

                    for (const auto& line : gameOverLines)
                    {
                        cout << line << endl;
                    }
                    cout << "\n\n";
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