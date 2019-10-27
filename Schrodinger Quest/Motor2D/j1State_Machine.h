#ifndef __StateMachine_H__
#define __StateMachine_H__

#include "p2Defs.h"
#include "j1Player.h"
#include "j1Module.h"

//Check inputs
void CheckInputs(bool god_mode, bool& tang_mode, bool& jump_over, bool& spacebar_pushed, bool& canJump, bool& tangSwitchDeadCheck, int& inputsouts, int& speed_y, state actual, inputin& input_in, inputout input_out[MAX_INPUTS_OUT], player_colision_state collision_state, SDL_Rect& collider_rect);

//Execute states
Animation* ExecuteState(iPoint& speed, state actual, bool reset_animation, bool& climbing, bool& alive, bool& god_mode, bool& in_air, bool& stop_jumping_up);

//Check states
bool CheckState(int &inputsouts, state& actual, inputin& input_in, inputout input_out[MAX_INPUTS_OUT]);

#endif