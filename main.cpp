// libs
#include <BearLibTerminal.h>
// person libs
#include "core/init.h"
#include "game_elements.h"
#include "state_machine.h"
// states
#include "./include/states/menu/menu_state.h"
#include "./include/states/playing/playing_state.h"
#include "./include/states/game_over/game_over_state.h"

bool stateChanged = false;

StateMachine currentState = STATE_NONE;
StateMachine nextState = STATE_MENU;
StateMachine previousState = currentState;

void switchState()
{
    if (!stateChanged && currentState == nextState)
    {
        return;
    }

    // exit state
    switch (currentState)
    {
    case STATE_NONE:
        break;
    case STATE_MENU:
        menu_exit();
        break;
    case STATE_PLAYING:
        playing_exit();
        break;
    case STATE_GAME_OVER:
        game_over_exit();
        break;
    default:
        break;
    }

    previousState = currentState;
    currentState = nextState;

    // enter state
    switch (currentState)
    {
    case STATE_NONE:
        break;
    case STATE_MENU:
        menu_enter();
        break;
    case STATE_PLAYING:
        playing_enter();
        break;
    case STATE_GAME_OVER:
        game_over_enter();
        break;
    default:
        break;
    }

    stateChanged = false;

    return;
}

void loopGame()
{
    while (gameRunning)
    {
        if(key == TK_CLOSE || key == TK_ESCAPE){
            gameRunning = false;
            return;
        }

        switchState();

        switch (currentState)
        {
        case STATE_NONE:
            break;
        case STATE_MENU:
            menu_update();
            break;
        case STATE_PLAYING:
            playing_update();
            break;
        case STATE_GAME_OVER:
            game_over_update();
            break;
        default:
            break;
        }
    }
    return;
}

int main()
{
    init();
    loopGame();
    terminal_close();
    return 0;
}