#ifndef __EnemyStateMachine_H__
#define __EnemyStateMachine_H__

#include "p2Defs.h"
#include "eEnemy.h"
//#include "j1Module.h"
class j1Timer;

//Check inputs
void CheckInputs(bool not_chase_tang_mode, bool lives, bool distance, j1Timer timer_idle, Enemy_State en_state, Enemy_State& en_state_update);

//Execute states
Animation* ExecuteState(p2Point<float>& speed, bool flip, bool& alive, j1Timer& timer_idle, Enemy_State& en_state, Animation* idle, Animation* move, Animation* death);

//Check states
bool CheckState(j1Timer& timer_idle, Enemy_State& en_state, Enemy_State& en_state_update);

#endif