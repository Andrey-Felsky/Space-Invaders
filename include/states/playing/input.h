#pragma once

#include "BearLibTerminal.h"
#include "game_elements.h"

void handle_input()
{
    key = terminal_read();
    return;
}