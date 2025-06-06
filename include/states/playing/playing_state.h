#pragma once

#include "BearLibTerminal.h"

#include "states/playing/input.h"
#include "states/playing/update.h"
#include "states/playing/render.h"

void playing_enter()
{
    terminal_print(0 , 0, "Playing Enter");
    return;
}

void playing_update()
{
    handle_input();
    update_frame();
    render_frame();
    return;
}

void playing_exit()
{
    return;
}