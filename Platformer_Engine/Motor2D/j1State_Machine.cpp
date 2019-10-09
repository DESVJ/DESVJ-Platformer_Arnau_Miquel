#include "p2Defs.h"
#include "j1App.h"
#include "j1State_Machine.h"
#include "j1Input.h"
#include "j1Player.h"
#include "j1Render.h"
#include "j1Audio.h"
#include "SDL/include/SDL.h"

void Preupdate(int& jump_timer, int& punch_timer, int& kick_timer, int& specialattack_timer, int& hitted_timer, int& inputsouts, iPoint position, bool flip, state actual, inputin input_in, inputout input_out[3], j1Module* Player, Animation& current) {
	if (actual != S_HITTED) {
		bool left = false;
		bool right = false;
		if (App->input->GetKey(SDL_SCANCODE_D) == KEY_DOWN || App->input->GetKey(SDL_SCANCODE_D) == KEY_REPEAT) {
			if (flip == true) left = true;
			else right = true;
		}
		if (App->input->GetKey(SDL_SCANCODE_A) == KEY_DOWN || App->input->GetKey(SDL_SCANCODE_A) == KEY_REPEAT) {
			if (flip == true) right = true;
			else left = true;
		}
		if (right == true && left == true)input_in = I_RIGHT_AND_LEFT;
		else if (right == true)input_in = I_RIGHT;
		else if (left == true)input_in = I_LEFT;
		if (App->input->GetKey(SDL_SCANCODE_D) == KEY_UP) {
			if (flip == true)input_out[inputsouts] = O_LEFT;
			else input_out[inputsouts] = O_RIGHT;
			if (input_in == I_RIGHT && flip == false)input_in = I_NONE;
			else if (input_in == I_RIGHT && flip == true)input_in = I_NONE;
			inputsouts++;
		}
		else if (App->input->GetKey(SDL_SCANCODE_A) == KEY_UP) {
			if (flip == true)input_out[inputsouts] = O_RIGHT;
			else input_out[inputsouts] = O_LEFT;
			if (input_in == I_LEFT && flip == false)input_in = I_NONE;
			else if (input_in == I_RIGHT && flip == true)input_in = I_NONE;
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

		//CHECK TIMERS//
		if (jump_timer > 0) {
			if (position.y == 210) {
				input_out[inputsouts] = O_JUMP_FINISH;
				inputsouts++;
				jump_timer = 0;
			}
		}
		if (hitted_timer == 3) {
			input_out[inputsouts] = O_NONE;
			inputsouts++;
			hitted_timer = 0;
		}
	}
}