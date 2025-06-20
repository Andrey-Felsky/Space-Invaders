#ifndef CONSOLE_UTILS_H
#define CONSOLE_UTILS_H

#include <iostream>
#include <string>
#include <windows.h> // For COORD, SetConsoleCursorPosition, SetConsoleTextAttribute

// Function to set cursor position
inline void setCursorPosition(int x, int y)
{
    COORD coord;
    coord.X = x;
    coord.Y = y;
    SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), coord);
}

// Function to reset cursor position to (0,0)
inline void resetCursorPosition()
{
    COORD coord = {0, 0};
    SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), coord);
}

// Function to set console text color
inline void setConsoleColor(int color)
{
    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), color);
}

// Function to reset console text color to default (white on black)
inline void resetConsoleColor()
{
    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 7); // Default white on black
}

// Function to hide the console cursor
inline void hideCursor()
{
    HANDLE consoleHandle = GetStdHandle(STD_OUTPUT_HANDLE);
    CONSOLE_CURSOR_INFO cci;
    GetConsoleCursorInfo(consoleHandle, &cci);
    cci.bVisible = FALSE; // set the cursor visibility
    SetConsoleCursorInfo(consoleHandle, &cci);
}

// Function to print a menu item with selection highlighting (defined here as it's a general utility)
inline void printMenuItem(const std::string &text, bool selected)
{
    if (selected)
    {
        setConsoleColor(14); // Amarelo
        std::cout << "                > " << text << " <\n";
    }
    else
    {
        setConsoleColor(7); // Branco
        std::cout << "                  " << text << "  \n";
    }
}

#endif // CONSOLE_UTILS_H