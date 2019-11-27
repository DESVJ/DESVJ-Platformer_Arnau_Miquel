#ifndef __EnemyStateMachine_H__
#define __EnemyStateMachine_H__

#include "p2Defs.h"
#include "eEnemy.h"
//#include "j1Module.h"

//Check inputs
void CheckInputs(bool not_chase_tang_mode, int lives, float distance, float timer_idle, Enemy_State en_state, Enemy_State& en_state_update);

//Execute states
Animation* ExecuteState(p2Point<float>& speed, bool flip, bool& alive, float& timer_idle, Enemy_State& en_state, Animation* idle, Animation* move, Animation* death);

//Check states
bool CheckState(float& timer_idle, Enemy_State& en_state, Enemy_State& en_state_update);

#endif