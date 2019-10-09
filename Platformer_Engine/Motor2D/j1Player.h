#ifndef __j1PLAYER_H__
#define __j1PLAYER_H__

#include "j1Module.h"
#include"p2Point.h"
#include "Animation.h"

struct Player_Info
{

	p2Point<int> player_position;
	SDL_Texture* player_spritesheet;


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
	bool Update(float dt);

	// Called before quitting
	bool CleanUp();

public:
	Player_Info player;
	Animation idle;
	Animation run;
	Animation jump;
	


};

#endif // __j1PLAYER_H__