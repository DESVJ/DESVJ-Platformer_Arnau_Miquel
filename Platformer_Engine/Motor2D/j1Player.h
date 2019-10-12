//Code by Miquel Suau && Arnau Falagueras

#ifndef __j1PLAYER_H__
#define __j1PLAYER_H__

#include "j1Module.h"
#include"p2Point.h"
#include "Animation.h"

struct Player_Info
{

	SDL_Rect player_rect = { 0, 0, 0, 0 };
	p2Point<int> player_speed;
	bool player_flip;
	bool player_not_jumping;
	bool player_god_mode;
	SDL_Texture* player_spritesheet;


};

enum state {
	S_NONE = 0,
	S_IDLE,
	S_RUN_RIGHT,
	S_RUN_LEFT,
	S_JUMP,
	S_JUMP_RIGHT,
	S_JUMP_LEFT,
	S_UP,
	S_UP_RIGHT,
	S_UP_LEFT,
	S_DOWN,
	S_DOWN_RIGHT,
	S_DOWN_LEFT,
	S_HITTED
};

enum inputin {
	I_NONE = 0,
	I_RIGHT,
	I_LEFT,
	I_RIGHT_AND_LEFT,
	I_JUMP_RIGHT,
	I_JUMP_LEFT,
	I_JUMP,
	I_UP,
	I_UP_RIGHT,
	I_UP_LEFT,
	I_UP_RIGHT_LEFT,
	I_DOWN,
	I_DOWN_RIGHT,
	I_DOWN_LEFT,
	I_DOWN_RIGHT_LEFT,
	I_DOWN_UP,
	I_DOWN_UP_RIGHT,
	I_DOWN_UP_LEFT,
	I_DOWN_UP_RIGHT_LEFT
};

enum inputout {
	O_NONE = 0,
	O_RIGHT,
	O_LEFT,
	O_UP,
	O_DOWN,
	O_JUMP_FINISH
};

class j1Player : public j1Module
{

public:

	j1Player();

	// Destructor
	virtual ~j1Player();

	// Called before render is available
	bool Awake(pugi::xml_node&);

	// Called before the first frame
	bool Start();

	// Called each loop iteration
	bool PreUpdate();
	bool Update(float dt);

	// Called before quitting
	bool CleanUp();

public:
	Player_Info player;
	Animation idle;
	Animation run;
	Animation jump;
	Animation idle_ladder;
	Animation movement_ladder;
	Animation death;

private:
	int gravity = 1;
	///////TEMPORAL
	int difference_y;
	int inputs_out;
	state actual_state;
	inputin input_in;
	inputout input_out[5];
};

#endif // __j1PLAYER_H__