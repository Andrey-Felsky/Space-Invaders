#include <conio.h>
#include "state_machine.h"
#include "game_elements.h"

void menu_enter()
{
    cout << "Menu Enter";
    return;
}

void menu_update()
{
    nextState = STATE_NONE;
    stateChanged = true;
    getch();
    return;
}

void menu_exit()
{
    cout << "Menu Exit";
    gameRunning = false;
    return;
}