#pragma once

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
    terminal_print(0 , 1, "Menu Update");
    nextState = STATE_PLAYING;
    key = terminal_read();
    stateChanged = true;
    terminal_refresh();
    return;
}

void menu_exit()
{
    terminal_print(0 , 3, "Menu Exit");
    terminal_refresh();
    return;
}