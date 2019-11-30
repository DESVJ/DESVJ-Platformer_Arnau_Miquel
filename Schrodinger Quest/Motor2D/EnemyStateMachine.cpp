#include "p2Defs.h"
//#include "j1App.h"
#include "j1State_Machine.h"
#include "eEnemy.h"
//#include "j1Input.h"
//#include "j1Player.h"
//#include "j1Audio.h"
#include "EntityManager.h"
#include "j1Timer.h"
//#include "SDL/include/SDL.h"

void CheckInputs(bool not_chase_tang_mode, int lives, bool distance, j1Timer timer_idle, Enemy_State en_state, Enemy_State& en_state_update) {
	//Check if the enemy is dead
	if (lives > 0)
	{
		//Check if the enemy is "seeing" the player
		if (distance == true && not_chase_tang_mode == false)
		{
			en_state_update = Enemy_State::chase;
		}
		else if (en_state == Enemy_State::chase || (en_state == Enemy_State::idle && timer_idle.ReadSec() < 2.0f))
		{
			en_state_update = Enemy_State::idle;
		}
		else
		{
			en_state_update = Enemy_State::move;
		}
	}
	else en_state_update = Enemy_State::dead;

}


Animation* ExecuteState(p2Point<float>& speed, bool flip, bool& alive, j1Timer& timer_idle, Enemy_State& en_state, Animation* idle, Animation* move, Animation* death) {
	
	bool left = false;
	bool right = false;

	Animation* current_animation;
	current_animation = idle;
	switch (en_state)
	{
	case Enemy_State::idle:
		current_animation = idle;
		break;

	case Enemy_State::move:
		current_animation = move;
		if (flip == false)
		{
			right = true;
		}
		else
		{
			left = true;
		}
		break;

	case Enemy_State::chase:
		current_animation = move;
		if (flip == false)
		{
			right = true;
		}
		else
		{
			left = true;
		}
		break;

	case Enemy_State::dead:
		current_animation = death;
		alive = false;
		break;
	}


	if (right == true)
	{
		if (speed.x < 2)
		{
			speed.x += 0.2f;
		}
	}
	else if (left == true)
	{
		if (speed.x > -2)
		{
			speed.x -= 0.2f;
		}
	}
	else
	{
		if (speed.x > 0)
		{
			speed.x -= 0.5f;
			if (speed.x <= 0)
				speed.x = 0;
		}
		else
		{
			speed.x += 0.5f;
			if (speed.x >= 0)
				speed.x = 0;
		}
	}


	return current_animation;
}

bool CheckState(j1Timer& timer_idle, Enemy_State& en_state, Enemy_State& en_state_update){
	bool reset_animation = false;
	Enemy_State before_check = en_state;

	switch (en_state) 
	{
	case Enemy_State::idle:
		switch (en_state_update)
		{
		case Enemy_State::chase:
			en_state = Enemy_State::chase;
			break;

		case Enemy_State::move:
			en_state = Enemy_State::move;
			break;

		case Enemy_State::dead:
			en_state = Enemy_State::dead;
			break;
		}
		break;

	case Enemy_State::move:
		switch (en_state_update)
		{
		case Enemy_State::chase:
			en_state = Enemy_State::chase;
			break;

		case Enemy_State::dead:
			en_state = Enemy_State::dead;
			break;
		}
		break;

	case Enemy_State::chase:
		switch (en_state_update)
		{
		case Enemy_State::idle:
			en_state = Enemy_State::idle;
			timer_idle.Start();
			break;

		case Enemy_State::dead:
			en_state = Enemy_State::dead;
			break;
		}
		break;
	}
	
	
	
	if (before_check != en_state)
	{
		reset_animation = true;
	}

	return reset_animation;
}
