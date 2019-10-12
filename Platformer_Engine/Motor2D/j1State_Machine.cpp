//Code by Miquel Suau && Arnau Falagueras

#include "p2Defs.h"
#include "j1App.h"
#include "j1State_Machine.h"
#include "j1Input.h"
#include "j1Player.h"
//#include "j1Render.h"
//#include "j1Audio.h"
#include "SDL/include/SDL.h"

void CheckInputs(int& inputsouts, int& speed_y, state actual, inputin& input_in, inputout input_out[3]) {
	if (actual != S_HITTED) {
		//LEFT AND RIGHT MOVEMENT
		bool left = false;
		bool right = false;
		if (App->input->GetKey(SDL_SCANCODE_D) == KEY_DOWN || App->input->GetKey(SDL_SCANCODE_D) == KEY_REPEAT) right = true;
		if (App->input->GetKey(SDL_SCANCODE_A) == KEY_DOWN || App->input->GetKey(SDL_SCANCODE_A) == KEY_REPEAT) left = true;
		if (right == true && left == true)input_in = I_RIGHT_AND_LEFT;
		else if (right == true) input_in = I_RIGHT;
		else if (left == true)input_in = I_LEFT;
		if (App->input->GetKey(SDL_SCANCODE_D) == KEY_UP) {
			input_out[inputsouts] = O_RIGHT;
			if (input_in == I_RIGHT)input_in = I_NONE;
			inputsouts++;
		}
		else if (App->input->GetKey(SDL_SCANCODE_A) == KEY_UP) {
			input_out[inputsouts] = O_LEFT;
			if (input_in == I_LEFT)input_in = I_NONE;
			inputsouts++;
		}
		//CHECK JUMP AND JUMP WITH DIRECTION//
		bool jump = false;
		if (App->input->GetKey(SDL_SCANCODE_W) == KEY_DOWN || App->input->GetKey(SDL_SCANCODE_W) == KEY_REPEAT)jump = true;
		if (jump == true) {
			if (input_in == I_RIGHT)input_in = I_JUMP_RIGHT;
			else if (input_in == I_LEFT)input_in = I_JUMP_LEFT;
			else input_in = I_JUMP;
		}
		///////////////////TEMPORAL!!!!!!!!!!!!!!!!!///////////////
		if (speed_y >= 12) {
			input_out[inputsouts] = O_JUMP_FINISH;
			speed_y = 0;
			inputsouts++;
		}
	}
}
Animation* ExecuteState(iPoint& speed, state actual, bool reset_animation) {
	Animation* current_animation;
	current_animation = &App->player->idle;
	bool right = false;
	bool left = false;
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
	}
	/////////////////
	if (right == true) {
		if (speed.x < 2)speed.x++;
	}
	else if (left == true) {
		if (speed.x > -2)speed.x--;
	}
	else speed.x = 0;
	/*else if (speed.x != 0) {
		if (speed.x > 0)speed.x--;
		else speed.x++;
	}*/
	if (jump == true) {
		if (speed.y == 0 && (current_animation->current_frame == 0 || reset_animation == true))speed.y = -12;
	}
	return current_animation;

}
bool CheckState(int &inputsouts, state& actual, inputin& input_in, inputout input_out[3]) {
	bool reset_animation = false;
	state before_check = actual;
	Animation* current_animation = &App->player->idle;
	switch (actual) {
	case S_IDLE:
		switch (input_in) {
		case I_LEFT: actual = S_RUN_LEFT; break;
		case I_RIGHT: actual = S_RUN_RIGHT; break;
		case I_JUMP: actual = S_JUMP; break;
		case I_JUMP_RIGHT:actual = S_JUMP_RIGHT; break;
		case I_JUMP_LEFT:actual = S_JUMP_LEFT; break;
		}
		break;
	case S_RUN_RIGHT:
		switch (input_in) {
		case I_RIGHT_AND_LEFT:
		case I_LEFT:actual = S_IDLE; break;
		case I_JUMP:
		case I_JUMP_RIGHT:actual = S_JUMP_RIGHT; break;
		case I_JUMP_LEFT:actual = S_JUMP_LEFT; break;
		}
		for (int i = 0; i <= inputsouts; i++)if (input_out[i] == O_RIGHT)actual = S_IDLE;
		break;
	case S_RUN_LEFT:
		switch (input_in) {
		case I_RIGHT_AND_LEFT:
		case I_RIGHT:actual = S_IDLE; break;
		case I_JUMP:
		case I_JUMP_LEFT:actual = S_JUMP_LEFT; break;
		case I_JUMP_RIGHT:actual = S_JUMP_RIGHT; break;
		}
		for (int i = 0; i <= inputsouts; i++)if (input_out[i] == O_LEFT)actual = S_IDLE;
		break;
	case S_JUMP:
		switch (input_in) {
		case I_JUMP_RIGHT:
		case I_RIGHT: actual = S_JUMP_RIGHT; break;
		case I_JUMP_LEFT:
		case I_LEFT: actual = S_JUMP_LEFT; break;
		}
		for (int i = 0; i <= inputsouts; i++)if (input_out[i] == O_JUMP_FINISH)actual = S_IDLE;
		break;
	case S_JUMP_RIGHT:
		switch (input_in) {
		case I_JUMP_LEFT:
		case I_LEFT: actual = S_JUMP_LEFT; break;
		case I_JUMP: actual = S_JUMP; break;
		}
		for (int i = 0; i <= inputsouts; i++)if (input_out[i] == O_JUMP_FINISH)actual = S_IDLE;
		break;
	case S_JUMP_LEFT:
		switch (input_in) {
		case I_JUMP_RIGHT:
		case I_RIGHT: actual = S_JUMP_RIGHT; break;
		case I_JUMP: actual = S_JUMP; break;
		}
		for (int i = 0; i <= inputsouts; i++)if (input_out[i] == O_JUMP_FINISH)actual = S_IDLE;
		break;
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
