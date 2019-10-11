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
	S_HITTED
};

enum inputin {
	I_NONE = 0,
	I_RIGHT,
	I_LEFT,
	I_RIGHT_AND_LEFT,
	I_JUMP_RIGHT,
	I_JUMP_LEFT,
	I_JUMP
};

enum inputout {
	O_NONE = 0,
	O_RIGHT,
	O_LEFT,
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

private:
	int gravity = 1;
	///////TEMPORAL
	int difference_y;
	int inputs_out;
	state actual_state;
	inputin input_in;
	inputout input_out[3];
};

#endif // __j1PLAYER_H__