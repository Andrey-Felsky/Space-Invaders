#pragma once


#include "states/playing/input.h"
#include "states/playing/update.h"
#include "states/playing/render.h"

void playing_enter()
{
    
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