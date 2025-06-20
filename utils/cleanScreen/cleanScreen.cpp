#include <windows.h>
#include "cleanScreen.h"

void cleanScreen() {
    // Esta é a implementação correta para limpar a tela no Windows.
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    COORD coordScreen = { 0, 0 };    // Posição inicial do cursor
    DWORD cCharsWritten;
    CONSOLE_SCREEN_BUFFER_INFO csbi;
    DWORD dwConSize;

    // Obter o número de células de caracteres no buffer atual.
    if (!GetConsoleScreenBufferInfo(hConsole, &csbi)) {
        return;
    }

    dwConSize = csbi.dwSize.X * csbi.dwSize.Y;

    // Preencher toda a tela com espaços em branco.
    if (!FillConsoleOutputCharacter(hConsole,        // Handle para o buffer da tela do console
                                    (TCHAR)' ',      // Caractere para escrever no buffer
                                    dwConSize,       // Número de células para escrever
                                    coordScreen,     // Coordenadas da primeira célula
                                    &cCharsWritten))// Recebe o número de caracteres escritos
    {
        return;
    }

    // Definir os atributos do buffer (cor do texto e do fundo).
    if (!FillConsoleOutputAttribute(hConsole,         // Handle para o buffer da tela do console
                                    csbi.wAttributes, // Atributos de caractere a serem usados
                                    dwConSize,        // Número de células para definir o atributo
                                    coordScreen,      // Coordenadas da primeira célula
                                    &cCharsWritten)) // Recebe o número de caracteres escritos
    {
        return;
    }

    // Colocar o cursor nas coordenadas iniciais.
    SetConsoleCursorPosition(hConsole, coordScreen);
}