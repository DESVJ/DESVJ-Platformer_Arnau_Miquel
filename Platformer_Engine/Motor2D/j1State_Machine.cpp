//Code by Miquel Suau && Arnau Falagueras

#include "p2Defs.h"
#include "j1App.h"
#include "j1State_Machine.h"
#include "j1Input.h"
#include "j1Player.h"
//#include "j1Render.h"
//#include "j1Audio.h"
#include "SDL/include/SDL.h"

void CheckInputs(bool god_mode, bool& not_jumping, int& inputsouts, int& speed_y, state actual, inputin& input_in, inputout input_out[5], player_colision_state collision_state) {
	if (actual != S_DEAD && (collision_state != DYING || god_mode == true)) {
		input_in = I_NONE;
		//LEFT AND RIGHT MOVEMENT
		bool left = false;
		bool right = false;
		if (App->input->GetKey(SDL_SCANCODE_D) == KEY_DOWN || App->input->GetKey(SDL_SCANCODE_D) == KEY_REPEAT) right = true;
		if (App->input->GetKey(SDL_SCANCODE_A) == KEY_DOWN || App->input->GetKey(SDL_SCANCODE_A) == KEY_REPEAT) left = true;
		if (right == true && left == true)input_in = I_RIGHT_AND_LEFT;
		else if (right == true) input_in = I_RIGHT;
		else if (left == true)input_in = I_LEFT;
		if (App->input->GetKey(SDL_SCANCODE_D) == KEY_UP || App->input->GetKey(SDL_SCANCODE_D) == KEY_IDLE) {
			input_out[inputsouts] = O_RIGHT;
			inputsouts++;
		}
		else if (App->input->GetKey(SDL_SCANCODE_A) == KEY_UP || App->input->GetKey(SDL_SCANCODE_D) == KEY_IDLE) {
			input_out[inputsouts] = O_LEFT;
			inputsouts++;
		}
		//CHECK JUMP AND JUMP WITH DIRECTION//
		bool jump = false;
		if ((App->input->GetKey(SDL_SCANCODE_SPACE) == KEY_DOWN || App->input->GetKey(SDL_SCANCODE_SPACE) == KEY_REPEAT)&&(god_mode==false))jump = true;
		if (jump == true) {
			if (input_in == I_RIGHT)input_in = I_JUMP_RIGHT;
			else if (input_in == I_LEFT)input_in = I_JUMP_LEFT;
			else input_in = I_JUMP;
		}
		//CHECK LADDER MOVEMENT/IDLE//
		if (collision_state == CLIMBING && god_mode == false && (jump == false && left == false && right == false)) {
			bool up = false;
			bool down = false;
			if (App->input->GetKey(SDL_SCANCODE_W) == KEY_DOWN || App->input->GetKey(SDL_SCANCODE_W) == KEY_REPEAT) up = true;
			if (App->input->GetKey(SDL_SCANCODE_S) == KEY_DOWN || App->input->GetKey(SDL_SCANCODE_S) == KEY_REPEAT) down = true;
			if (down == true && up == false)input_in = I_LADDER_DOWN;
			else if (down == false && up == true)input_in = I_LADDER_UP;
			else if (actual == S_LADDER_DOWN || actual == S_LADDER_UP || actual == S_LADDER_IDLE) input_in = I_LADDER;
		}

		//CHECK FLYING IN GOD MODE//
		bool up = false;
		bool down = false;
		if ((App->input->GetKey(SDL_SCANCODE_W) == KEY_DOWN || App->input->GetKey(SDL_SCANCODE_W) == KEY_REPEAT) && (god_mode == true))up = true;
		if ((App->input->GetKey(SDL_SCANCODE_S) == KEY_DOWN || App->input->GetKey(SDL_SCANCODE_S) == KEY_REPEAT) && (god_mode == true))down = true;
		if (up == true && down == true) {
			if (input_in == I_RIGHT_AND_LEFT)input_in = I_DOWN_UP_RIGHT_LEFT;
			else if (input_in == I_RIGHT)input_in = I_DOWN_UP_RIGHT;
			else if (input_in == I_LEFT)input_in = I_DOWN_UP_LEFT;
			else input_in = I_DOWN_UP;
		}
		else if (up == true) {
			if (input_in == I_RIGHT_AND_LEFT)input_in = I_UP_RIGHT_LEFT;
			else if (input_in == I_RIGHT)input_in = I_UP_RIGHT;
			else if (input_in == I_LEFT)input_in = I_UP_LEFT;
			else input_in = I_UP;
		}
		else if (down == true) {
			if (input_in == I_RIGHT_AND_LEFT)input_in = I_DOWN_RIGHT_LEFT;
			else if (input_in == I_RIGHT)input_in = I_DOWN_RIGHT;
			else if (input_in == I_LEFT)input_in = I_DOWN_LEFT;
			else input_in = I_DOWN;
		}
		if (App->input->GetKey(SDL_SCANCODE_W) == KEY_UP || App->input->GetKey(SDL_SCANCODE_D) == KEY_IDLE) {
			input_out[inputsouts] = O_UP;
			inputsouts++;
		}
		else if (App->input->GetKey(SDL_SCANCODE_S) == KEY_UP || App->input->GetKey(SDL_SCANCODE_D) == KEY_IDLE) {
			input_out[inputsouts] = O_DOWN;
			inputsouts++;
		}


		//CHECK IF CHANGE TANGIBILITY//
		if (App->input->GetMouseButtonDown(SDL_BUTTON_LEFT) == KEY_DOWN)App->player->player.player_tang_mode = !App->player->player.player_tang_mode;

		///////////////////TEMPORAL!!!!!!!!!!!!!!!!!///////////////
		if (not_jumping == true) {
			input_out[inputsouts] = O_JUMP_FINISH;
			speed_y = 0;
			inputsouts++;
			not_jumping = false;
		}
	}
	else if (collision_state == DYING) {
		input_in = I_DEAD;
	}
}
Animation* ExecuteState(iPoint& speed, state actual, bool reset_animation) {
	Animation* current_animation;
	current_animation = &App->player->idle;
	bool right = false;
	bool left = false;
	bool up = false;
	bool down = false;
	bool jump = false;
	switch (actual) {
	case S_IDLE:
		current_animation = &App->player->idle;
		break;
	case S_RUN_LEFT:
		left = true;
	case S_RUN_RIGHT:
		if (left==false)right = true;
		current_animation = &App->player->run;
		break;
	case S_JUMP_LEFT:
		left = true;
	case S_JUMP_RIGHT:
		if (left == false)right = true;
	case S_JUMP:
		current_animation = &App->player->jump;
		jump = true;
		break;
	case S_UP_LEFT:
		left = true;
	case S_UP_RIGHT:
		if (left == false)right = true;
		current_animation = &App->player->run;
	case S_UP:
		up = true;
		break;
	case S_DOWN_LEFT:
		left = true;
	case S_DOWN_RIGHT:
		if (left == false)right = true;
		current_animation = &App->player->run;
	case S_DOWN:
		down = true;
		break;
	case S_LADDER_IDLE:
		App->player->player.player_climbing = true;
		current_animation = &App->player->idle_ladder;
		break;
	case S_LADDER_DOWN:
		down = true;
	case S_LADDER_UP:
		if (down == false)up = true;
		App->player->player.player_climbing = true;
		current_animation = &App->player->movement_ladder;
		break;
	case S_DEAD:
		App->player->player.player_alive = false;
		current_animation = &App->player->death;
		break;
	}
	/////////////////
	if (right == true) {
		if (speed.x < 2)speed.x++;
	}
	else if (left == true) {
		if (speed.x > -2)speed.x--;
	}
	else speed.x = 0;
	if (jump == true) {
		if (speed.y == 0 && (current_animation->current_frame == 0 || reset_animation == true))speed.y = -13;
	}
	else if (up == true) {
		if (speed.y > -2)speed.y--;
	}
	else if (down == true) {
		if (speed.y < 2)speed.y++;
	}
	else speed.y = 0;
	return current_animation;

}
bool CheckState(int &inputsouts, state& actual, inputin& input_in, inputout input_out[5]) {
	bool reset_animation = false;
	state before_check = actual;
	Animation* current_animation = &App->player->idle;
	switch (actual) {
	case S_IDLE:
		switch (input_in) {
		case I_DOWN_UP_LEFT:
		case I_LEFT: actual = S_RUN_LEFT; break;
		case I_DOWN_UP_RIGHT:
		case I_RIGHT: actual = S_RUN_RIGHT; break;
		case I_JUMP: actual = S_JUMP; break;
		case I_JUMP_RIGHT:actual = S_JUMP_RIGHT; break;
		case I_JUMP_LEFT:actual = S_JUMP_LEFT; break;
		case I_UP_RIGHT_LEFT:
		case I_UP:actual = S_UP; break;
		case I_UP_LEFT:actual = S_UP_LEFT; break;
		case I_UP_RIGHT:actual = S_UP_RIGHT; break;
		case I_DOWN_RIGHT_LEFT:
		case I_DOWN:actual = S_DOWN; break;
		case I_DOWN_LEFT:actual = S_DOWN_LEFT; break;
		case I_DOWN_RIGHT:actual = S_DOWN_RIGHT; break;
		case I_LADDER_DOWN:actual = S_LADDER_DOWN; break;
		case I_LADDER_UP:actual = S_LADDER_UP; break;
		case I_DEAD:actual = S_DEAD; break;
		}
		break;
	case S_RUN_RIGHT:
		switch (input_in) {
		case I_RIGHT_AND_LEFT:
		case I_DOWN_UP_LEFT:
		case I_DOWN_UP_RIGHT_LEFT:
		case I_DOWN_UP:
		case I_LEFT:actual = S_IDLE; break;
		case I_JUMP:
		case I_JUMP_RIGHT:actual = S_JUMP_RIGHT; break;
		case I_JUMP_LEFT:actual = S_JUMP_LEFT; break;
		case I_UP_RIGHT_LEFT:
		case I_UP_LEFT:
		case I_UP:actual = S_UP; break;
		case I_UP_RIGHT:actual = S_UP_RIGHT; break;
		case I_DOWN_RIGHT_LEFT:
		case I_DOWN_LEFT:
		case I_DOWN:actual = S_DOWN; break;
		case I_DOWN_RIGHT:actual = S_DOWN_RIGHT; break;
		case I_LADDER_DOWN:actual = S_LADDER_DOWN; break;
		case I_LADDER_UP:actual = S_LADDER_UP; break;
		case I_DEAD:actual = S_DEAD; break;
		}
		for (int i = 0; i <= inputsouts; i++)if (input_out[i] == O_RIGHT && actual == S_RUN_RIGHT)actual = S_IDLE;
		break;
	case S_RUN_LEFT:
		switch (input_in) {
		case I_RIGHT_AND_LEFT:
		case I_DOWN_UP_RIGHT:
		case I_DOWN_UP_RIGHT_LEFT:
		case I_DOWN_UP:
		case I_RIGHT:actual = S_IDLE; break;
		case I_JUMP:
		case I_JUMP_LEFT:actual = S_JUMP_LEFT; break;
		case I_JUMP_RIGHT:actual = S_JUMP_RIGHT; break;
		case I_UP_RIGHT_LEFT:
		case I_UP_RIGHT:
		case I_UP:actual = S_UP; break;
		case I_UP_LEFT:actual = S_UP_LEFT; break;
		case I_DOWN_RIGHT_LEFT:
		case I_DOWN_RIGHT:
		case I_DOWN:actual = S_DOWN; break;
		case I_DOWN_LEFT:actual = S_DOWN_LEFT; break;
		case I_LADDER_DOWN:actual = S_LADDER_DOWN; break;
		case I_LADDER_UP:actual = S_LADDER_UP; break;
		case I_DEAD:actual = S_DEAD; break;
		}
		for (int i = 0; i <= inputsouts; i++)if (input_out[i] == O_LEFT && actual == S_RUN_LEFT)actual = S_IDLE;
		break;
	case S_JUMP:
		switch (input_in) {
		case I_JUMP_RIGHT:
		case I_RIGHT: actual = S_JUMP_RIGHT; break;
		case I_JUMP_LEFT:
		case I_LEFT: actual = S_JUMP_LEFT; break;
		case I_LADDER_DOWN:actual = S_LADDER_DOWN; break;
		case I_LADDER_UP:actual = S_LADDER_UP; break;
		case I_DEAD:actual = S_DEAD; break;
		}
		for (int i = 0; i <= inputsouts; i++)if (input_out[i] == O_JUMP_FINISH)actual = S_IDLE;
		break;
	case S_JUMP_RIGHT:
		switch (input_in) {
		case I_JUMP_LEFT:
		case I_LEFT: actual = S_JUMP_LEFT; break;
		case I_JUMP: actual = S_JUMP; break;
		case I_LADDER_DOWN:actual = S_LADDER_DOWN; break;
		case I_LADDER_UP:actual = S_LADDER_UP; break;
		case I_DEAD:actual = S_DEAD; break;
		}
		for (int i = 0; i <= inputsouts; i++)if (input_out[i] == O_JUMP_FINISH)actual = S_IDLE;
		break;
	case S_JUMP_LEFT:
		switch (input_in) {
		case I_JUMP_RIGHT:
		case I_RIGHT: actual = S_JUMP_RIGHT; break;
		case I_JUMP: actual = S_JUMP; break;
		case I_LADDER_DOWN:actual = S_LADDER_DOWN; break;
		case I_LADDER_UP:actual = S_LADDER_UP; break;
		case I_DEAD:actual = S_DEAD; break;
		}
		for (int i = 0; i <= inputsouts; i++)if (input_out[i] == O_JUMP_FINISH)actual = S_IDLE;
		break;
	case S_UP:
		switch (input_in) {
		case I_DOWN:
		case I_DOWN_RIGHT_LEFT:
		case I_DOWN_UP:
		case I_DOWN_UP_RIGHT_LEFT:
		case I_RIGHT_AND_LEFT: actual = S_IDLE; break;
		case I_RIGHT:
		case I_DOWN_UP_RIGHT:
		case I_DOWN_RIGHT:actual = S_RUN_RIGHT; break;
		case I_LEFT:
		case I_DOWN_UP_LEFT:
		case I_DOWN_LEFT:actual = S_RUN_LEFT; break;
		case I_UP_LEFT:actual = S_UP_LEFT; break;
		case I_UP_RIGHT:actual = S_UP_RIGHT; break;
		case I_DEAD:actual = S_DEAD; break;
		}
		for (int i = 0; i <= inputsouts; i++)if (input_out[i] == O_UP && actual == S_UP)actual = S_IDLE;
		break;
	case S_UP_RIGHT:
		switch (input_in) {
		case I_DOWN_LEFT:
		case I_LEFT:
		case I_DOWN_UP_LEFT:
		case I_DOWN:
		case I_DOWN_RIGHT_LEFT:
		case I_DOWN_UP:
		case I_DOWN_UP_RIGHT_LEFT:
		case I_RIGHT_AND_LEFT: actual = S_IDLE; break;
		case I_RIGHT:
		case I_DOWN_UP_RIGHT:
		case I_DOWN_RIGHT:actual = S_RUN_RIGHT; break;
		case I_UP:
		case I_UP_RIGHT_LEFT:
		case I_UP_LEFT:actual = S_UP; break;
		case I_DEAD:actual = S_DEAD; break;
		}
		for (int i = 0; i <= inputsouts; i++) {
			switch (input_out[i]) {
			case O_UP:
				if (actual == S_UP_RIGHT)actual = S_RUN_RIGHT;
				if (actual == S_UP)actual = S_IDLE;
				break;
			case O_RIGHT:
				if (actual == S_UP_RIGHT)actual = S_UP;
				if (actual == S_RUN_RIGHT)actual = S_IDLE;
				break;
			}
		}
		break;
	case S_UP_LEFT:
		switch (input_in) {
		case I_DOWN_RIGHT:
		case I_RIGHT:
		case I_DOWN_UP_RIGHT:
		case I_DOWN:
		case I_DOWN_RIGHT_LEFT:
		case I_DOWN_UP:
		case I_DOWN_UP_RIGHT_LEFT:
		case I_RIGHT_AND_LEFT: actual = S_IDLE; break;
		case I_LEFT:
		case I_DOWN_UP_LEFT:
		case I_DOWN_LEFT:actual = S_RUN_LEFT; break;
		case I_UP:
		case I_UP_RIGHT_LEFT:
		case I_UP_RIGHT:actual = S_UP; break;
		case I_DEAD:actual = S_DEAD; break;
		}
		for (int i = 0; i <= inputsouts; i++) {
			switch (input_out[i]) {
			case O_UP:
				if (actual == S_UP_LEFT)actual = S_RUN_LEFT;
				if (actual == S_UP)actual = S_IDLE;
				break;
			case O_LEFT:
				if (actual == S_UP_LEFT)actual = S_UP;
				if (actual == S_RUN_LEFT)actual = S_IDLE;
				break;
			}
		}
		break;
	case S_DOWN:
		switch (input_in) {
		case I_UP:
		case I_UP_RIGHT_LEFT:
		case I_DOWN_UP:
		case I_DOWN_UP_RIGHT_LEFT:
		case I_RIGHT_AND_LEFT: actual = S_IDLE; break;
		case I_RIGHT:
		case I_DOWN_UP_RIGHT:
		case I_UP_RIGHT:actual = S_RUN_RIGHT; break;
		case I_LEFT:
		case I_DOWN_UP_LEFT:
		case I_UP_LEFT:actual = S_RUN_LEFT; break;
		case I_DOWN_LEFT:actual = S_DOWN_LEFT; break;
		case I_DOWN_RIGHT:actual = S_DOWN_RIGHT; break;
		case I_DEAD:actual = S_DEAD; break;
		}
		for (int i = 0; i <= inputsouts; i++)if (input_out[i] == O_DOWN && actual == S_DOWN)actual = S_IDLE;
		break;
	case S_DOWN_RIGHT:
		switch (input_in) {
		case I_UP_LEFT:
		case I_LEFT:
		case I_DOWN_UP_LEFT:
		case I_UP:
		case I_UP_RIGHT_LEFT:
		case I_DOWN_UP:
		case I_DOWN_UP_RIGHT_LEFT:
		case I_RIGHT_AND_LEFT: actual = S_IDLE; break;
		case I_RIGHT:
		case I_DOWN_UP_RIGHT:
		case I_UP_RIGHT:actual = S_RUN_RIGHT; break;
		case I_DOWN:
		case I_DOWN_RIGHT_LEFT:
		case I_DOWN_LEFT:actual = S_DOWN; break;
		case I_DEAD:actual = S_DEAD; break;
		}
		for (int i = 0; i <= inputsouts; i++) {
			switch (input_out[i]) {
			case O_DOWN:
				if (actual == S_DOWN_RIGHT)actual = S_RUN_RIGHT;
				if (actual == S_DOWN)actual = S_IDLE;
				break;
			case O_RIGHT:
				if (actual == S_DOWN_RIGHT)actual = S_DOWN;
				if (actual == S_RUN_RIGHT)actual = S_IDLE;
				break;
			}
		}
		break;
	case S_DOWN_LEFT:
		switch (input_in) {
		case I_UP_RIGHT:
		case I_RIGHT:
		case I_DOWN_UP_RIGHT:
		case I_UP:
		case I_UP_RIGHT_LEFT:
		case I_DOWN_UP:
		case I_DOWN_UP_RIGHT_LEFT:
		case I_RIGHT_AND_LEFT: actual = S_IDLE; break;
		case I_LEFT:
		case I_DOWN_UP_LEFT:
		case I_UP_LEFT:actual = S_RUN_LEFT; break;
		case I_DOWN:
		case I_DOWN_RIGHT_LEFT:
		case I_DOWN_RIGHT:actual = S_DOWN; break;
		case I_DEAD:actual = S_DEAD; break;
		}
		for (int i = 0; i <= inputsouts; i++) {
			switch (input_out[i]) {
			case O_DOWN:
				if (actual == S_DOWN_LEFT)actual = S_RUN_LEFT;
				if (actual == S_DOWN)actual = S_IDLE;
				break;
			case O_LEFT:
				if (actual == S_DOWN_LEFT)actual = S_DOWN;
				if (actual == S_RUN_LEFT)actual = S_IDLE;
				break;
			}
		}
		break;
	case S_LADDER_IDLE:
		if (App->player->player.col_state != CLIMBING || App->player->player.player_god_mode == true)actual = S_IDLE;
		else {
			switch (input_in) {
			case I_LADDER_DOWN:actual = S_LADDER_DOWN; break;
			case I_LADDER_UP:actual = S_LADDER_UP; break;
			case I_LEFT:
			case I_RIGHT:actual = S_IDLE; break;
				//TEMPORAL
		//	case I_JUMP:actual = S_JUMP; break;
		//	case I_JUMP_LEFT:actual = S_JUMP_LEFT; break;
		//	case I_JUMP_RIGHT:actual = S_JUMP_RIGHT; break;
			case I_DEAD:actual = S_DEAD; break;
			}
		}
		break;
	case S_LADDER_DOWN:
		if (App->player->player.col_state != CLIMBING || App->player->player.player_god_mode == true)actual = S_IDLE;
		else {
			switch (input_in) {
			case I_LADDER_UP:
			case I_LADDER:actual = S_LADDER_IDLE; break;
			case I_LEFT:
			case I_RIGHT:actual = S_IDLE; break;
				//TEMPORAL
		//	case I_JUMP:actual = S_JUMP; break;
		//	case I_JUMP_LEFT:actual = S_JUMP_LEFT; break;
		//	case I_JUMP_RIGHT:actual = S_JUMP_RIGHT; break;
			case I_DEAD:actual = S_DEAD; break;
			}
			for (int i = 0; i < inputsouts; i++)if (input_out[i] == O_DOWN)actual = S_LADDER_IDLE;
		}
		break;
	case S_LADDER_UP:
		if (App->player->player.col_state != CLIMBING || App->player->player.player_god_mode == true)actual = S_IDLE;
		else {
			switch (input_in) {
			case I_LADDER_DOWN:
			case I_LADDER:actual = S_LADDER_IDLE; break;
			case I_LEFT:
			case I_RIGHT:actual = S_IDLE; break;
				//TEMPORAL
		//	case I_JUMP:actual = S_JUMP; break;
		//	case I_JUMP_LEFT:actual = S_JUMP_LEFT; break;
		//	case I_JUMP_RIGHT:actual = S_JUMP_RIGHT; break;
			case I_DEAD:actual = S_DEAD; break;
			}
			for (int i = 0; i < inputsouts; i++)if (input_out[i] == O_UP)actual = S_LADDER_IDLE;
		}
	}
	if (before_check != actual) {
		switch (before_check)
		{
		case S_JUMP:
		case S_JUMP_LEFT:
		case S_JUMP_RIGHT: if (actual == S_IDLE) reset_animation = true; break;
		default: reset_animation = true; break;
		}
	}
	return reset_animation;
}
