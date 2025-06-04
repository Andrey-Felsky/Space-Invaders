#include "BearLibTerminal.h"

#include <conio.h>
#include "state_machine.h"
#include "game_elements.h"

void menu_enter()
{
    terminal_print(0 , 0, "Menu Enter");
    terminal_refresh();
    return;
}

void menu_update()
{
    nextState = STATE_PLAYING;
    stateChanged = true;
    key = terminal_read();

    return;
}

void menu_exit()
{
    terminal_print(0 , 0, "Menu Exit");
    return;
}