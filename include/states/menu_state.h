#include "BearLibTerminal.h"

#include <conio.h>
#include "state_machine.h"
#include "game_elements.h"

void menu_enter()
{
    terminal_print(0 , 0, "Hellow");
    terminal_refresh();
    return;
}

void menu_update()
{
    nextState = STATE_NONE;
    stateChanged = true;
    key = terminal_read();
    
    return;
}

void menu_exit()
{
    cout << "Menu Exit";
    gameRunning = false;
    return;
}