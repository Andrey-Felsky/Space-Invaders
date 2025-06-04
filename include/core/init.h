#pragma once

#include <BearLibTerminal.h>
#include "game_elements.h"

void init()
{
    terminal_open();
    std::string config = "window: title='Ispace Invaders', size= " + std::to_string(w_width) + "x" + std::to_string(w_height) + ", fullscreen=true";
    terminal_set(config.c_str());
    terminal_set("font: Media/HackNerdFontMono-Regular.ttf, size = 11");
    terminal_clear();
    return;
}
