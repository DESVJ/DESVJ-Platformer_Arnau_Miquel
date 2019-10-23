//Code by Miquel Suau && Arnau Falagueras

#ifndef __j1PLAYER_H__
#define __j1PLAYER_H__

#include "j1Module.h"
#include"p2Point.h"
#include"p2SString.h"
#include "Animation.h"
enum player_colision_state {

	NONE, CLIMBING, DYING

};

struct Player_Info
{

	SDL_Rect player_rect = { 0, 0, 0, 0 };
	SDL_Rect player_render_rect = { 0, 0, 20, 20 };
	p2Point<int> player_speed;
	bool player_flip;
	bool player_not_jumping;
	bool player_god_mode;
	bool player_tang_mode;
	bool player_alive;
	bool player_respawn;
	bool player_climbing;
	SDL_Texture* player_spritesheet;
	player_colision_state col_state;


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
	S_LADDER_DOWN,
	S_LADDER_UP,
	S_LADDER_IDLE,
	S_DEAD
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
	I_DOWN_UP_RIGHT_LEFT,
	I_LADDER,
	I_LADDER_UP,
	I_LADDER_DOWN,
	I_DEAD
};

enum inputout {
	O_NONE = 0,
	O_RIGHT,
	O_LEFT,
	O_UP,
	O_DOWN,
	O_JUMP_FINISH,
	O_DEAD_FINISH
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

	//Start level again
	void Start_F3();

	//Save
	bool Save(pugi::xml_node&) const;

	//Load
	bool Load(pugi::xml_node&);

	//Load animation pushbacks by name
	//void LoadAnimation(pugi::xml_node* animation_node, Animation* anim,const char* name);
	void LoadAnimationFromTMX(pugi::xml_node* animation_node, Animation* anim,const char* name);

	void Change_Col_State(player_colision_state state);

public:
	Player_Info player;
	Animation idle;
	Animation run;
	Animation jump;
	Animation idle_ladder;
	Animation movement_ladder;
	Animation death;

	int animation_created_mov = 0;

private:
	int gravity = 1;
	///////TEMPORAL
	int difference_y;
	int inputs_out;
	state actual_state;
	inputin input_in;
	inputout input_out[5];

	pugi::xml_document	player_info_file;
};

#endif // __j1PLAYER_H__