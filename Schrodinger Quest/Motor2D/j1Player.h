//Code by Miquel Suau && Arnau Falagueras

#ifndef __j1PLAYER_H__
#define __j1PLAYER_H__

#include "j1Module.h"
#include"p2Point.h"
#include"p2SString.h"
#include "Animation.h"
#include"p2List.h"
#define MAX_INPUTS_OUT 5

//Collision state enum
enum player_colision_state 
{
	NONE, 
	CLIMBING, 
	DYING
};

//Player struct
struct Player_Info
{

	int minPlayerHeight;
	SDL_Rect player_rect;
	SDL_Rect player_collider_rect;
	p2Point<float> player_speed;
	bool player_flip;
	bool player_not_jumping;
	bool player_stop_jumping_up;
	bool player_god_mode;
	bool player_tang_mode;
	bool player_alive;
	bool player_respawn;
	bool player_climbing;
	bool player_in_air;
	bool spacebar_pushed;
	p2SString texture_source;
	SDL_Texture* player_spritesheet;
	player_colision_state col_state;

	//Set to xml
	bool isSliding = false;


};

//Player states
enum state 
{
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

//Input states
enum inputin 
{
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

//Output states
enum inputout 
{
	O_NONE = 0,
	O_RIGHT,
	O_LEFT,
	O_UP,
	O_DOWN,
	O_JUMP_FINISH,
	O_DEAD_FINISH
};

//Player class
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
	void LoadAnimationFromTMX(pugi::xml_node* animation_node, Animation* anim,const char* name);

	//Load sound by TMX
	void LoadSoundFXFromTMX(pugi::xml_node* sound_node, unsigned int& fx, const char* name);

	//Change col_state state
	void Change_Col_State(player_colision_state state);

	//Move player to spawn
	void MoveToSpawn();

public:

	Player_Info player;
	Animation idle;
	Animation run;
	Animation jump;
	Animation idle_ladder;
	Animation movement_ladder;
	Animation death;
	unsigned int death_fx;
	unsigned int jump_up_fx;
	unsigned int jump_down_fx;
	unsigned int switch_fx;
	int animation_created_mov;
	int killLimit;
	bool tangSwitchDeadCheck;
	bool canJump;

private:

	int gravity;
	int maximum_speed;
	int difference_y;
	int inputs_out;
	state actual_state;
	inputin input_in;
	inputout input_out[MAX_INPUTS_OUT];

	pugi::xml_document	player_info_file;
};

#endif // __j1PLAYER_H__